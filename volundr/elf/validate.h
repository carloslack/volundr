#ifndef _ELF_VALIDATE_H
#define _ELF_VALIDATE_H

#include <stdio.h>
#include <elf.h>

bool elf_validate_eheader(const elf_ehdr_t*);
bool elf_validate_filetype(FILE *); 
bool elf_validate_index(const elf_t *, elf_half_t);

#endif
