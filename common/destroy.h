#include "../elf/types.h"
#include "utils.h"

bool destroy_meta       (elf_t* elf);
bool destroy_header     (elf_t* elf);
bool destroy_program    (elf_t* elf);
bool destroy_section    (elf_t* elf);
bool destroy_file       (elf_t* elf);
