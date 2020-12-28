#ifndef _ELF_PARSE_H
#define _ELF_PARSE_H

/**
 *    @file  parse.h
 *   @brief  Parse general ELF information (definition module).
 *  @author  v00d00
 */

#include <stdio.h>
#include "types.h"
#include "destroy.h"

/*
 * Interfaces
 */

/* read ELF file and return its image */
elf_t *elf_parse_file(FILE *);
elf_t *elf_parse_content(void *);

/*
 * Headers
 */
/* read ELF header */
elf_ehdr_t* elf_parse_ehdr(const elf_t*);

/* read all program headers */
elf_phdr_t** elf_parse_phdrs(const elf_t*);

/* read all section headers */
elf_shdr_t** elf_parse_shdrs(const elf_t*);


/*
 *  Sections
 */
/* read section header pointer by its literal name */
elf_shdr_t *elf_parse_shdr_byname(const elf_t *, const sbyte *);

/* read section's header index by its name */
elf_word_t elf_parse_shdr_idx_byname(const elf_t *, const sbyte *);

/* read all section headers of given types */
elf_shdr_t **elf_parse_shdrs_bytype(const elf_t *, elf_word_t*, u32);

/* read name from section of given index */
sbyte *elf_parse_shname_byindex(const elf_t *, elf_word_t);

/* read section's name */
sbyte *elf_parse_shname(const elf_t *elfo, const elf_shdr_t *);


/*
 * Symbols
 */
/* read all symbol tables from ELF file */
elf_shdr_t **elf_parse_all_symtabs(const elf_t *);

/* read all symbols from provided symbol table */
elf_sym_t **elf_parse_all_syms(const elf_t *, const elf_shdr_t *);

/* read symbol from section pointed by section header */
elf_sym_t *elf_parse_sym(const elf_t *, const elf_shdr_t *, elf_word_t);


/*
 * Strings
 */
/* read string table byte array from section */
sbyte* elf_parse_strtab(const elf_t *, const elf_shdr_t*);

/* read all string table section headers from ELF file */
elf_shdr_t **elf_parse_all_strtabs(const elf_t *);

/* read strtab byte array from section of given index */
sbyte* elf_parse_strtab_byindex(const elf_t *, elf_half_t, elf_word_t *);

/* read .shstrtab byte array */
sbyte* elf_parse_shstrtab(const elf_t *);

/* read .dynstr byte array */
sbyte* elf_parse_dynstr(const elf_t *);

#endif
