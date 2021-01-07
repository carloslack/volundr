#ifndef _ELF_PARSE_H
#define _ELF_PARSE_H

/**
 *    @file  parse.h
 *   @brief  Parse general ELF information (definition module).
 *  @author  v00d00
 */

#include <stdio.h>

/*
 * Interfaces
 */

/* read ELF file and return its image */
elf_t *elf_parse_file(FILE *);

/*
 * Headers
 */

/* read all section headers */
elf_shdr_t** elf_parse_shdrs(const elf_t*);


/*
 *  Sections
 */
/* read section's header index by its name */
elf_word_t elf_parse_shdr_idx_byname(const sbyte *);

/* read all section headers of given types */
elf_shdr_t **elf_parse_shdrs_bytype(elf_word_t*, u32);

/* read name from section of given index */
sbyte *elf_parse_shname_byindex(elf_word_t);

/* read section's name */
sbyte *elf_parse_shname(const elf_shdr_t *);


/*
 * Symbols
 */
/* read all symbol tables from ELF file */
elf_shdr_t **elf_parse_all_symtabs(void);

/* read all symbols from provided symbol table */
elf_sym_t **elf_parse_all_syms(const elf_shdr_t *);

/* read symbol from section pointed by section header */
elf_sym_t *elf_parse_sym(const elf_shdr_t *, elf_word_t);


/* read all string table section headers from ELF file */
elf_shdr_t **elf_parse_all_strtabs(void);

/* read strtab byte array from section of given index */
char *elf_parse_strtab_byindex(elf_half_t, elf_word_t *i, elf_sym_t **);

sbyte* elf_parse_shstrtab(elf_sym_t **);

/* read .dynstr byte array */
sbyte* elf_parse_section_by_name(const char *);

#endif
