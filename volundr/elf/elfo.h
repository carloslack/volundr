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

#define ELF_SUCCESS 0
#define ELF_FAILURE -1
/*! @see types.c */
#define ELFDEF -1

// This valus is quite arbitraty
// but should suffice for most cases
#define MAXMAP      0xff
#define PH_TYPE_MAX  20
#define SH_TYPE_MAX  36


// WTF?! Combine the most and least significant bytes
// ex: 0x6ffffff5 -> 0x65 -> map[0x65]
// yeah yeah lazy as hell only because I don't want
// looping too much
#define LAZYMAP(x) (((x>>24)&0xf0)>>4)<<4|(x&0x0f)

// PT & SHT most & least significant bytes never repeat
// Always 32 bits in size
#define LAZY_PT_NULL            PT_NULL
#define LAZY_PT_LOAD            LAZYMAP(PT_LOAD)
#define LAZY_PT_DYNAMIC         LAZYMAP(PT_DYNAMIC)
#define LAZY_PT_INTERP          LAZYMAP(PT_INTERP)
#define LAZY_PT_NOTE            LAZYMAP(PT_NOTE)
#define LAZY_PT_SHLIB           LAZYMAP(PT_SHLIB)
#define LAZY_PT_PHDR            LAZYMAP(PT_PHDR)
#define LAZY_PT_TLS             LAZYMAP(PT_TLS)
#define LAZY_PT_NUM             LAZYMAP(PT_NUM)
#define LAZY_PT_LOOS            LAZYMAP(PT_LOOS)
#define LAZY_PT_GNU_EH_FRAME    LAZYMAP(PT_GNU_EH_FRAME)
#define LAZY_PT_GNU_STACK       LAZYMAP(PT_GNU_STACK)
#define LAZY_PT_GNU_RELRO       LAZYMAP(PT_GNU_RELRO )
#define LAZY_PT_LOSUNW          LAZYMAP(PT_LOSUNW)
#define LAZY_PT_SUNWBSS         LAZYMAP(PT_SUNWBSS)
#define LAZY_PT_SUNWSTACK       LAZYMAP(PT_SUNWSTACK)
#define LAZY_PT_HISUNW          LAZYMAP(PT_HISUNW)
#define LAZY_PT_HIOS            LAZYMAP(PT_HIOS)
#define LAZY_PT_LOPROC          LAZYMAP(PT_LOPROC)
#define LAZY_PT_HIPROC          LAZYMAP(PT_HIPROC)

#define LAZY_SHT_NULL           SHT_NULL
#define LAZY_SHT_PROGBITS       LAZYMAP(SHT_PROGBITS)
#define LAZY_SHT_SYMTAB         LAZYMAP(SHT_SYMTAB)
#define LAZY_SHT_STRTAB         LAZYMAP(SHT_STRTAB)
#define LAZY_SHT_RELA           LAZYMAP(SHT_RELA)
#define LAZY_SHT_HASH           LAZYMAP(SHT_HASH)
#define LAZY_SHT_DYNAMIC        LAZYMAP(SHT_DYNAMIC)
#define LAZY_SHT_NOTE           LAZYMAP(SHT_NOTE)
#define LAZY_SHT_NOBITS         LAZYMAP(SHT_NOBITS)
#define LAZY_SHT_REL            LAZYMAP(SHT_REL)
#define LAZY_SHT_SHLIB          LAZYMAP(SHT_SHLIB)
#define LAZY_SHT_DYNSYM         LAZYMAP(SHT_DYNSYM)
#define LAZY_SHT_INIT_ARRAY     LAZYMAP(SHT_INIT_ARRAY)
#define LAZY_SHT_FINI_ARRAY     LAZYMAP(SHT_FINI_ARRAY)
#define LAZY_SHT_PREINIT_ARRAY  LAZYMAP(SHT_PREINIT_ARRAY)
#define LAZY_SHT_GROUP          LAZYMAP(SHT_GROUP)
#define LAZY_SHT_SYMTAB_SHNDX   LAZYMAP(SHT_SYMTAB_SHNDX)
#define LAZY_SHT_NUM            LAZYMAP(SHT_NUM)
#define LAZY_SHT_LOOS           LAZYMAP(SHT_LOO)
#define LAZY_SHT_GNU_ATTRIBUTES LAZYMAP(SHT_GNU_ATTRIBUTE)
#define LAZY_SHT_GNU_HASH       LAZYMAP(SHT_GNU_HASH)
#define LAZY_SHT_GNU_LIBLIST    LAZYMAP(SHT_GNU_LIBLIST)
#define LAZY_SHT_CHECKSUM       LAZYMAP(SHT_CHECKSUM)
#define LAZY_SHT_LOSUNW         LAZYMAP(SHT_LOSUNW)
#define LAZY_SHT_SUNW_move      LAZYMAP(SHT_SUNW_move)
#define LAZY_SHT_SUNW_COMDAT    LAZYMAP(SHT_SUNW_COMDAT)
#define LAZY_SHT_SUNW_syminfo   LAZYMAP(SHT_SUNW_syminfo)
#define LAZY_SHT_GNU_verdef     LAZYMAP(SHT_GNU_verdef)
#define LAZY_SHT_GNU_verneed    LAZYMAP(SHT_GNU_verneed)
#define LAZY_SHT_GNU_versym     LAZYMAP(SHT_GNU_versym)
#define LAZY_SHT_HISUNW         LAZYMAP(SHT_HISUNW)
#define LAZY_SHT_HIOS           LAZYMAP(SHT_HIOS)
#define LAZY_SHT_LOPROC         LAZYMAP(SHT_LOPROC)
#define LAZY_SHT_HIPROC         LAZYMAP(SHT_HIPROC)
#define LAZY_SHT_LOUSER         LAZYMAP(SHT_LOUSER)
#define LAZY_SHT_HIUSER         LAZYMAP(SHT_HIUSER)

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
typedef Elf64_Xword     elf_xword_t;
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
        int map[PH_TYPE_MAX];
        int nr;
    } phmap[MAXMAP];

    /**
     * Stack allocated map of number of sections
     * and section indexes
     */
    struct {
        int map[SH_TYPE_MAX];
        int nr;
    } shmap[MAXMAP];

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
static elf_t *_elfo;

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
