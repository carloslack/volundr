#define _ELF_TYPES_OWNER

#include <stdio.h>
#define _TYPES_DECLARE
#include "types.h"
#undef _TYPES_DECLARE

elf_info_t _type[]={
    {0, "NOTYPE"},
    {1, "OBJECT"},
    {2, "FUNC"},
    {3, "SECTION"},
    {4, "FILE"},
    {10, "LOOS"},
    {12, "HIOS"},
    {13, "LOPROC"},
    {15, "HIPROC"},
    {-1, NULL},
};

elf_info_t _info[]={
    {0, "LOCAL"},
    {1, "GLOBAL"},
    {2, "WEAK"},
    {10, "LOOS"},
    {12, "HIOS"},
    {13, "LOPROC"},
    {15, "HIPROC"},
    {-1, NULL},
};

elf_info_t _visibility[]={
    {0, "DEFAULT"},
    {1, "INTERNAL"},
    {2, "HIDDEN"},
    {10, "PROTECTED"},
    {-1, NULL},
};

elf_info_t _index[]={
    {SHN_ABS, "ABS"},
    {SHN_COMMON, "COM"},
    {SHN_UNDEF, "UND"},
    {SHN_XINDEX, "COM"},
    {-1, NULL},
};
//mdata mData;
