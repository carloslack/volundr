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
static bool dump_elf_data(FILE *felf, FILE *fout, enum vflags flags)
{
    ASSERT_ARG_RET_FALSE(felf && fout);

    log_debug("whatever");
    // validate it
    if(!elf_validate_filetype(felf)) {
        log_warning("Not a valid ELF file");
        return false;
    }

    /* Load the file in the hope it is an ELF */
    elf_t *elfo = elf_parse_file(felf);
    elf_set_my_elfo(elfo);


    /* Display ELF Header */
    if (!flags || flags & V_HEADER) {
        if(!elf_print_header(fout)) {
            log_error("failed reading headers");
            return false;
        }
    }

    /* Display programs section */
    if (!flags || flags & V_PROGRAM) {
        if(!elf_print_programs(fout)) {
            log_error("failed reading programs");
            return false;
        }
    }

    /* Display ELF sections */
    if (!flags || flags & V_SECTION) {
        if(!elf_print_sections(fout)) {
            log_error("failed reading sections");
            return false;
        }
    }

    /* Display symbols */
    if (!flags || flags & V_SYM) {
        elf_shdr_t **sym_tables = elf_parse_all_symtabs();
        if (sym_tables) {
            for(int i=0; sym_tables[i] != NULL; i++)
                elf_print_symtab(fout, sym_tables[i]);
        }
    }

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

    if (output != NULL)
        fout = file_open_ow(output);

    FILE *felf = file_open_ro(binfile);
    if (felf) {
        /* Go! */
        dump_elf_data(felf, fout, flags);

        /* volundr implements some x64 syscalls */
        asm_close(fileno(felf));
    }

    /* Get the main elf object */
    elf_t *elfo = elf_get_my_elfo();

    /* Do some cleanup */
    assert(elf_destroy_header(elfo));
    assert(elf_destroy_program(elfo));
    assert(elf_destroy_section(elfo));
    assert(elf_destroy_elfo(elfo));

    return 0;
}
/** @} main */
