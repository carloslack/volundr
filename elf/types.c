#define _ELF_TYPES_OWNER

#include <stdio.h>
#define _TYPES_DECLARE
#include "types.h"
#undef _TYPES_DECLARE

elf_info_t _header[]={
    {0, "e_ident: ", NULL},
    {1, "e_type: ", HALF32},
    {2, "e_machine: ", HALF32},
    {3, "e_version: ", WORD32},
    {4, "e_entry: ", nADDR32},
    {5, "e_phoff: ", nINT},
    {6, "e_shoff: ", nINT},
    {7, "e_flags: ", nWORD32},
    {8, "e_ehsize: ", nINT},
    {9, "e_phentsize: ", nINT},
    {10, "e_phnum: ", nINT},
    {11, "e_shentsize: ", nINT},
    {12, "e_shnum: ", nINT},
    {13, "e_shstrndx: ", nINT},
    {-1, NULL, NULL}
};

elf_info_t _section[]={
    {0, "sh_name: ", WORD32},
    {1, "sh_type: ", nWORD32},
    {2, "sh_flags: ", nWORD32},
    {3, "sh_addr: ", nADDR32},
    {4, "sh_offset: ", nINT},
    {5, "sh_size: ", nINT},
    {6, "sh_link: ", nWORD32},
    {7, "sh_info: ", nWORD32},
    {8, "sh_addralign: ", nWORD32},
    {9, "sh_entsize: ", nINT},
    {-1, NULL, NULL}
};

elf_info_t _program[]={
    {0, "p_type: ", WORD32},
    {1, "p_offset: ", nINT},
    {2, "p_vaddr: ", nADDR32},
    {3, "p_paddr: ", nADDR32},
    {4, "p_filez: ", nINT},
    {5, "p_memsz: ", nINT},
    {6, "p_flags: ", nWORD32},
    {7, "p_align: ", nINT},
    {-1, NULL, NULL},
};

elf_info_t _type[]={
    {0, "NOTYPE", SBYTE},
    {1, "OBJECT", SBYTE},
    {2, "FUNC", SBYTE},
    {3, "SECTION", SBYTE},
    {4, "FILE", SBYTE},
    {10, "LOOS", SBYTE},
    {12, "HIOS", SBYTE},
    {13, "LOPROC", SBYTE}, // ignore sparc
    {15, "HIPROC", SBYTE},
    {-1, NULL, NULL},
};

elf_info_t _info[]={
    {0, "LOCAL", SBYTE},
    {1, "GLOBAL", SBYTE},
    {2, "WEAK", SBYTE},
    {10, "LOOS", SBYTE},
    {12, "HIOS", SBYTE},
    {13, "LOPROC", SBYTE},
    {15, "HIPROC", SBYTE},
    {-1, NULL, NULL},
};

elf_info_t _visibility[]={
    {0, "DEFAULT", SBYTE},
    {1, "INTERNAL", SBYTE},
    {2, "HIDDEN", SBYTE},
    {10, "PROTECTED", SBYTE},
    {-1, NULL, NULL},
};
//mdata mData;
