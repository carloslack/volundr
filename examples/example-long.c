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

/**
 *  example-long.c
 *  Example code that shows the use of
 *  volundr library
 *
 *  Compile:
 *      make
 *  Run
 *      LD_LIBRARY_PATH=volundr ./example-long -h
 */

#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdbool.h>

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

enum vflags {
    V_ALL, V_HEADER = 1, V_PROGRAM = 2,
    V_SECTION = 4, V_SYM = 8
};

/**
 * @brief Prints usage to standard output.
 */
static void usage(char *prog)
{
    log_it( "Usage %s <OPTION>... \n"
            "-f <elf file>      Input binary to be analysed\n"
            "-v                 Show Volundr version\n"
            "-t                 Show header\n"
            "-p                 Show programs\n"
            "-s                 Show sections\n"
            "-y                 Show symbols\n"
            "-h                 Show this help\n", prog);
}

/**
 * @brief parse and print elf header, program, sections and symbols
 */
static bool doit(const char *binfile, enum vflags flags)
{
    ASSERT_CON_RET_FALSE(binfile);

    open_mode_t m;
    struct mapped_file map = {0};
    FILE *felf = file_open_ro(binfile, &m);
    if (!felf) {
        log_error("Can't open file");
        return false;
    }

    // validate it
    if(!elf_validate_filetype(felf)) {
        log_warning("Not a valid ELF file");
        return false;
    }

    if (!file_load_target(&map, felf, m)) {
        log_error("Error loading target ELF\n");
        return false;
    }

    elf_t *elfo = elf_parse_file(binfile, &map);

    /* Display ELF Header */
    if (!flags || flags & V_HEADER) {
        if(!elf_print_header(elfo, stdout)) {
            log_error("failed reading headers");
            return false;
        }
    }

    /* Display programs section */
    if (!flags || flags & V_PROGRAM) {
        if(!elf_print_programs(elfo, stdout)) {
            log_error("failed reading programs");
            return false;
        }
    }

    /* Display ELF sections */
    if (!flags || flags & V_SECTION) {
        if(!elf_print_sections(elfo, stdout)) {
            log_error("failed reading sections");
            return false;
        }
    }

    /* Display symbols */
    if (!flags || flags & V_SYM) {
        elf_shdr_t **sht_dynsym = elf_get_section_header(elfo, SHT_DYNSYM);
        if (sht_dynsym) {
            for(int i=0; sht_dynsym[i] != NULL; i++) {
                elf_sym_t **syms = elf_load_section_header_symbols(elfo, sht_dynsym[i]);
                elf_print_symtab(elfo, stdout, sht_dynsym[i], (const elf_sym_t**)syms);
            }
        }
        elf_shdr_t **sht_symtab = elf_get_section_header(elfo, SHT_SYMTAB);
        if (sht_symtab) {
            for(int i=0; sht_symtab[i] != NULL; i++) {
                elf_sym_t **syms = elf_load_section_header_symbols(elfo, sht_symtab[i]);
                elf_print_symtab(elfo, stdout, sht_symtab[i], (const elf_sym_t**)syms);
            }
        }
    }

    /* The mother of all cleanups */
    assert(elf_destroy_all(elfo));
    file_close(felf);

    return true;
}

/**
 * @brief Main
 */
int main(int argc, char** argv)
{
    int c, flags = V_ALL;
    char *binfile = NULL;

    while ((c = getopt(argc, argv, "f:tpcsyhv")) != -1) {
        switch (c) {
            case 'f':
                binfile = optarg;
                break;
            case 't':
                flags |= V_HEADER;
                break;
            case 'p':
                flags |= V_PROGRAM;
                break;
            case 's':
                flags |= V_SECTION;
                break;
            case 'y':
                flags |= V_SYM;
                break;
            case 'h':
                usage(argv[0]);
                asm_exit(EXIT_SUCCESS);
            case 'v':
                log_it("Völundr %s\n", elf_get_version());
                asm_exit(EXIT_SUCCESS);
            default:
                usage(argv[0]);
                asm_exit(EXIT_SUCCESS);
        }
    }
    if (!binfile) {
        usage(argv[0]);
        asm_exit(EXIT_FAILURE);
    }

    /* Go! */
    if (!doit(binfile, flags))
        return 1;

    return 0;
}
/** @} main */
