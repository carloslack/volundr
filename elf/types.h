#ifndef _ELF_TYPES_H
#define _ELF_TYPES_H

#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#include "../common/common.h" // XXX use relative path (change Makefile)
#ifdef _TYPES_DECLARE
#define _TYPES_XTRN
#else
#define _TYPES_XTRN extern
#endif

#define HALF32 "0x%08x  "
#define nHALF32 "0x%08x\n"
#define INT "%d"
#define nINT "%d\n"
#define WORD32 HALF32
#define nWORD32 nHALF32
#define ADDR32 HALF32
#define nADDR32 nHALF32
#define OFF32 HALF32
#define nOFF32 nHALF32

#ifndef PT_PAX_FLAGS
#define PT_PAX_FLAGS 0x65041580 /* Indicates PaX flag markings */
#endif

#define false   0
#define true    1

enum {
    E_SHENTSIZE = 0,
    E_SHSTRNDX,
    E_PHOFF,
    E_PHENTSIZE,
    E_PHNUM,
};

/* Architecture independet macros */
#ifdef __ELF32__
#define ST_BIND(i)      (ELF32_ST_BIND(i))
#define ST_TYPE(i)      (ELF32_ST_TYPE(i))
#define ST_INFO(b,t)    (ELF32_ST_INFO(b,t))
#elif defined __ELF64__
#define ST_BIND(i)      (ELF64_ST_BIND(i))
#define ST_TYPE(i)      (ELF64_ST_TYPE(i))
#define ST_INFO(b,t)    (ELF64_ST_INFO(b,t))
#endif

#ifdef __ELF32__
typedef Elf32_Addr      elf_addr_t;
typedef Elf32_Half      elf_half_t;
typedef Elf32_Off       elf_off_t;
typedef Elf32_Sword     elf_sword_t;
typedef Elf32_Word      elf_word_t;
typedef Elf32_Ehdr      elf_ehdr_t;
typedef Elf32_Shdr      elf_shdr_t;
typedef Elf32_Phdr      elf_phdr_t;
typedef Elf32_Sym       elf_sym_t;
# ifdef __ELF_SYSV__
typedef Elf32_Dyn       elf_dyn_t;
# endif // __ELF_SYSV__
#elif defined __ELF64__
typedef Elf64_Addr      elf_addr_t;
typedef Elf64_Half      elf_half_t;
typedef Elf64_Off       elf_off_t;
typedef Elf64_Sword     elf_sword_t;
typedef Elf64_Word      elf_word_t;
typedef Elf64_Ehdr      elf_ehdr_t;
typedef Elf64_Phdr      elf_phdr_t;
typedef Elf64_Shdr      elf_shdr_t;
typedef Elf32_Sym       elf_sym_t;
# ifdef __ELF_SYSV__
typedef Elf64_Dyn       elf_dyn_t;
# endif // __ELF_SYSV__
#endif // __ELF32__ || __ELF64__


/*
 * Some other handy macros
 */

/* x = elf image */
#define PHNUM(x)        (x->ehdr->e_phnum)

/* x = elf image */
#define SHNUM(x)        (x->ehdr->e_shnum)

/* x = string table, y = index; look-up name in string table */
#define NAME2STR(x,y)   (x + y)

/* x = section header, y = .shstrtab content (sbyte*) */
#define SHNAME(x,y)     (NAME2STR(y,(x)->sh_name))

/* x = symbol, y = string table to look-up */
#define SYMNAME(x,y)    (NAME2STR(y,(x)->st_name))

/* x = section header */
#define SHSIZE(x)       ((x)->sh_size)

/* x = elf image */
#define SHSTRTAB(x)     (x->shdrs[elfo->ehdr->e_shstrndx])

/* cast to byte pointer */
#define ASBYTE(x)       ((char*)(x))

/* number of elements in section x, where x equals a pointer to elf_shdr_t */
#define SENTNUM(x)      (((x)->sh_size)/((x)->sh_entsize))

/* x = offset to be aligned for mmap pages */
#define ALIGNOFFSET(x) x & ~(sysconf(_SC_PAGE_SIZE) - 1)

/*
 * ELF general information abstraction
 */
typedef struct elf_info {
    i32 i;                       // TODO ??
    sbyte* name;                 // TODO ??
    sbyte* format;               // TODO ??
} elf_info_t;

/*
 * ELF Object ADT
 * NOTE: All arrays of types are null terminated.
 */
typedef struct _elf
{
    /* elf headers */
    elf_ehdr_t *ehdr;
    elf_phdr_t **phdrs;
    elf_shdr_t **shdrs;

    /* ELF raw image */
    unsigned char *data;
	
    /* info for mapping */
    void* mapaddr;
    off_t fsize;

} elf_t;

/* information references */
extern elf_info_t _section[];
extern elf_info_t _header[];
extern elf_info_t _program[];

#endif
