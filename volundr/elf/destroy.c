
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
bool elf_destroy_header(elf_t *elf) {
    ASSERT_CON_NULL(elf);
    elf_t **e = &elf;
    /** Ehdr here is just the pointer to memory area */
    return !!((map_fileunmap((*e)->mapaddr,
                    (*e)->fsize)) == 0);
}

/**
 * @brief Free-up Program Header
 *
 * @param elfo Main EFL object
 * @see elf_parse_file
 * @return True on success
 */
bool elf_destroy_program(elf_t *elf) {
    ASSERT_CON_NULL(elf);
    elf_t **e = &elf;
    sfree((void**)&(*e)->phdrs);
    return !!((*e)->phdrs == NULL);
}

/**
 * @brief Free-up Section Header
 *
 * @param elfo Main EFL object
 * @return True on success
 */
bool elf_destroy_section(elf_t *elf) {
    ASSERT_CON_NULL(elf);
    elf_t **e = &elf;
    sfree((void**)&(*e)->shdrs);
    return !!((*e)->shdrs == NULL);
}

/**
 * @brief Free-up Symbol Table
 *
 * @param elfo Main EFL object
 * @return True
 */
bool elf_destroy_symbol_table(elf_t *elf) {
    ASSERT_CON_NULL(elf);
    /** ELF object can have no symbols */
    elf_t **e = &elf;
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
bool elf_destroy_global_symbol_table(elf_t *elf) {
    ASSERT_CON_NULL(elf);
    /** ELF object can have no symbols */
    elf_t **e = &elf;
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
bool elf_destroy_elfo(elf_t *elf) {
    ASSERT_CON_NULL(elf);
    elf_t **e = &elf;
    sfree((void**)e);
    return !!(*e == NULL);
}
/** @} */
