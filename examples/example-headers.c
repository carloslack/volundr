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
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "common.h"
#include "utils.h"
#include "log.h"
#include "elfo.h"
#include "parse.h"
#include "map.h"
#include "asm.h"
#include "print.h"
#include "validate.h"
#include "destroy.h"

extern elf_info_t   _program[];

static bool doit(const char *binfile) {
    ASSERT_CON_RET_FALSE(binfile);
    open_mode_t m;
    struct mapped_file map = {0};
    const char *file = binfile;
    FILE *fp = file_open_ro(file, &m);

    if (!fp) {
        log_error("Can't open file");
        return false;
    }

    if(!elf_validate_filetype(fp)) {
        log_error("Not a valid ELF file");
        file_close(fp);
        return false;
    }


    if (!file_load_target(&map, fp, m)) {
        log_error("Error loading target ELF\n");
        return false;
    }

    elf_t *elfo = elf_parse_file(file, &map);

    elf_ehdr_t *ehdr = elf_get_elf_header(elfo);
    if (ehdr)
        log_info("Ehdr=%p %c%c%c\n",
                ehdr, ehdr->e_ident[EI_MAG1], ehdr->e_ident[EI_MAG2], ehdr->e_ident[EI_MAG3]);

    elf_phdr_t **phdrs = elf_get_elf_programs(elfo);
    if (phdrs) {
        log_info("Phdrs=%p Number of Programs %d:\n", phdrs, ehdr->e_phnum);

        for (int i = 0, phidx; i < ehdr->e_phnum; ++i, (*phdrs)++) {
            const elf_phdr_t *phdr = (const elf_phdr_t *)*phdrs;
            ELF_DICT(&phidx, program, phdr->p_type);
            log_info("\t%d: %s\n",i,  _program[phidx].name);
        }
        free(phdrs);
    }

    elf_shdr_t **shdrs = elf_get_elf_sections(elfo);
    if (shdrs) {
        log_info("Shdrs=%p Number of Sections %d:\n", phdrs, ehdr->e_shnum);

        for(int i=0; i < ehdr->e_shnum; i++, (*shdrs)++) {
            char *shname = elf_get_section_header_name(elfo, *shdrs);
            log_info("\t%d: %s\n",i, strlen(shname) ? shname : "--");
        }
        free(shdrs);
    }

    assert(elf_destroy_all(elfo));
    file_close(fp);
    return true;
}

int main(int argc, char **argv) {
    if (argc < 2 || !strcmp(argv[1], "-h")) {
        log_info("Use %s <elf file>\n", argv[0]);
        asm_exit(0);
    }

    (void)doit(argv[1]);

    return 0;
}
