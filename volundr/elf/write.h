#ifndef _ELF_WRITE_H
#define _ELF_WRITE_H

#include <stdio.h>

void elf_write_strtab(i32, i32, const sbyte*);
void elf_write_stringname(FILE*, i32);

#endif
