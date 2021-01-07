/**
 *    @file  parse.c
 *   @brief  Parse general ELF information.
 *
 * TODO: add "@param x" to doxygen comments
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "common.h"
#include "utils.h"
#include "log.h"
#include "elfo.h"
#include "parse.h"
#include "write.h"
#include "validate.h"
#include "log.h"
#include "map.h"

/**
 * @defgroup parse Parse
 * @ingroup elf
 * This set of functions provides mechanisms to retrieve information from an
 * ELF image.
 */

/************ INTERFACES ************/
/**
 * @defgroup interfaces
 * @ingroup parse
 * @addtogroup interfaces Interfaces
 * @{
 */
/**
 * @brief Read ELF Header
 *
 * Contract:
 * @note This function depends on populating elf_t's raw image element
 * (elf_t->mapaddr). Currently the only possibility is reading ELF from
 * a disk file (by calling @ref elf_parse_file).
 * @see elf_parse_file
 *
 * @return A pointer to the ELF Header.
 */
static elf_ehdr_t* _elf_parse_ehdr(const elf_t *elfo)
{
    ASSERT_ARG_RET_NULL(elfo);
    ASSERT_CON_RET_NULL(elfo->mapaddr);
    return (elf_ehdr_t*)elfo->mapaddr;
}

/**
 * @brief Read Program Headers
 *
 * Contract:
 * @note This method depends on both populating the ELF object with image's
 * raw data and calling _elf_parse_ehdrs.
 * @see _elf_parse_ehdrs
 *
 * @return An array referencing all Program Headers in ELF image.
 * @note The array itself is unique for each function call, but its elements
 * are references to the ELF raw image in @ref elf_t interface. Never try
 * freeing these elements.
 */
static elf_phdr_t** _elf_parse_phdrs(const elf_t *elfo)
{
    ASSERT_ARG_RET_NULL(elfo);
    ASSERT_CON_RET_NULL(elfo->mapaddr && elfo->ehdr);

#ifdef PARANOID_CHECK
    // check for inconsistency
    size_t elf_size = (size_t) elfo->ehdr->e_phentsize;
    size_t vol_size = sizeof(elf_phdr_t);

    if(vol_size != elf_size) {
        log_error("Invalid program header size : "
                "rcvd: %d xpctd: %d", elf_size, vol_size);
    }
#endif
    int i;
    elf_phdr_t **array = smalloc((PHNUM(elfo)+1) * sizeof(elf_phdr_t*));
    elf_phdr_t *ptr = (elf_phdr_t*)(elfo->mapaddr + elfo->ehdr->e_phoff);

    for(i=0; i<PHNUM(elfo); i++) { array[i] = ptr++; }
    array[i] = (elf_phdr_t*)0;

    return array;
}

/**
 * @brief Read Section Headers
 *
 * Contract:
 * @note This method depends on calling _elf_parse_ehdrs first.
 * @see _elf_parse_ehdrs
 *
 * @return An array with all Section Header in ELF image.
 * @note The array itself is unique for each function call, but its elements
 * are references to the ELF raw image in @ref elf_t interface. Never try
 * freeing these elements.
 */
static elf_shdr_t** _elf_parse_shdrs(const elf_t *elfo)
{
    ASSERT_ARG_RET_NULL(elfo);
    ASSERT_CON_RET_NULL(elfo->mapaddr && elfo->ehdr);

#ifdef PARANOID_CHECK
    // check for inconsistency
    size_t elf_size = (size_t) elfo->ehdr->e_shentsize;
    size_t vol_size = sizeof(elf_shdr_t);

    if(vol_size != elf_size) {
        log_error("Invalid program header size : "
                "rcvd: %d xpctd: %d", elf_size, vol_size);
    }
#endif
    int i;
    elf_shdr_t **array = smalloc((SHNUM(elfo)+1) * sizeof(elf_shdr_t*));
    elf_shdr_t *ptr = (elf_shdr_t*)(elfo->mapaddr + elfo->ehdr->e_shoff);

    for(i=0; i<SHNUM(elfo); i++) { array[i] = ptr++; }
    array[i] = (elf_shdr_t*)0;

    return array;
}

/**
 * @brief Look-up Section Header pointer from file by its name
 *
 * Contract:
 * @note This method depends on calling elf_parse_shdrs first.
 * @see elf_parse_shdrs
 *
 * @return A pointer to Section Header
 */
static elf_shdr_t *_elf_parse_shdr_byname(const elf_t *elfo, const sbyte *name)
{
    ASSERT_ARG_RET_NULL(elfo && name != NULL);
    ASSERT_CON_RET_NULL(elfo->shdrs);

    int i;
    size_t arg_len = strlen(name);

    for(i=0; i<SHNUM(elfo); i++) {
        sbyte *curr = elf_parse_shname(elfo->shdrs[i]);
        if(curr != NULL) {
            if(strlen(curr) == arg_len && !strncmp(name, curr, arg_len)) {
                return elfo->shdrs[i];
            }
        }
    }
    return NULL;
}

/**
 * This method reads an ELF file to memory and returns an interface to
 * the parsed content.
 *
 * @note Allocating the whole file to memory is possibly not the smartest of the
 * implementation, but it's the simplest for sure, and simple is good.
 *
 * @brief Creates an interface to an ELF image on disk.
 *
 * @return An interface to ELF image on disk.
 */
elf_t *elf_parse_file(FILE *fp)
{
    ASSERT_ARG_RET_NULL(fp);
    elf_t *elfo;
    struct mapped_file user_data;
    elfo = smalloc(sizeof(elf_t));

    bool rc = file_read_all(&user_data, fp);
    ASSERT_ARG_RET_FALSE(rc);

    // create interface
    elfo->fsize = user_data.st.st_size;
    elfo->mapaddr = user_data.mapaddr;

    // read ELF headers
    elfo->ehdr  = _elf_parse_ehdr(elfo);
    elfo->phdrs = _elf_parse_phdrs(elfo);
    elfo->shdrs = _elf_parse_shdrs(elfo);

    return elfo;
}

/*@} ELF.interfaces */

/************ Headers ************/
/**
 * @defgroup headers
 * @ingroup parse
 * @addtogroup headers Headers
 * @{
 */



/** @}*/

/************ SECTIONS ************/
/**
 * @defgroup sections
 * @ingroup parse
 * @addtogroup sections Sections
 * @{
 */

/**
 * @brief Look-up Section Header index from file by its name
 *
 * Contract:
 * @note This method depends on calling elf_parse_shdrs first.
 * @see elf_parse_shdrs
 *
 * @return Index of given section, if any
 */
elf_word_t elf_parse_shdr_idx_byname(const sbyte *name)
{
    const elf_t *elfo = (const elf_t *)elf_get_my_elfo();
    elf_word_t idx = -1;
    if(!(elfo && elfo->shdrs && name != NULL))
        return idx;

    size_t arg_len = strlen(name);

    for(elf_word_t i=0; i<SHNUM(elfo) && idx == -1; i++) {
        sbyte *curr = elf_parse_shname(elfo->shdrs[i]);
        if(strlen(curr) == arg_len && !strncmp(name, curr, arg_len))
            idx = i;
    }
    return idx;
}

/**
 * @brief Read Section Header Name using its index as reference
 *
 * Contract:
 * @note This method depends on calling elf_parse_shdrs first.
 * @see elf_parse_shdrs
 *
 * @return A string containing section's name
 */
sbyte *elf_parse_shname_byindex(elf_word_t shidx)
{
    const elf_t *elfo = (const elf_t *)elf_get_my_elfo();
    elf_word_t n = 0;
    sbyte *shstrtab = NULL;

    ASSERT_ARG_RET_NULL(elf_validate_index(elfo, shidx));
    ASSERT_CON_RET_NULL(elfo->shdrs);

    shstrtab = elf_parse_shstrtab(NULL);
    ASSERT_RET_NULL(shstrtab && n, "Invalid .shstrtab");

    elf_word_t index = elfo->shdrs[shidx]->sh_name;
    ASSERT_RET_NULL(shidx < n, "Invalid string table index");

    return (shstrtab + index);
}

/**
 * @brief Read Section Header Name
 *
 * Contract:
 * @note This method depends on calling elf_parse_shdrs first.
 * @see elf_parse_shdrs
 *
 * @return A string containing section's name
 */
sbyte *elf_parse_shname(const elf_shdr_t *shdr)
{
    ASSERT_CON_RET_NULL(shdr);

    sbyte *shstrtab = elf_parse_shstrtab(NULL);
    return shstrtab ? SHNAME(shdr,shstrtab) : NULL;
}

/**
 * @brief Read all section headers of a given type.
 *
 * Contract:
 * @note This method depends on calling elf_parse_shdrs first.
 * @see elf_parse_shdrs
 *
 * @return A null terminated array of section header pointers.
 * @note The array itself is unique for each function call, but its elements
 * are references to the ELF raw image in @ref elf_t interface. Never try
 * freeing these elements.
 */
elf_shdr_t **elf_parse_shdrs_bytype(elf_word_t *types, u32 ntypes)
{
    const elf_t *elfo = (const elf_t *)elf_get_my_elfo();
    ASSERT_ARG_RET_NULL(types);
    ASSERT_CON_RET_NULL(elfo->shdrs);

    // rather spend some extra space than calling realloc several times.
    elf_shdr_t **sections = smalloc(SHNUM(elfo)*sizeof(elf_shdr_t*));

    int i,j,cnt = 0;
    for(i=0; i<elfo->ehdr->e_shnum && elfo->shdrs[i]; i++) {
        for(j=0; j<ntypes; j++) {
            if(elfo->shdrs[i]->sh_type == types[j]) {
                sections[cnt++] = elfo->shdrs[i];
                break;
            }
        }
    }
    sections[cnt] = NULL;
    sections = srealloc(sections, (cnt+1)*sizeof(elf_shdr_t));
    return sections;
}

/** @} */

/************ SYMBOLS ************/
/**
 * @defgroup symbols
 * @ingroup parse
 * @addtogroup symbols Symbols
 * @{
 */

/**
 * @brief Returns pointers to all symbol tables in ELF file.
 *
 * Contract:
 * @note This method depends on calling elf_parse_shdrs first.
 * @see elf_parse_shdrs
 *
 * @return A null-terminated array of all symbol table section headers. Returns
 * NULL either when no symbol table is found, or when an error occurs.
 *
 * This method depends on calling elf_parse_shdrs first.
 */
elf_shdr_t **elf_parse_all_symtabs(void)
{
    elf_word_t types[] = {SHT_SYMTAB, SHT_DYNSYM};
    elf_shdr_t **symtabs = elf_parse_shdrs_bytype(types, 2);
    return symtabs;
}

/**
 * TODO: comments
 */
elf_sym_t **elf_parse_all_syms(const elf_shdr_t *symtab)
{
    const elf_t *elfo = (const elf_t *)elf_get_my_elfo();
    ASSERT_ARG_RET_NULL(symtab);
    ASSERT_CON_RET_NULL(symtab->sh_type == SHT_SYMTAB ||
            symtab->sh_type == SHT_DYNSYM);

    size_t size = SENTNUM(symtab)+1 * sizeof(elf_sym_t*);
    elf_sym_t **syms = smalloc(size * sizeof(elf_sym_t*));

    int i;
    for(i=0; i<SENTNUM(symtab); i++) {
        syms[i] = (elf_sym_t*)
            (elfo->mapaddr + symtab->sh_offset + symtab->sh_entsize*i);
    }
    syms[i] = (elf_sym_t*)0;
    return syms;
}

/**
 * @brief Read symbol from symbol table.
 *
 * @param symtab The Symbol Table
 * @param index Symbol's index in table (starting at 0).
 *
 * @return An ELF Symbol. Returns NULL if the symbol does not exist.
 */
#if 0
elf_sym_t *elf_parse_sym(const elf_shdr_t *symtab, elf_word_t index)
{
    const elf_t *elfo = (const elf_t *)elf_get_my_elfo();
    ASSERT_ARG_RET_NULL(symtab);
    ASSERT_CON_RET_NULL(elfo->mapaddr);

    if(index >= SENTNUM(symtab))
        return NULL; // symbol not found

#ifdef PARANOID_CHECK
    // TODO: validate sh_entsize and sh_off.
#endif

    return (elf_sym_t*)
        (elfo->mapaddr + symtab->sh_offset + symtab->sh_entsize*index);
}
#endif
/** @} */

/************ STRINGS ************/
/**
 * @defgroup strings
 * @ingroup parse
 * @addtogroup strings Strings
 * @{
 */

/**
 * @brief This method retrieves pointers to all string tables in ELF file.
 *
 * @return An array of pointers to all string tables in ELF file/image.
 * Returns NULL if fails. The array can be free()'d, but not its elements.
 *
 * @note This method depends on calling elf_parse_shdrs first.
 * @see elf_parse_shdrs
 */
#if 0
elf_shdr_t **elf_parse_all_strtabs(void)
{
    const elf_t *elfo = (const elf_t *)elf_get_my_elfo();
    elf_word_t types[] = {SHT_STRTAB};
    elf_shdr_t **strtabs = elf_parse_shdrs_bytype(types, 1);
    return strtabs;
}
#endif
/**
 * @brief Read string table pointed by section header of given index.
 *
 * Contract:
 * @note This method depends on calling elf_parse_shdrs first.
 * @see elf_parse_shdrs
 *
 * @return An array of bytes (hopefully, a sequence of strings) containing
 * all elements of string table pointed by section header of given index.
 */
char *elf_parse_strtab_byindex(elf_half_t sh_idx, elf_word_t *n, elf_sym_t **syms)
{
    const elf_t *elfo = (const elf_t *)elf_get_my_elfo();
    ASSERT_ARG_RET_NULL(elf_validate_index(elfo, sh_idx));
    elf_shdr_t *shdr = elfo->shdrs[sh_idx];
    sbyte *strings = elf_parse_shstrtab(syms);
    if(strings != NULL && n != NULL) {
        *n = shdr->sh_size;
    }
    return strings;
}

/**
 * @brief Read .shstrtab section's string table.
 *
 * Contract:
 * @note This method depends on calling both _elf_parse_ehdr and
 * elf_parse_shdrs first.
 * @see _elf_parse_ehdrs
 * @see elf_parse_shdrs
 *
 * @return An array of bytes (hopefully, a sequence of strings) containing
 * all elements of .shstrtab string table.
 */
sbyte* elf_parse_shstrtab(elf_sym_t **syms)
{
    const elf_t *elfo = (const elf_t *)elf_get_my_elfo();
    ASSERT_CON_RET_NULL(elfo->ehdr && elfo->shdrs);
    if(elfo->ehdr->e_shstrndx == SHN_UNDEF) {
        log_debug("undefined string table");
        return NULL;
    }
    elf_shdr_t *shstrtab = elfo->shdrs[elfo->ehdr->e_shstrndx];
    return (char*)elfo->mapaddr + shstrtab->sh_offset;
}

/**
 * @brief Read .dynstr section's string table.
 *
 * @return An array of bytes (hopefully, a sequence of strings) containing
 * all elements of .dynstr string table.
 *
 * 
 *
 */
sbyte* elf_parse_section_by_name(const char *name)
{
    const elf_t *elfo = (const elf_t *)elf_get_my_elfo();
    elf_shdr_t *sct = _elf_parse_shdr_byname(elfo, name);
    if(sct == NULL) {
        log_debug("%s not found!", name);
        return NULL;
    }
    return (char*)elfo->mapaddr + sct->sh_offset;
}

/** @} */

/* __EOF__ */
