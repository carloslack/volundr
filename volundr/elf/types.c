#define _ELF_TYPES_OWNER

#include <stdio.h>
#include <elf.h>
#define _TYPES_DECLARE
#include "common.h"
#include "utils.h"
#include "log.h"
#include "elfo.h"
#undef _TYPES_DECLARE

elf_info_t _type[]={
    {0, 0, "NOTYPE"},
    {1, 1, "OBJECT"},
    {2, 2, "FUNC"},
    {3, 3, "SECTION"},
    {4, 4, "FILE"},
    {5, 10, "LOOS"},
    {6, 12, "HIOS"},
    {7, 13, "LOPROC"},
    {8, 15, "HIPROC"},
    {ELFDEF, ELFDEF, "unhandled"}
};

elf_info_t _bind[]={
    {1, 0, "LOCAL"},
    {2, 1, "GLOBAL"},
    {3, 2, "WEAK"},
    {4, 10, "LOOS"},
    {5, 12, "HIOS"},
    {6, 13, "LOPROC"},
    {7, 15, "HIPROC"},
    {ELFDEF, ELFDEF, "unhandled"}
};

elf_info_t _visibility[]={
    {0, 0, "DEFAULT"},
    {1, 1, "INTERNAL"},
    {2, 2, "HIDDEN"},
    {3, 10, "PROTECTED"},
    {ELFDEF, ELFDEF, "unhandled"}
};

elf_info_t _index[]={
    {0, SHN_ABS, "ABS"},
    {1, SHN_COMMON, "COM"},
    {2, SHN_UNDEF, "UND"},
    {3, SHN_XINDEX, "COM"},
    {ELFDEF, ELFDEF, "unhandled"}
};

elf_info_t _machine[]={
    {0, EM_NONE , "No machine"},
    {1, EM_X86_64 , "AMD x86-64 architecture"},
    {2, EM_M32, "AT&T WE32100" },
    {3, EM_SPARC, "SPARC"},
    {4, EM_386, "Intel Arch"},
    {5, EM_68K, "Motorola 6800"},
    {6, EM_88K, "Motorola 88000"},
    {7, EM_860, "Intel 80860"},
    {8, EM_MIPS, "MIPS RS3000 big-endian"},
    {9, EM_MIPS_RS3_LE, "MIPS RS4000 little-endian"},
    {ELFDEF, ELFDEF, "unhandled"}
};

elf_info_t _version[]={
    {0, 0, "Invalid version"},
    {1, 1, "CUrrent version"},
    {ELFDEF, ELFDEF, "unhandled"}
};

elf_info_t _header[]={
    {0, ET_NONE, "No file type"},
    {1, ET_REL, "Relocatable file"},
    {2, ET_EXEC, "Executable file"},
    {3, ET_DYN, "Shared object file"},
    {4, ET_CORE, "Core file"},
    {5, ET_NUM, "Number of defined types"},
    {6, ET_LOOS, "OS-specific range start"},
    {7, ET_HIOS, "OS-specific range end"},
    {8, ET_LOPROC, "Processor-specific range start"},
    {9, ET_HIPROC, "Processor-specific range end"},
    {ELFDEF, ELFDEF, "unhandled"}
};

elf_info_t _program[]={
    {0, PT_NULL, "Unused"},
    {1, PT_LOAD, "Loadable segment"},
    {2, PT_DYNAMIC, "Dynamic linking" },
    {3, PT_INTERP, "Program interpreter" },
    {4, PT_NOTE, "Auxiliary information"},
    {5, PT_SHLIB, "Reserved" },
    {6, PT_PHDR, "Header table entry" },
    {7, PT_TLS, "Thread-local storage segment"},
    {8, PT_NUM, "Number of defined types"},
    {9, PT_LOOS, "Start of OS-specific" },
    {10, PT_GNU_EH_FRAME, "GCC .eh_frame_hdr segment"},
    {11, PT_GNU_STACK, "Stack executability"},
    {11, PT_GNU_RELRO, "Read-only after relocation"},
    {13, PT_PAX_FLAGS, "PaX flag markings" },  /* PT 13 porra! */
    {ELFDEF, ELFDEF, "unhandled"},
};

