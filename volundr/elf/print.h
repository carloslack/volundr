#ifndef _ELF_PRINT_H
#define _ELF_PRINT_H

#include <elf.h>
#include <stdio.h>

bool elf_print_header(const elf_t *, FILE *);
bool elf_print_programs(const elf_t *, FILE *);
bool elf_print_sections(const elf_t *, FILE *);
bool elf_print_symtab(const elf_t*, FILE *fout, const elf_shdr_t *, const elf_sym_t **);
//bool elf_print_strtab(FILE *, const elf_t *);
//bool elf_print_dynstr(FILE *, const elf_t *);
//bool elf_print_shstrtab_index(FILE *, const elf_t *, elf_word_t);
//bool elf_print_strtab_index(FILE *, const elf_t*);

#endif
