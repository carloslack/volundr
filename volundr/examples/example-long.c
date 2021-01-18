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
            "-o <output file>   Log output into file instead of stdout\n"
            "-v                 Show program version\n"
            "-t                 Show header\n"
            "-p                 Show programs\n"
            "-s                 Show sections\n"
            "-y                 Show symbols\n"
            "-h                 Show this help\n", prog);
}

/**
 * @brief Prints version tag to standard output.
 */
static void version(void)
{
    log_it("Völundr %s\n", elf_get_version());
}
/**
 * @brief parse and print elf header, program, sections and symbols
 */
static bool dump_elf_data(const char *filename, FILE *felf, FILE *fout, enum vflags flags, open_mode_t m)
{
    ASSERT_ARG_RET_FALSE(felf && fout);

    // validate it
    if(!elf_validate_filetype(felf)) {
        log_warning("Not a valid ELF file");
        return false;
    }

    /* Load the file in the hope it is an ELF */
    elf_t *elfo = elf_parse_file(filename, felf, m);


    /* Display ELF Header */
    if (!flags || flags & V_HEADER) {
        if(!elf_print_header(elfo, fout)) {
            log_error("failed reading headers");
            return false;
        }
    }

    /* Display programs section */
    if (!flags || flags & V_PROGRAM) {
        if(!elf_print_programs(elfo, fout)) {
            log_error("failed reading programs");
            return false;
        }
    }

    /* Display ELF sections */
    if (!flags || flags & V_SECTION) {
        if(!elf_print_sections(elfo, fout)) {
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
                elf_print_symtab(elfo, fout, sht_dynsym[i], (const elf_sym_t**)syms);
            }
        }
        elf_shdr_t **sht_symtab = elf_get_section_header(elfo, SHT_SYMTAB);
        if (sht_symtab) {
            for(int i=0; sht_symtab[i] != NULL; i++) {
                elf_sym_t **syms = elf_load_section_header_symbols(elfo, sht_symtab[i]);
                elf_print_symtab(elfo, fout, sht_symtab[i], (const elf_sym_t**)syms);
            }
        }
    }

    /* The mother of all cleanups */
    assert(elf_destroy_all(elfo));

    return true;
}

/**
 * @brief Main
 */
int main(int argc, char** argv)
{
    int c, flags = V_ALL;
    char *binfile = NULL, *output = NULL;
    FILE *fout = stdout;

    while ((c = getopt(argc, argv, "f:o:tpcsyhv")) != -1) {
        switch (c) {
            case 'f':
                binfile = optarg;
                break;
            case 'o':
                output = optarg;
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
                version();
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

    open_mode_t m;
    if (output != NULL)
        /** m here is just to respect
         * the contract. We care about it only
         * in file_open_ro() below */
        fout = file_open_ow(output, &m);

    FILE *felf = file_open_ro(binfile, &m);
    if (felf) {
        /* Go! */
        dump_elf_data(binfile, felf, fout, flags, m);

        /* volundr implements some x64 syscalls */
        asm_close(fileno(felf));
    }


    if (fout != stdout)
        file_close(fout);
    file_close(felf);

    return 0;
}
/** @} main */
