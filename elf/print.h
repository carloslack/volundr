#ifndef _ELF_PRINT_H
#define _ELF_PRINT_H

#include <elf.h>
#include "types.h"
#include <stdio.h>

bool elf_print_elf(FILE*, const elf_t *);
bool elf_print_header(FILE*, const elf_t*);
bool elf_print_programs(FILE*, const elf_t*);
bool elf_print_sections(FILE*, const elf_t*);

bool elf_print_symtab(FILE *, const elf_t*, const elf_shdr_t *);
bool elf_print_all_symtabs(FILE *, const elf_t *);

bool elf_print_shstrtab(FILE *, const elf_t *);
bool elf_print_strtab(FILE *, const elf_t *);
bool elf_print_dynstr(FILE *, const elf_t *);
bool elf_print_shstrtab_index(FILE *, const elf_t *, elf_word_t);
bool elf_print_strtab_index(FILE *, const elf_t*);

#ifdef __ELF_SYSV__
bool elf_print_dyntab_index(FILE *, const elf_t *, elf_word_t);
#endif

#endif
