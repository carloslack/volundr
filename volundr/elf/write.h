#ifndef _ELF_WRITE_H
#define _ELF_WRITE_H

#include <stdio.h>

void elf_write_strtab(int32_t, int32_t, const char*);
void elf_write_stringname(FILE*, int32_t);

#endif
