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
 *    @file  parse.c
 *   @brief  Parse general ELF information.
 *
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "common.h"
#include "utils.h"
#include "log.h"
#include "elfo.h"
#include "parse.h"
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
 * Helper
 * Lookup the Section Headers and return the one
 * matching name.
 */
static elf_shdr_t *_elf_get_shdr_byname(const elf_t *elfo, const char *name)
{
    ASSERT_ARG_RET_NULL(elfo && name != NULL);
    ASSERT_CON_RET_NULL(elfo->shdrs);

    elf_shdr_t *shdr = NULL;
    size_t len = strlen(name);

    for(int i = 0; i<SHNUM(elfo) && !shdr; i++) {
        char *curr = elf_get_section_header_name(elfo, elfo->shdrs[i]);
        if(curr && strlen(curr) == len && !strncmp(name, curr, len))
            shdr = elfo->shdrs[i];
    }
    return shdr;
}

/**
 * @brief Read ELF Header
 *
 * Contract:
 * @note This function depends on populating (mmap) elf_t's raw image element
 * (elf_t->mapaddr). Currently the only possibility is reading ELF from
 * a disk file (by calling @ref elf_parse_file).
 * @see elf_parse_file
 *
 * @return A pointer to the ELF Header.
 */
elf_ehdr_t* elf_parse_ehdr(const elf_t *elfo)
{
    ASSERT_ARG_RET_NULL(elfo);
    ASSERT_CON_RET_NULL(elfo->mapaddr);
    return (elf_ehdr_t*)elfo->mapaddr;
}

/**
 * Contract:
 * @note This method depends on both populating the ELF object with image's
 * raw data and calling elf_parse_ehdr.
 * @see elf_parse_ehdr
 *
 * @return An array referencing all Program Headers in ELF image.
 * @note The array itself is unique for each function call, but its elements
 * are references to the ELF raw image in @ref elf_t interface. Never try
 * freeing these elements.
 *
 *
 * Parse program headers
 *  Return an array referencing all Program Headers in ELF image
 */
elf_phdr_t** elf_parse_phdrs(const elf_t *elfo)
{
    ASSERT_ARG_RET_NULL(elfo);
    ASSERT_CON_RET_NULL(elfo->mapaddr && elfo->ehdr);

    // check for inconsistency
    size_t elf_size = (size_t) elfo->ehdr->e_phentsize;
    size_t vol_size = sizeof(elf_phdr_t);

    if(vol_size != elf_size) {
        log_error("Invalid program header size : "
                "rcvd: %d xpctd: %d", elf_size, vol_size);
    }

    int i;
    elf_phdr_t **array = smalloc((PHNUM(elfo)+1) * sizeof(elf_phdr_t*));
    elf_phdr_t *ptr = (elf_phdr_t*)(elfo->ehdr->e_phoff +
            (unsigned char*)elfo->mapaddr);

    for(i=0; i<PHNUM(elfo); i++) { array[i] = ptr++; }
    array[i] = (elf_phdr_t*)0;

    return array;
}

/**
 * Load a map of number and index of
 * programs in phdrs for direct access
 */
static void _elf_load_programs_map(elf_t *elfo)
{
    memset(&elfo->phmap, 0, sizeof(elfo->phmap));
    for (int i = 0; i < PHNUM(elfo); ++i) {
        int p_type = LAZYMAP(elfo->phdrs[i]->p_type);
        int nr = elfo->phmap[p_type].nr;
        elfo->phmap[p_type].map[nr] = i;
        elfo->phmap[p_type].nr++;
    }
}

/**
 * Load a map of number and index of
 * sections in shdrs for direct access
 */
static void _elf_load_sections_map(elf_t *elfo)
{
    memset(&elfo->shmap, 0, sizeof(elfo->shmap));
    for (int i = 0; i < SHNUM(elfo); ++i) {
        int sh_type = LAZYMAP(elfo->shdrs[i]->sh_type);
        int nr = elfo->shmap[sh_type].nr;
        elfo->shmap[sh_type].map[nr] = i;
        elfo->shmap[sh_type].nr++;
    }
}

/**
 * @brief Read Section Headers
 *
 * Contract:
 * @note This method depends on calling elf_parse_ehdr first.
 * @see elf_parse_ehdr
 *
 * @return An array with all Section Header in ELF image.
 * @note The array itself is unique for each function call, but its elements
 * are references to the ELF raw image in @ref elf_t interface. Never try
 * freeing these elements.
 */
elf_shdr_t** elf_parse_shdrs(const elf_t *elfo)
{
    ASSERT_ARG_RET_NULL(elfo);
    ASSERT_CON_RET_NULL(elfo->mapaddr && elfo->ehdr);

    // check for inconsistency
    size_t elf_size = (size_t) elfo->ehdr->e_shentsize;
    size_t vol_size = sizeof(elf_shdr_t);

    if(vol_size != elf_size)
        log_error("Invalid program header size : "
                "rcvd: %d xpctd: %d", elf_size, vol_size);
    int i;
    elf_shdr_t **array = smalloc((SHNUM(elfo)+1) * sizeof(elf_shdr_t*));
    elf_shdr_t *ptr = (elf_shdr_t*)(elfo->mapaddr + elfo->ehdr->e_shoff);

    for(i=0; i<SHNUM(elfo); i++)
        array[i] = ptr++;

    array[i] = (elf_shdr_t*)0;

    return array;
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
elf_t *elf_parse_file(const char *filename, const struct mapped_file *map)
{
    ASSERT_ARG_RET_NULL(filename);
    ASSERT_CON_RET_NULL(map);
    elf_t *elfo = smalloc(sizeof(elf_t));

    // create interface
    elfo->elf_size = map->st.st_size;
    elfo->mapaddr = map->mapaddr;
    strncpy(elfo->filename, filename, sizeof(elfo->filename));

    elfo->syms_symtab = NULL;
    elfo->syms_dynsym = NULL;
    elfo->sht_symtab  = NULL;
    elfo->sht_dynsym  = NULL;

    // read ELF headers
    elfo->ehdr  = elf_parse_ehdr(elfo);
    elfo->phdrs = elf_parse_phdrs(elfo);
    elfo->shdrs = elf_parse_shdrs(elfo);

    // Basic check for endianess
    ASSERT_RET_NULL((elfo->ehdr->e_ident[EI_DATA] == 1 ||
                elfo->ehdr->e_ident[EI_DATA] == 2), "Invalid endianess");

    // Load mapping of number & index
    // of Programs and sections
    _elf_load_programs_map(elfo);
    _elf_load_sections_map(elfo);

    return elf_set_my_elfo(elfo);
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
elf_word_t elf_parse_shdr_idx_byname(const elf_t *elfo, const char *name)
{
    elf_word_t idx = -1;
    if(!(elfo && elfo->shdrs && name != NULL))
        return idx;

    size_t len = strlen(name);

    for(elf_word_t i=0; i<SHNUM(elfo) && idx == -1; i++) {
        char *curr = elf_get_section_header_name(elfo, elfo->shdrs[i]);
        if(strlen(curr) == len && !strncmp(name, curr, len))
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
char *elf_parse_shname_byindex(const elf_t *elfo, elf_word_t shidx)
{
    elf_word_t n = 0;
    char *shstrtab = NULL;

    ASSERT_ARG_RET_NULL(elf_validate_index(elfo, shidx));
    ASSERT_CON_RET_NULL(elfo->shdrs);

    shstrtab = elf_parse_shstrtab(elfo, NULL);
    ASSERT_RET_NULL(shstrtab && n, "Invalid .shstrtab");

    /** sh_name: offset to .shstrtab */
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
char *elf_get_section_header_name(const elf_t *elfo, const elf_shdr_t *shdr)
{
    ASSERT_CON_RET_NULL(shdr);

    char *shstrtab = elf_parse_shstrtab(elfo, NULL);
    return shstrtab ? SHNAME(shdr,shstrtab) : NULL;
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
 * @brief Store current global symbol table.
 * @see _elf_store_symbol_table
 * @note only call this from elf_load_section_header_global_symbols
 * @see elf_load_section_header_global_symbols
 */
static void _elf_store_global_symbol_table(const elf_t *elfo, elf_shdr_t **shdrs
        , elf_word_t sh_type)
{
    /** sh_type: content type, e.g.: static & dynamic symbols,
     * rolocation information...
     */
    elf_t *e = (elf_t*)elfo;
    if (sh_type == SHT_SYMTAB)
        e->sht_symtab = shdrs;
    else
        e->sht_dynsym = shdrs;
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
 * @note it stores global symbol table on elfo object
 * @note stored object must be freed when no longer needed
 */
elf_shdr_t **elf_load_section_header_global_symbols(const elf_t *elfo, elf_word_t sh_type)
{
    ASSERT_ARG_RET_FALSE(sh_type == SHT_SYMTAB || sh_type == SHT_DYNSYM);
    ASSERT_CON_RET_NULL(elfo->shdrs);

    int shnum = SHNUM(elfo);
    if (!shnum) {
        log_debug("no symbols");
        return NULL;
    }

    int cnt = 0;
    elf_shdr_t **shdrs = smalloc(shnum*sizeof(elf_shdr_t*));
    for(int i=0; i<shnum && elfo->shdrs[i]; i++) {
        if(elfo->shdrs[i]->sh_type == sh_type)
            shdrs[cnt++] = elfo->shdrs[i];
    }

    shdrs[cnt] = NULL;
    shdrs = srealloc(shdrs, (cnt+1)*sizeof(elf_shdr_t));

    _elf_store_global_symbol_table(elfo, shdrs, sh_type);

    return shdrs;
}

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
elf_shdr_t **elf_get_section_header(const elf_t *elfo, elf_word_t sh_type)
{
    elf_shdr_t **symtabs = elf_load_section_header_global_symbols(elfo, sh_type);
    return symtabs;
}

/**
 * @brief Store current symbol table.
 *  This is useful mainly for two reason: to avoid
 *  leaking when we go out of context and to allow callee
 *  to either keep the reference for later use, although there
 *  is no free lunch the these pointers need to be freed when no
 *  longer needed.
 *  @note only call this from elf_load_section_header_symbols
 *  @see elf_load_section_header_symbols
 */
static void _elf_store_symbol_table(const elf_t *elfo, elf_sym_t **syms, elf_word_t sh_type)
{
    elf_t *e = (elf_t *)elfo;
    if (sh_type == SHT_SYMTAB)
        e->syms_symtab = syms;
    else
        e->syms_dynsym = syms;
}
/**
 * @brief Read symbols from symbol table.
 *
 * @param elfo The main elf object (handler)
 * @param symtab The Symbol Table
 *
 * @return An array of ELF Symbols. Returns NULL if there are no symbols
 */

elf_sym_t **elf_load_section_header_symbols(const elf_t *elfo, elf_shdr_t *symtab)
{
    ASSERT_ARG_RET_NULL(symtab);
    ASSERT_CON_RET_NULL(symtab->sh_type == SHT_SYMTAB ||
            symtab->sh_type == SHT_DYNSYM);

    int sentnum = SENTNUM(symtab);
    if (!sentnum) {
        log_debug("no symbols");
        return NULL;
    }

    size_t size = sentnum+1 * sizeof(elf_sym_t*);
    elf_sym_t **syms = smalloc(size * sizeof(elf_sym_t*));

    int i;
    for(i=0; i < sentnum; i++) {
        syms[i] = (elf_sym_t*)
            (symtab->sh_offset + symtab->sh_entsize*i +
             (unsigned char*)elfo->mapaddr);
    }
    syms[i] = (elf_sym_t*)0;

    /** Store the current symbol table */
    _elf_store_symbol_table(elfo, syms, symtab->sh_type);

    return syms;
}

/**
 * @brief Read string table pointed by section header of given index.
 *
 * Contract:
 * @note This method depends on calling elf_parse_shdrs first.
 * @see elf_parse_shdrs
 *
 * @param elfo The main elf object (handler)
 * @return An array of bytes (hopefully, a sequence of strings) containing
 * all elements of string table pointed by section header of given index.
 */
char *elf_parse_strtab_byindex(const elf_t *elfo, elf_half_t sh_idx, elf_word_t *n, elf_sym_t **syms)
{
    ASSERT_ARG_RET_NULL(elf_validate_index(elfo, sh_idx));
    elf_shdr_t *shdr = elfo->shdrs[sh_idx];
    char *strings = elf_parse_shstrtab(elfo, syms);
    if(strings != NULL && n != NULL) {
        /** sh_size: size of the section in isk */
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
 * @see elf_parse_ehdr
 * @see elf_parse_shdrs
 *
 * @param elfo The main elf object (handler)
 * @return An array of bytes (hopefully, a sequence of strings) containing
 * all elements of .shstrtab string table.
 */
char* elf_parse_shstrtab(const elf_t *elfo, elf_sym_t **syms)
{
    ASSERT_CON_RET_NULL(elfo && elfo->ehdr && elfo->shdrs);
    if(elfo->ehdr->e_shstrndx == SHN_UNDEF) {
        log_debug("undefined string table");
        return NULL;
    }
    elf_shdr_t *shstrtab = elfo->shdrs[elfo->ehdr->e_shstrndx];
    /** sh_offset: offset of the first byte of the section in disk */
    return ((char*)elfo->mapaddr + shstrtab->sh_offset);
}

/**
 * @brief Read .dynstr section's string table.
 *
 * @param elfo The main elf object (handler)
 * @return An array of bytes (hopefully, a sequence of strings) containing
 * all elements of .dynstr string table.
 */
unsigned char* elf_get_symname_offset(const elf_t *elfo, const char *name)
{
    elf_shdr_t *sct = _elf_get_shdr_byname(elfo, name);
    if(sct == NULL) {
        log_debug("%s not found!", name);
        return NULL;
    }
    return ((unsigned char*)elfo->mapaddr + sct->sh_offset);
}


/************ GETTERS ************/
/**
 * @defgroup getters
 * @ingroup parse
 * @addtogroup getters Getters
 * @{
 */

elf_ehdr_t *elf_get_elf_header(const elf_t *elfo) {
    return elf_parse_ehdr(elfo);
}

elf_phdr_t **elf_get_elf_programs(const elf_t *elfo) {
    return elf_parse_phdrs(elfo);
}

elf_shdr_t **elf_get_elf_sections(const elf_t *elfo) {
    return elf_parse_shdrs(elfo);
}

/** @} */

/** @} */
/* __EOF__ */
