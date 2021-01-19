/**
 *  example.c
 *  Example code that shows the use of
 *  volundr library
 *  - hash
 *
 *  Compile:
 *      make
 *  Run
 *      ./run -h
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

    /* Load the file in the hope it is an ELF */
    elf_t *elfo = elf_parse_file(binfile, felf, m);


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
    (void)doit(binfile, flags);

    return 0;
}
/** @} main */
