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
 *    @file  destroy.c
 *   @brief  Free memory interface
 */

#include <errno.h>
#include "common.h"
#include "utils.h"
#include "log.h"
#include "elfo.h"
#include "destroy.h"
#include "asm.h"
#include "map.h"

/**
 * @defgroup cleanup Cleanup
 * @ingroup elf
 * This set of functions provides mechanisms to free memory allocated
 * during ELF parse and manipulation
 */

/************ CLEANUP ************/
/**
 * @defgroup interfaces
 * @ingroup cleanup
 * @{
 */

/**
 * @brief Free-up memory area of EFL raw file
 *
 * @param elfo Main EFL object
 * @see elf_parse_file
 * @return True on success
 */
bool elf_destroy_header(elf_t *elfo) {
    ASSERT_CON_NULL(elfo);
    elf_t **e = &elfo;
    /** Ehdr here is just the pointer to memory area */
    return !!((map_fileunmap((*e)->mapaddr,
                    (*e)->elf_size)) == 0);
}

/**
 * @brief Free-up Program Header
 *
 * @param elfo Main EFL object
 * @see elf_parse_file
 * @return True on success
 */
bool elf_destroy_program(elf_t *elfo) {
    ASSERT_CON_NULL(elfo);
    elf_t **e = &elfo;
    sfree((void**)&(*e)->phdrs);
    return !!((*e)->phdrs == NULL);
}

/**
 * @brief Free-up Section Header
 *
 * @param elfo Main EFL object
 * @return True on success
 */
bool elf_destroy_section(elf_t *elfo) {
    ASSERT_CON_NULL(elfo);
    elf_t **e = &elfo;
    sfree((void**)&(*e)->shdrs);
    return !!((*e)->shdrs == NULL);
}

/**
 * @brief Free-up Symbol Table
 *
 * @param elfo Main EFL object
 * @return True
 */
bool elf_destroy_symbol_table(elf_t *elfo) {
    ASSERT_CON_NULL(elfo);
    /** ELF object can have no symbols */
    elf_t **e = &elfo;
    if ((*e)->syms_symtab) {
        free((*e)->syms_symtab);
        (*e)->syms_symtab = NULL;
    }
    if ((*e)->syms_dynsym) {
        free((*e)->syms_dynsym);
        (*e)->syms_dynsym = NULL;
    }
    return true;
}

/**
 * @brief Free-up Global Symbol Table
 *
 * @param elfo Main EFL object
 * @return True
 */
bool elf_destroy_global_symbol_table(elf_t *elfo) {
    ASSERT_CON_NULL(elfo);
    /** ELF object can have no symbols */
    elf_t **e = &elfo;
    if ((*e)->sht_symtab) {
        free((*e)->sht_symtab);
        (*e)->sht_symtab = NULL;
    }
    if ((*e)->sht_dynsym) {
        free((*e)->sht_dynsym);
        (*e)->sht_dynsym = NULL;
    }
    return true;
}

/**
 * @brief Free-up Main ELF object
 *
 * @param elfo Main EFL object
 * @return True on success
 */
bool elf_destroy_elfo(elf_t *elfo) {
    ASSERT_CON_NULL(elfo);
    elf_t **e = &elfo;
    sfree((void**)e);
    return !!(*e == NULL);
}

bool elf_destroy_all(elf_t *elfo) {
    bool rc1 = elf_destroy_program(elfo);
    bool rc2 = elf_destroy_section(elfo);
    bool rc3 = elf_destroy_global_symbol_table(elfo);
    bool rc4 = elf_destroy_symbol_table(elfo);
    bool rc5 = elf_destroy_header(elfo);
    bool rc6 = elf_destroy_elfo(elfo);

    return (rc1 && rc2 && rc3 && rc4 && rc5 && rc6);
}

/** @} */
