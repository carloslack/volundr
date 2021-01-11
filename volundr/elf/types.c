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
    {0, 0,                      "NOTYPE"},
    {1, 1,                      "OBJECT"},
    {2, 2,                      "FUNC"},
    {3, 3,                      "SECTION"},
    {4, 4,                      "FILE"},
    {5, 10,                     "LOOS"},
    {6, 12,                     "HIOS"},
    {7, 13,                     "LOPROC"},
    {8, 15,                     "HIPROC"},
    {ELFDEF, ELFDEF,            "unhandled"}
};

elf_info_t _bind[]={
    {1, 0,                      "LOCAL"},
    {2, 1,                      "GLOBAL"},
    {3, 2,                      "WEAK"},
    {4, 10,                     "LOOS"},
    {5, 12,                     "HIOS"},
    {6, 13,                     "LOPROC"},
    {7, 15,                     "HIPROC"},
    {ELFDEF, ELFDEF,            "unhandled"}
};

elf_info_t _visibility[]={
    {0, 0,                      "DEFAULT"},
    {1, 1,                      "INTERNAL"},
    {2, 2,                      "HIDDEN"},
    {3, 3,                      "PROTECTED"},
    {ELFDEF, ELFDEF,            "unhandled"}
};

elf_info_t _index[]={
    {0, SHN_ABS,                "ABS"},
    {1, SHN_COMMON,             "COM"},
    {2, SHN_UNDEF,              "UND"},
    {3, SHN_XINDEX,             "COM"},
    {ELFDEF, ELFDEF,            "unhandled"}
};

elf_info_t _machine[]={
    {0, EM_NONE ,               "No machine"},
    {1, EM_X86_64 ,             "AMD x86-64 architecture"},
    {2, EM_M32,                 "AT&T WE32100" },
    {3, EM_SPARC,               "SPARC"},
    {4, EM_386,                 "Intel Arch"},
    {5, EM_68K,                 "Motorola 6800"},
    {6, EM_88K,                 "Motorola 88000"},
    {7, EM_860,                 "Intel 80860"},
    {8, EM_MIPS,                "MIPS RS3000 big-endian"},
    {9, EM_MIPS_RS3_LE,         "MIPS RS4000 little-endian"},
    {ELFDEF, ELFDEF,            "unhandled"}
};

elf_info_t _version[]={
    {0, 0,                      "Invalid version"},
    {1, 1,                      "Current version"},
    {ELFDEF, ELFDEF,            "unhandled"}
};

elf_info_t _header[]={
    {0, ET_NONE,                "No file type"},
    {1, ET_REL,                 "Relocatable file"},
    {2, ET_EXEC,                "Executable file"},
    {3, ET_DYN,                 "Shared object file"},
    {4, ET_CORE,                "Core file"},
    {5, ET_NUM,                 "Number of defined types"},
    {6, ET_LOOS,                "OS-specific range start"},
    {7, ET_HIOS,                "OS-specific range end"},
    {8, ET_LOPROC,              "Processor-specific range start"},
    {9, ET_HIPROC,              "Processor-specific range end"},
    {ELFDEF, ELFDEF,            "unhandled"}
};

elf_info_t _program[]={
    {0, PT_NULL,                "Unused"},
    {1, PT_LOAD,                "Loadable segment"},
    {2, PT_DYNAMIC,             "Dynamic linking" },
    {3, PT_INTERP,              "Program interpreter" },
    {4, PT_NOTE,                "Auxiliary information"},
    {5, PT_SHLIB,               "Reserved" },
    {6, PT_PHDR,                "Header table entry" },
    {7, PT_TLS,                 "Thread-local storage segment"},
    {8, PT_NUM,                 "Number of defined types"},
    {9, PT_LOOS,                "Start of OS-specific" },
    {10, PT_GNU_EH_FRAME,       "GCC .eh_frame_hdr segment"},
    {11, PT_GNU_STACK,          "Stack executability"},
    {11, PT_GNU_RELRO,          "Read-only after relocation"},
    {13, PT_PAX_FLAGS,          "PaX flag markings" },  /* PT 13 porra! */
    {ELFDEF, ELFDEF,            "unhandled"},
};

elf_info_t _osabi[]={
    {0, ELFOSABI_NONE,          "UNIX System V ABI"}, /**< == Alias */
    {1, ELFOSABI_HPUX,          "HP-UX"},
    {2, ELFOSABI_NETBSD,        "NetBSD"},
    {3, ELFOSABI_GNU,           "Object uses GNU ELF extensions"},
    {4, ELFOSABI_SOLARIS,       "Sun Solaris"},
    {5, ELFOSABI_AIX,           "IBM AIX"},
    {6, ELFOSABI_IRIX,          "SGI Irix"},
    {7, ELFOSABI_FREEBSD,       "FreeBSD"},
    {8, ELFOSABI_TRU64,         "Compaq TRU64 UNIX"},
    {9, ELFOSABI_MODESTO,       "Novell Modesto"},
    {10, ELFOSABI_OPENBSD,      "OpenBSD"},
    {11, ELFOSABI_ARM_AEABI,    "ARM EABI"},
    {12, ELFOSABI_ARM,          "ARM"},
    {13, ELFOSABI_STANDALONE,   "Standalone (embedded) application"},
    {ELFDEF, ELFDEF,             "unhandled"},
};

elf_info_t _endianness[]={
    {0, ELFDATANONE,            "Invalid data encoing"},
    {1, ELFDATA2LSB,            "2's complement, little endian"},
    {2, ELFDATA2MSB,            "2's complement, big endian"},
    {ELFDEF, ELFDEF,            "unhandled"},
};

elf_info_t _file_class[]={
    {0, ELFCLASSNONE,           "Invalid class"},
    {1, ELFCLASS32,             "32-Bit object"},
    {2, ELFCLASS64,             "64-Bit object"},
    {ELFDEF, ELFDEF,            "unhandled"},
};

