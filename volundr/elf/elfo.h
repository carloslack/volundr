/*
 * Völundr 0.1 / volundr.c
 *
 * [Völundr] the ruler of the elves
 *
 * Author: hash
 *
 * Brief: Elf file reader / Elf file parasite( well, not just yet...)
 */

#ifndef _ELF_TYPES_H
#define _ELF_TYPES_H

#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#ifdef _TYPES_DECLARE
#define _TYPES_XTRN
#else
#define _TYPES_XTRN extern
#endif

#if defined( PARANOID_CHECK ) && defined( LOOSE_CHECK )
# error "Conflicting pre-processor definition: PARANOID_CHECK, LOOSE_CHECK"
#endif

#define USEMAP // TODO: Remove it later. It is just for fread/mmap transition

#ifndef PT_PAX_FLAGS
#define PT_PAX_FLAGS 0x65041580 /* Indicates PaX flag markings */
#endif

#define false   0
#define true    1
#define ELF_SUCCESS 0
#define ELF_FAILURE -1
#define ELFDEF -1  /* See types.c */

enum {
    E_SHENTSIZE = 0,
    E_SHSTRNDX,
    E_PHOFF,
    E_PHENTSIZE,
    E_PHNUM,
};

static const char *__volundr_ver__="0.1";

/* Architecture independet macros */
#define ST_BIND(i)      (ELF64_ST_BIND(i))
#define ST_TYPE(i)      (ELF64_ST_TYPE(i))
#define ST_INFO(b,t)    (ELF64_ST_INFO(b,t))

typedef Elf64_Addr      elf_addr_t;
typedef Elf64_Half      elf_half_t;
typedef Elf64_Off       elf_off_t;
typedef Elf64_Sword     elf_sword_t;
typedef Elf64_Word      elf_word_t;
typedef Elf64_Ehdr      elf_ehdr_t;
typedef Elf64_Phdr      elf_phdr_t;
typedef Elf64_Shdr      elf_shdr_t;
typedef Elf64_Sym       elf_sym_t;
# ifdef __ELF_SYSV__
typedef Elf64_Dyn       elf_dyn_t;
# endif // __ELF_SYSV__

/*
 * ELF Object ADT
 * NOTE: All arrays of types are null terminated.
 */
typedef struct elf
{
    /* elf headers */
    elf_sym_t **syms;
    elf_ehdr_t *ehdr;
    elf_phdr_t **phdrs;
    elf_shdr_t **shdrs;
    elf_shdr_t *dynstr;
    elf_shdr_t *strtab;
    elf_shdr_t *shstrtab;
    /* ELF raw image */
    unsigned char *data;
    /* info for mapping */
    void* mapaddr;
    off_t fsize;
} elf_t;

/*
 * ELF general information abstraction
 */
typedef struct elf_info {
    i32 idx;
    i32 i;                       // TODO ??
    sbyte* name;                 // TODO ??
} elf_info_t;


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

/* Translate raw data to user message */
extern elf_info_t   _section[];
extern elf_info_t   _header[];
extern elf_info_t   _program[];
extern elf_info_t   _type[];
extern elf_info_t   _bind[];
extern elf_info_t   _visibility[];
extern elf_info_t   _index[];
extern elf_info_t   _machine[];
extern elf_info_t   _version[];

elf_t *_elfo;
static inline elf_t *elf_set_my_elfo(elf_t *elfo) {
    ASSERT_ARG_RET_NULL(elfo);
    static bool is_set;
    if (!is_set && elfo) {
        _elfo = elfo;
        is_set = true;
    }
    return _elfo;
}

static inline elf_t *elf_get_my_elfo(void) {

    ASSERT_CON_RET_NULL(_elfo);
    return _elfo;
}

static const char __attribute__((unused))*elf_get_version(void)
{
    return __volundr_ver__;
}
#endif
