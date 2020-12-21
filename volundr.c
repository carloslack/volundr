/*
 * Völundr 0.1 / volundr.c
 *
 * [Völundr] the ruler of the elves
 *
 * Author: Carlos Carvalho
 * Contact: carloslack@gmail.com
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

#include "volundr.h"
#include "elf/types.h"
#include "elf/file.h"
#include "elf/parse.h"
#include "common/utils.h"
#include "common/log.h"
#include "common/map.h"

/** local functions */
static void usage(sbyte*);
static void version(void);

// TODO: ifdef GNU

elf_t *vol_open_file(const char *path, const char *mode)
{
    ASSERT_ARG_RET_NULL(path && mode);
    FILE *fp = fopen(path, mode);

    if(fp == NULL) {
        log_error("can't open file : %s : %s", path, strerror(errno));
        return NULL;
    }
    return elf_parse_file(fp);
}

bool vol_print_file(const char *elf_file_path, FILE *fout)
{
    ASSERT_ARG_RET_FALSE(elf_file_path && fout);

    // open elf file
    FILE *felf = file_open_ro(elf_file_path);
    if(felf == NULL) {
        log_error("Could not read ELF file");
        return false;
    }

    // validate it
    if(!elf_validate_filetype(felf)) {
        log_warning("Not a valid ELF file");
        return false;
    }

    // read, parse and print
    elf_t *elfo = elf_parse_file(felf);
    (void) elf_print_elf(fout, elfo);
    // TODO: elf_destroy_elf(elfo);

    fclose(felf);
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
static void usage(sbyte* it)
{
    version();
    printf( "Use %s [ARGS] <elf-file> \n"
            "-f <output> \t : write output to file\n", it);
}

/**
 * @brief Prints version tag to standard output.
 */
static void version(void)
{
    printf("Völundr %s\n", VERSION);
}

/** @} local */

/**
 * @brief .ctors
 */
static void __CONSTRUCTOR__ inVolundr() {
    // Here we initialize data, behavior, environment, etc..
    //log_debug("Initialization takes place");
}

/**
 * @brief .dtors
 */
static void __DESTRUCTOR__ outVolundr() {
    // Here we unitialize data, free memory, behavior, environment, etc...
    //log_debug("Cleaning up");
}

/**
 * @brief Main
 */
int main(int argc, char** argv)
{
    int c, i;
    char *fout_path = NULL;
    FILE *fout;

    if(argc<2) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    while ((c = getopt(argc, argv, "f:hv")) != -1) {
        switch (c) {
            // output file
            case 'f': {
                          ASSERT_OPT_ONCE(fout_path == NULL);
                          fout_path = optarg;
                          break;
                      }
                      // version
            case 'v': {
                          version();
                          return 0;
                          break;
                      }
                      // help
            case 'h': {
                          usage(argv[0]);
                          return 0;
                      }
                      // elf file
            default: {
                         exit(EXIT_FAILURE);
                         break;
                     }
        }
    }
    if(optind >= argc) {
        log_error("You must provide an ELF image.");
        exit(EXIT_FAILURE);
    }

    // decide where to print output messages
    if(fout_path == NULL) {
        fout = stdout;
    } else { // open output stream.
        fout = file_open_ow(fout_path);
        if(!fout) {
            log_error("Error reading output file : %s\n", strerror(errno));
            exit(1);
        }
    }

    // finally...
    for(i=optind; i<argc; i++) {
        printf("\nVolundr -> %s\n\n", argv[i]);
        vol_print_file(argv[i], fout);
    }

    return 0;
}
/** @} main */
