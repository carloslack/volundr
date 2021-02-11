#ifndef _ELF_PARSE_H
#define _ELF_PARSE_H


elf_t *elf_parse_file                                   (const char *, const struct mapped_file *);
elf_shdr_t** elf_parse_shdrs                            (const elf_t*);
elf_word_t elf_parse_shdr_idx_byname                    (const elf_t *elfo, const char *);
elf_shdr_t **elf_load_section_header_global_symbols     (const elf_t *elfo, elf_word_t);
char *elf_parse_shname_byindex                         (const elf_t *elfo, elf_word_t);
char *elf_get_section_header_name                       (const elf_t *elfo, const elf_shdr_t *);
elf_ehdr_t* elf_parse_ehdr                              (const elf_t *elfo);
elf_phdr_t** elf_parse_phdrs                            (const elf_t *elfo);
elf_phdr_t** elf_parse_phdrs                            (const elf_t *elfo);
elf_shdr_t **elf_get_section_header                     (const elf_t *elfo, elf_word_t);
elf_sym_t **elf_load_section_header_symbols             (const elf_t *elfo,elf_shdr_t *);
//elf_sym_t *elf_parse_sym(const elf_shdr_t *, elf_word_t);
//elf_shdr_t **elf_parse_all_strtabs(void);
char *elf_parse_strtab_byindex                          (const elf_t *elfo, elf_half_t, elf_word_t *i, elf_sym_t **);
char* elf_parse_shstrtab                               (const elf_t *elfo, elf_sym_t **);
unsigned char *elf_get_symname_offset                   (const elf_t *elfo, const char *);


elf_ehdr_t *elf_get_elf_header                          (const elf_t *elfo);
elf_phdr_t **elf_get_elf_programs                       (const elf_t *elfo);
elf_shdr_t **elf_get_elf_sections                       (const elf_t *elfo);

#endif
