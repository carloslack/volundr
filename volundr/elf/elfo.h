/*
 * Völundr 0.1 / volundr.c
 *
 * [Völundr] the ruler of the elves
 *
 * Author: hash
 *
 * Brief: Elf file reader / Elf file parasite( well, not just yet...)
 */

/**
 *    @file  elfo.h
 *   @brief  Main volundr data
 */

#ifndef _ELF_TYPES_H
#define _ELF_TYPES_H

#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef _TYPES_DECLARE
#define _TYPES_XTRN
#else
#define _TYPES_XTRN extern
#endif

#define USEMAP // TODO: Remove it later. It is just for fread/mmap transition

#ifndef PT_PAX_FLAGS
#define PT_PAX_FLAGS 0x65041580 /* Indicates PaX flag markings */
#endif

//#define false   0
//#define true    1
#define ELF_SUCCESS 0
#define ELF_FAILURE -1
/*! @see types.c */
#define ELFDEF -1

// This valus is quite arbitraty
// but should suffice for most cases
#define MAXMAP      100
#define P_TYPE_MAX  20

// Mod - decreases the value
#define MAP_IN(x) (x % MAXMAP)
// Reverse mod
#define MAP_OUT(x,y) ((x / MAXMAP) + y)

static const char *__volundr_ver__="1.0";

#define ST_BIND(i)      (ELF64_ST_BIND(i))
#define ST_TYPE(i)      (ELF64_ST_TYPE(i))
#define ST_INFO(b,t)    (ELF64_ST_INFO(b,t))
#define ELF_DICT(result, type, val)                 \
do {                                                \
    *result = ELFDEF;                               \
    for (int i = 0; _ ## type[i].idx != -1; ++i) {  \
        if (val == _ ## type[i].i) {                \
            *result = i;                            \
            break;                                  \
        }                                           \
    }                                               \
} while(0)


/*! @see /usr/include/elf.h (depending on your system) */
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

/**
 *  @brief ELF Object ADT - This is the main volundr structure
 *      It keeps the references for ELF headers,
 *      ELF image from the disk and useful variables.
 * @note All arrays of types are null terminated.
 *
 */
typedef struct elf
{
    /**
     * @name Symbols
     */
    /*@{*/
    /** Section header tables - linking view */
    elf_shdr_t  **sht_symtab;       /**< .dynsym (global symbols) reference to symbol table from the section header */
    elf_shdr_t  **sht_dynsym;       /**< .dynsym (global symbols) reference to dynamic symbol table from the section header */

    elf_sym_t   **syms_symtab;      /**< .symtab reference to symbol table from the section header */
    elf_sym_t   **syms_dynsym;      /**< .symtab reference to dynamic symbol table from the section header */
    /*@}*/
    /**
     * @name ELF Headers and Sections
     */
    /*@{*/
    elf_ehdr_t  *ehdr;                              /**< ELF file header */
    elf_phdr_t  **phdrs;                            /**< Program Segment Header - execution view,
                                                      helps translating sections (disk) into segments (memory)
                                                      and lies just after header at byte 64 */
    elf_shdr_t  **shdrs;                            /**< Section Header */
    elf_shdr_t  *dynstr;                            /**< .dynstr string table for dynamic linking (dynsym) */
    elf_shdr_t  *strtab;                            /**< .strtab string table for names associated with symbol table entries (symtab) */
    elf_shdr_t  *shstrtab;                          /**< Stores all Section Names */
    elf_shdr_t  **symtab;                           /**< Symbol Table */

    /**
     * Stack allocated map of number of programs
     * and program indexes
     */
    struct {
        int map[P_TYPE_MAX];
        int nr;
    } pmap[MAXMAP];

    /*@}*/
    /**
     * @name ELF raw file
     */
    /*@{*/
    void*       mapaddr;            /**< ELF raw image from the disk, loaded with asm_mmap() */
    off_t       elf_size;           /**< ELF total file size in disk. Also: (ehdr->e_shoff + ehdr->e_shnum) * sizeof(elf_shdr_t)) */
    char filename[PATH_MAX];
    /*@}*/
} elf_t;

/**
 *  @brief Helper structure for parsing and
 *  user output about ELF image
 *
 */
typedef struct elf_info {
    int32_t idx;                        /**< Index used in for/while loops */
    int32_t i;                          /**< ELF code */
    char *name;                    /**< Text representing the code */
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

/* x = section header, y = .shstrtab content (char*) */
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

/*! Translate raw data to user message @see types.c
 * @see types.c
 */
extern elf_info_t   _section[];
extern elf_info_t   _header[];
extern elf_info_t   _program[];
extern elf_info_t   _type[];
extern elf_info_t   _bind[];
extern elf_info_t   _visibility[];
extern elf_info_t   _index[];
extern elf_info_t   _machine[];
extern elf_info_t   _version[];
extern elf_info_t   _osabi[];
extern elf_info_t   _endianness[];
extern elf_info_t   _file_class[];

/**< opaque elfo */
elf_t *_elfo;

/*
 * helper that returns elfo object
 * */
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
