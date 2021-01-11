#ifndef _DESTROY_H
#define _DESTROY_H
bool elf_destroy_meta                   (elf_t *elf);
bool elf_destroy_header                 (elf_t *elf);
bool elf_destroy_program                (elf_t *elf);
bool elf_destroy_section                (elf_t *elf);
bool elf_destroy_symbol_table           (elf_t *elf);
bool elf_destroy_global_symbol_table    (elf_t *elf);
bool elf_destroy_file                   (elf_t *elf);
bool elf_destroy_elfo                   (elf_t *elf);
bool elf_destroy_all                    (elf_t *elfo);
#endif
