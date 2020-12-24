/**
 *    @file  parse.c
 *   @brief  Parse general ELF information.
 *
 * TODO: add "@param x" to doxygen comments
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "parse.h"
#include "write.h"
#include "validate.h"
#include "../common/utils.h" // XXX : use relative path (change Makefile)
#include "../common/log.h"
#include "../common/map.h"

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
elf_t* elf_parse_file(FILE *fp)
{
    ASSERT_ARG_RET_NULL(fp);
    void *data  = file_read_all(fp, NULL);
    return elf_parse_content(data, fp);
}

/**
 * This function parses ELF image (as raw data) and creates an interface
 * to all ELF parts defined in TIS ELF Spec v1.2.
 *
 * @brief Parse ELF image data and make references to all ELF parts
 * according to propoer abstract data type.
 *
 * @return An interface to the provided ELF image.
 */
elf_t *elf_parse_content(void *data, FILE* fp)
{
    ASSERT_ARG_RET_NULL(data);

    struct stat st;
    i32 fdesc = fileno(fp);

    // create interface
    elf_t *elfo;
    elfo = smalloc(sizeof(elf_t));
    elfo->data = (unsigned char*)data;

    /*
     * ATTENTION: this is just a testing for mmapping
     * Here we just map a file and then unmap it.
     */
    fstat(fdesc, &st)?log_fatal("fstat %s", strerror(errno)):0;
    elfo->fsize = st.st_size;
    elfo->mapaddr = map_filemap((void*)0
            , elfo->fsize
            , fdesc);
    if (map_fileunmap(elfo->mapaddr, elfo->fsize) == -1) {
        log_warning("Ooops! Error mapping fd %d of size %d",
                fileno(fp), elfo->fsize);
    }

    // read ELF headers
    elfo->ehdr  = elf_parse_ehdr(elfo);
    elfo->phdrs = elf_parse_phdrs(elfo);
    elfo->shdrs = elf_parse_shdrs(elfo);

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


/**
 * @brief Read ELF Header
 *
 * Contract:
 * @note This function depends on populating elf_t's raw image element
 * (elf_t->data). Currently the only possibility is reading ELF from
 * a disk file (by calling @ref elf_parse_file).
 * @see elf_parse_file
 *
 * @return A pointer to the ELF Header.
 */
elf_ehdr_t* elf_parse_ehdr(const elf_t *elfo)
{
    ASSERT_ARG_RET_NULL(elfo);
    ASSERT_CON_RET_NULL(elfo->data);
    return (elf_ehdr_t*)elfo->data;
}

/**
 * @brief Read Program Headers
 *
 * Contract:
 * @note This method depends on both populating the ELF object with image's
 * raw data and calling elf_parse_ehdrs.
 * @see elf_parse_ehdrs
 *
 * @return An array referencing all Program Headers in ELF image.
 * @note The array itself is unique for each function call, but its elements
 * are references to the ELF raw image in @ref elf_t interface. Never try
 * freeing these elements.
 */
elf_phdr_t** elf_parse_phdrs(const elf_t *elfo)
{
    ASSERT_ARG_RET_NULL(elfo);
    ASSERT_CON_RET_NULL(elfo->data && elfo->ehdr);

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
    elf_phdr_t *ptr = (elf_phdr_t*)(elfo->data + elfo->ehdr->e_phoff);

    for(i=0; i<PHNUM(elfo); i++) { array[i] = ptr++; }
    array[i] = (elf_phdr_t*)0;

    return array;
}

/**
 * @brief Read Section Headers
 *
 * Contract:
 * @note This method depends on calling elf_parse_ehdrs first.
 * @see elf_parse_ehdrs
 *
 * @return An array with all Section Header in ELF image.
 * @note The array itself is unique for each function call, but its elements
 * are references to the ELF raw image in @ref elf_t interface. Never try
 * freeing these elements.
 */
_INLINE
elf_shdr_t** elf_parse_shdrs(const elf_t *elfo)
{
    ASSERT_ARG_RET_NULL(elfo);
    ASSERT_CON_RET_NULL(elfo->data && elfo->ehdr);

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
    elf_shdr_t *ptr = (elf_shdr_t*)(elfo->data + elfo->ehdr->e_shoff);

    for(i=0; i<SHNUM(elfo); i++) { array[i] = ptr++; }
    array[i] = (elf_shdr_t*)0;

    return array;
}

/** @}*/

/************ SECTIONS ************/
/**
 * @defgroup sections
 * @ingroup parse
 * @addtogroup sections Sections
 * @{
 */

/**
 * @brief Look-up Section Header pointer from file by its name
 *
 * Contract:
 * @note This method depends on calling elf_parse_shdrs first.
 * @see elf_parse_shdrs
 *
 * @return A pointer to Section Header
 */
elf_shdr_t *elf_parse_shdr_byname(const elf_t *elfo, const sbyte *name)
{
    ASSERT_ARG_RET_NULL(elfo && name != NULL);
    ASSERT_CON_RET_NULL(elfo->shdrs);

    int i;
    size_t arg_len = strlen(name);

    for(i=0; i<SHNUM(elfo); i++) {
        sbyte *curr = elf_parse_shname(elfo, elfo->shdrs[i]);
        if(curr != NULL) {
            if(strlen(curr) == arg_len && !strncmp(name, curr, arg_len)) {
                return elfo->shdrs[i];
            }
        }
    }
    return NULL;
}

/**
 * @brief Look-up Section Header index from file by its name
 *
 * Contract:
 * @note This method depends on calling elf_parse_shdrs first.
 * @see elf_parse_shdrs
 *
 * @return Index of given section
 */
elf_word_t elf_parse_shdr_idx_byname(const elf_t *elfo, const sbyte *name)
{
    if( ! (elfo && elfo->shdrs && name != NULL) ) {
        return (elf_word_t)-1;
    }

    int i;
    size_t arg_len = strlen(name);

    for(i=0; i<SHNUM(elfo); i++) {
        sbyte *curr = elf_parse_shname(elfo, elfo->shdrs[i]);
        if(strlen(curr) == arg_len && !strncmp(name, curr, arg_len)) {
            return i;
        }
    }
    return (elf_word_t)-1;
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
sbyte *elf_parse_shname_byindex(const elf_t *elfo, elf_word_t shidx)
{
    ASSERT_ARG_RET_NULL(elf_validate_index(elfo, shidx));
    ASSERT_CON_RET_NULL(elfo->shdrs);

    elf_word_t n = 0;
    sbyte *shstrtab = NULL;

    shstrtab = elf_parse_shstrtab(elfo);
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
sbyte *elf_parse_shname(const elf_t *elfo, const elf_shdr_t *shdr)
{
    ASSERT_ARG_RET_NULL(elfo);
    ASSERT_CON_RET_NULL(shdr);

    sbyte *shstrtab = elf_parse_shstrtab(elfo);
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
elf_shdr_t **elf_parse_shdrs_bytype( const elf_t *elfo
                                  , elf_word_t *types
                                  , u32 ntypes)
{
    ASSERT_ARG_RET_NULL(elfo && types);
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
elf_shdr_t **elf_parse_all_symtabs(const elf_t *elfo)
{
    ASSERT_ARG_RET_NULL(elfo);
    elf_word_t types[] = {SHT_SYMTAB, SHT_DYNSYM};
    elf_shdr_t **symtabs = elf_parse_shdrs_bytype(elfo, types, 2);
    return symtabs;
}

/**
 * TODO: comments
 */
elf_sym_t **elf_parse_all_syms(const elf_t *elfo, const elf_shdr_t *symtab)
{
    ASSERT_ARG_RET_NULL(elfo && symtab);
    ASSERT_CON_RET_NULL(symtab->sh_type == SHT_SYMTAB ||
            symtab->sh_type == SHT_DYNSYM );
#ifdef PARANOID_CHECK
    // TODO: check if symtab is really in elfo.
    // TODO: check if SHT_NOBIT
#endif
    int i;
    elf_sym_t **syms = smalloc((SENTNUM(symtab)+1) * sizeof(elf_sym_t*));
    for(i=0; i<SENTNUM(symtab); i++) {
        syms[i] = (elf_sym_t*)
            (elfo->data + symtab->sh_offset + symtab->sh_entsize*i);
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
elf_sym_t *elf_parse_sym( const elf_t *elfo
                       , const elf_shdr_t *symtab
                       , elf_word_t index)
{
    ASSERT_ARG_RET_NULL(elfo && symtab);
    ASSERT_CON_RET_NULL(elfo->data);

    if(index >= SENTNUM(symtab))
        return NULL; // symbol not found

#ifdef PARANOID_CHECK
    // TODO: validate sh_entsize and sh_off.
#endif

    return (elf_sym_t*)
        (elfo->data + symtab->sh_offset + symtab->sh_entsize*index);
}

/** @} */

/************ STRINGS ************/
/**
 * @defgroup strings
 * @ingroup parse
 * @addtogroup strings Strings
 * @{
 */

/**
 * @brief Read string table pointed by section header
 *
 * @return An array os chars (hopefully, a sequence of strings) containing
 * all string table elements.
 */
sbyte* elf_parse_strtab(const elf_t *elfo, const elf_shdr_t *strtab)
{
    ASSERT_ARG_RET_NULL(elfo && strtab);
    if(strtab->sh_type != SHT_STRTAB) {
        log_error("Not a string table section (%d : %d).",
                strtab->sh_name, strtab->sh_type);
        return NULL;
    }
    return (sbyte*)elfo->data + strtab->sh_offset;
}

/**
 * @brief This method retrieves pointers to all string tables in ELF file.
 *
 * @return An array of pointers to all string tables in ELF file/image.
 * Returns NULL if fails. The array can be free()'d, but not its elements.
 *
 * @note This method depends on calling elf_parse_shdrs first.
 * @see elf_parse_shdrs
 */
elf_shdr_t **elf_parse_all_strtabs(const elf_t *elfo)
{
    ASSERT_ARG_RET_NULL(elfo);
    elf_word_t types[] = {SHT_STRTAB};
    elf_shdr_t **strtabs = elf_parse_shdrs_bytype(elfo, types, 1);
    return strtabs;
}

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
sbyte* elf_parse_strtab_byindex( const elf_t *elfo
                              , elf_half_t sh_idx
                              , elf_word_t *n )
{
    ASSERT_ARG_RET_NULL(elf_validate_index(elfo, sh_idx));
    elf_shdr_t *strtab = elfo->shdrs[sh_idx];
    sbyte *strings = elf_parse_strtab(elfo, strtab);
    if(strings != NULL && n != NULL) {
        *n = strtab->sh_size;
    }
    return strings;
}

/**
 * @brief Read .shstrtab section's string table.
 *
 * Contract:
 * @note This method depends on calling both elf_parse_ehdr and
 * elf_parse_shdrs first.
 * @see elf_parse_ehdrs
 * @see elf_parse_shdrs
 *
 * @return An array of bytes (hopefully, a sequence of strings) containing
 * all elements of .shstrtab string table.
 */
sbyte* elf_parse_shstrtab(const elf_t *elfo)
{
    ASSERT_ARG_RET_NULL(elfo);
    ASSERT_CON_RET_NULL(elfo->ehdr && elfo->shdrs);
    if(elfo->ehdr->e_shstrndx == SHN_UNDEF) {
        log_debug("undefined string table");
        return NULL;
    }
    elf_shdr_t *shstrtab = elfo->shdrs[elfo->ehdr->e_shstrndx];
    return elf_parse_strtab(elfo, shstrtab);
}

/**
 * @brief Read .dynstr section's string table.
 *
 * @return An array of bytes (hopefully, a sequence of strings) containing
 * all elements of .dynstr string table.
 */
sbyte* elf_parse_dynstr(const elf_t *elfo)
{
    elf_shdr_t *dynstr = elf_parse_shdr_byname(elfo, ".dynstr");
    if(dynstr == NULL) {
        log_debug(".dynstr not found!");
        return NULL;
    }
    return elf_parse_strtab(elfo, dynstr);
}

/** @} */

/* __EOF__ */
