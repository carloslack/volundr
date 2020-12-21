/**
 *
 * Völundr / general.h
 *
 * Main header file
 *
 */

#ifndef _VOLUNDR_H
#define _VOLUNDR_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#include "elf.h"

#define VERSION     "0.1"

#ifdef DEBUG
#define LOGF    logf_debug
#else
#define LOGF    logf_info
#endif

#define ELFNAME "-ELF.txt"

#define __CONSTRUCTOR__ __attribute__((constructor))
#define __DESTRUCTOR__ __attribute__((destructor))

#define ASSERT_OPT_ONCE(x) {                                      \
            if(!(x)) {                                            \
                log_error("invalid option : %s\n", argv[optind]); \
                usage(argv[0]);                                   \
                exit(EXIT_FAILURE);                               \
            }                                                     \
    }

elf_t * vol_open_file   (const char *, const char*);
int     vol_print_file  (const char*, FILE *);

#endif
