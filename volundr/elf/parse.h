#ifndef _ELF_PARSE_H
#define _ELF_PARSE_H


elf_t *elf_parse_file(const char *filename, FILE *);
elf_shdr_t** elf_parse_shdrs(const elf_t*);
elf_word_t elf_parse_shdr_idx_byname(const sbyte *);
elf_shdr_t **elf_load_section_header_global_symbols(elf_word_t);
sbyte *elf_parse_shname_byindex(elf_word_t);
char *elf_get_section_header_name(const elf_shdr_t *);
elf_ehdr_t* elf_parse_ehdr(const elf_t *elfo);
elf_phdr_t** elf_parse_phdrs(const elf_t *elfo);
elf_phdr_t** elf_parse_phdrs(const elf_t *elfo);
elf_shdr_t **elf_get_section_header(elf_word_t);
elf_sym_t **elf_load_section_header_symbols(elf_shdr_t *);
elf_sym_t *elf_parse_sym(const elf_shdr_t *, elf_word_t);
elf_shdr_t **elf_parse_all_strtabs(void);
char *elf_parse_strtab_byindex(elf_half_t, elf_word_t *i, elf_sym_t **);
sbyte* elf_parse_shstrtab(elf_sym_t **);
unsigned char *elf_get_symname_offset(const char *);

#endif
