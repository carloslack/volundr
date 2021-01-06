/*
 * Völundr 0.1 / volundr.c
 *
 * [Völundr] the ruler of the elves
 *
 * Author: hash
 *
 * Brief: Elf file reader / Elf file parasite( well, not yet...)
 */

/**
 * @defgroup volundr Volundr
 *
 *     @file volundr.c
 *    @brief The evil parasite.
 */

#include <string.h>
#include <errno.h>

#include "elfo.h"
#include "parse.h"
#include "utils.h"
#include "log.h"
#include "map.h"
#include "asm.h"
#include "print.h"
#include "validate.h"
#include "destroy.h"

enum vflags {
    V_ALL, V_HEADER = 1, V_PROGRAM = 2,
    V_SECTION = 4, V_SYM = 8
};

static bool _vol_print_file(FILE *felf, FILE *fout, enum vflags flags)
{
    ASSERT_ARG_RET_FALSE(felf && fout);

    log_debug("whatever");
    // validate it
    if(!elf_validate_filetype(felf)) {
        log_warning("Not a valid ELF file");
        return false;
    }

    // read, parse and print
    elf_t *elfo = elf_parse_file(felf);
    elf_set_my_elfo(elfo);


    if (!flags || flags & V_HEADER) {
        if(!elf_print_header(fout)) {
            log_error("failed reading headers");
            return false;
        }
    }

    if (!flags || flags & V_PROGRAM) {
        if(!elf_print_programs(fout)) {
            log_error("failed reading programs");
            return false;
        }
    }

    if (!flags || flags & V_SECTION) {
        if(!elf_print_sections(fout)) {
            log_error("failed reading sections");
            return false;
        }
    }

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
 * Main program
 *
 * @defgroup main Main
 * @ingroup volundr
 * @addtogroup
 * @{
 */

/**
 * @defgroup local Local
 * @ingroup main
 *
 * Local functions.
 *
 * @addtogroup
 * @{
 */

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

/** @} local */

/**
 * @brief .dtors
 */
static void __attribute__((destructor))  outVolundr() {
    elf_t *elfo = elf_get_my_elfo();
    elf_destroy_program(elfo);
    elf_destroy_section(elfo);
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
        _vol_print_file(felf, fout, flags);
        asm_close(fileno(felf));
    }

    return 0;
}
/** @} main */
