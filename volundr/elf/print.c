/**
 *    @file  print.c
 *   @brief  This module prints user messages.
 *
 * TODO: implementation of mutex to handle asynchronous calls
 */

#include <stdio.h>
#include <string.h>

/**
 * @defgroup print Print
 * @ingroup elf

 * This module is responsible for all user output messages.
 */

#include "print.h"
#include "elfo.h"
#include "write.h"
#include "parse.h"
#include "common.h"
#include "log.h"
#include "utils.h"

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

static elf_info_t *get_index(int index_nr) {
    int index;
    ELF_DICT(&index, index, index_nr);
    if (index != ELFDEF)
        return &_index[index];

    /* Not there - return same value */
    static elf_info_t rv;
    char buf[32] = {0};
    snprintf(buf, sizeof(buf), "%d", index_nr);
    rv.i = 0;
    rv.name = buf;
    return &rv;
}

bool elf_print_header(FILE* fout) {
    if(fout == NULL) {
        log_debug("elf_print_header : invalid argument\n");
        return false;
    }

    const elf_ehdr_t *ehdr = elf_get_my_elfo()->ehdr;
    int index;

    logf_it( fout, "**** ELF HEADER FILE : [%lu bytes] ****\n"
            , ehdr->e_shoff + ehdr->e_shnum * sizeof(elf_shdr_t));

    logf_it(fout, "[Elf Header]\n");

    logf_it(fout, "e_ident[EI_MAG0]: %02x\n", ehdr->e_ident[EI_MAG0]);
    logf_it(fout, "e_ident[EI_MAG1]: %c\n", ehdr->e_ident[EI_MAG1]);
    logf_it(fout, "e_ident[EI_MAG2]: %c\n", ehdr->e_ident[EI_MAG2]);
    logf_it(fout, "e_ident[EI_MAG3]: %c\n", ehdr->e_ident[EI_MAG3]);
    logf_it(fout, "e_ident[EI_CLASS]: %d ", ehdr->e_ident[EI_CLASS]);

    if (ehdr->e_ident[EI_CLASS] == ELFCLASSNONE)
        logf_it(fout, "Invalid class\n");
    else if (ehdr->e_ident[EI_CLASS] == ELFCLASS32)
        logf_it(fout, "32-Bit object\n");
    else if (ehdr->e_ident[EI_CLASS] == ELFCLASS64)
        logf_it(fout, "64-Bit object\n");
    else
        logf_it(fout, "Class unknown\n");

    logf_it(fout, "e_ident[EI_DATA]: %d ", ehdr->e_ident[EI_DATA]);

    if (ehdr->e_ident[EI_DATA] == ELFDATANONE)
        logf_it(fout,"Invalid data encoding\n");
    else if (ehdr->e_ident[EI_DATA] == ELFDATA2LSB)
        logf_it(fout, "LSB\n");
    else if (ehdr->e_ident[EI_DATA] == ELFDATA2MSB)
        logf_it(fout, "MSB\n");
    else
        logf_it(fout, "Unknown - %d\n", ehdr->e_ident[EI_DATA]);

    logf_it(fout, "e_ident[EI_VERSION]: %d\n", ehdr->e_ident[EI_VERSION]);
    logf_it(fout, "e_ident[EI_PAD]: %d\n", ehdr->e_ident[EI_PAD]);

    ELF_DICT(&index, header, ehdr->e_type);
    logf_it(fout, "e_type: 0x%x %s\n", ehdr->e_type, _header[index].name);

    ELF_DICT(&index, machine, ehdr->e_machine);
    logf_it(fout, "e_machine: 0x%x %s\n", ehdr->e_machine, _machine[index].name);

    ELF_DICT(&index, version, ehdr->e_version);
    logf_it(fout, "e_version: 0x%x %s\n", ehdr->e_version, _version[index].name);

    logf_it(fout, "e_entry: 0x%lx\n", ehdr->e_entry);
    logf_it(fout, "e_phoff: 0x%lx\n", ehdr->e_phoff);
    logf_it(fout, "e_shoff: 0x%lx\n", ehdr->e_shoff);
    logf_it(fout, "e_flags: 0x%x\n", ehdr->e_flags);
    logf_it(fout, "e_ehsize: %d\n", ehdr->e_ehsize);
    logf_it(fout, "e_phentsize: %d\n", ehdr->e_phentsize);
    logf_it(fout, "e_phnum: %d\n", ehdr->e_phnum);
    logf_it(fout, "e_shentsize: %d\n", ehdr->e_shentsize);
    logf_it(fout, "e_shnum: %d\n", ehdr->e_shnum);
    logf_it(fout, "e_shstrndx: 0x%x\n", ehdr->e_shstrndx);

    return true;
}

bool elf_print_programs(FILE* fout) // XXX : either programs
                                                 // or phdrs...
{
    const elf_t *elfo = elf_get_my_elfo();
    const elf_ehdr_t *ehdr = (const elf_ehdr_t *)elfo->ehdr;
    const elf_phdr_t **phdr = (const elf_phdr_t **)elfo->phdrs;

    if( !(ehdr && phdr)) {
        log_debug("elf_print_program : invalid argument");
        return false;
    }

    //TODO :
    //       elf = (elf_phdr_t*) &filein[h + ehdr->e_phoff];
    //       h+=ehdr->e_phentsize;

    for(int i = 0; i < ehdr->e_phnum; ++i) {
        const elf_phdr_t *prog = (const elf_phdr_t *)phdr[i];
        int index;

        ELF_DICT(&index, program, prog->p_type);
        logf_it(fout, "\n[Elf Program %d]\n", i);
        logf_it(fout, "p_type 0x%x %s\n", prog->p_type, _program[index].name);
        logf_it(fout, "p_offset 0x%lx\n", prog->p_offset);
        logf_it(fout, "p_vaddr 0x%lx\n", prog->p_vaddr);
        logf_it(fout, "p_paddr 0x%lx\n", prog->p_paddr);
        logf_it(fout, "p_filesz 0x%lx\n", prog->p_filesz);
        logf_it(fout, "p_namesz 0x%lx\n", prog->p_memsz);
        logf_it(fout, "p_align 0x%lx\n", prog->p_align);
    }
    return true;
}

bool elf_print_sections(FILE *fout)
{
    //TODO: verify which and why specific data should be kept
    //      so this parser could feed the parasite

    elf_t *elfo = elf_get_my_elfo();
    const elf_ehdr_t *ehdr = (const elf_ehdr_t *)elfo->ehdr;
    const elf_shdr_t **shdrs = (const elf_shdr_t **)elfo->shdrs;

    if( !(ehdr && shdrs)) {
        log_debug("elf_print_sections : invalid argument");
        return false;
    }

    u16 i;
    i32 n_secs = ehdr->e_shnum;

    for(i=0; i<n_secs; i++) {
        sbyte *shname = elf_parse_shname(shdrs[i]);
        if(!shname || !strlen(shname))
            shname = "NULL";
        logf_it(fout, "\n[Elf Section %s]\n", shname);
        logf_it(fout, "sh_name: 0x%x\n", shdrs[i]->sh_name);
        logf_it(fout, "sh_type: %d\n", shdrs[i]->sh_type);
        logf_it(fout, "sh_flags: 0x%lx\n",shdrs[i]->sh_flags);
        logf_it(fout, "sh_addr: 0x%lx\n", shdrs[i]->sh_addr);
        logf_it(fout, "sh_offset: 0x%lx\n", shdrs[i]->sh_offset);
        logf_it(fout, "sh_size: 0x%lx\n", shdrs[i]->sh_size);
        logf_it(fout, "sh_link: 0x%x\n", shdrs[i]->sh_link);
        logf_it(fout, "sh_info: 0x%x\n", shdrs[i]->sh_info);
        logf_it(fout, "sh_addralign: 0x%lx\n", shdrs[i]->sh_addralign);
        logf_it(fout, "sh_entsize: 0x%lu\n" ,shdrs[i]->sh_entsize);
    }
    return true;
}

bool elf_print_symtab(FILE *fout, const elf_shdr_t *symtab)
{
    ASSERT_ARG_RET_FALSE(fout && symtab);

    elf_sym_t **syms = NULL;
    syms = elf_parse_all_syms(symtab);
    if(syms == NULL) {
        log_error("Could not read symbols from provided symbol table");
        return false;
    }

    sbyte *symtab_name = elf_parse_shname(symtab);
    if(symtab_name == NULL) {
        symtab_name = "NULL";
        log_warning("Could not read symbol table's name");
    }

    sbyte *dynstr = elf_parse_section_by_name(".dynstr");
    if(dynstr == NULL)
        log_debug("Could not read dynamic string table");

    sbyte *strtab = elf_parse_section_by_name(".strtab");
    if(strtab == NULL)
        log_debug("Could not read string table");

    // introduction
    logf_it(fout, "[SYMTAB] Symtab %s has %lu symbols:\n\n"
            , symtab_name, SENTNUM(symtab));

    // output table header
    logf_it(fout, "%6s %10s %13s %7s %12s %4s %10s %s\n",
            "Num", "Val", "Size", "Type", "Bind", "Vis", "Shndx", "Name");

    for(int i=0; i<SENTNUM(symtab) && syms[i]; i++) {
        int type,vis,bind;
        elf_sym_t *sym = syms[i];
        elf_info_t *index = get_index(sym->st_shndx);
        ELF_DICT(&type, type, ST_TYPE(sym->st_info));
        ELF_DICT(&bind, bind, ST_BIND(sym->st_info));
        ELF_DICT(&vis, visibility, sym->st_other);

        char *sname = "";
        if (symtab->sh_type == SHT_DYNSYM)
            sname = SYMNAME(sym, dynstr);
        else if (symtab->sh_type == SHT_SYMTAB)
            sname = SYMNAME(sym, strtab);

        logf_it(fout, "%6d: %016lx %6ld %10s %10s %6s %6s %6s\n"
                , i                         // Num
                , sym->st_value             // Val
                , sym->st_size              // Size
                , _type[type].name          // Type
                , _bind[bind].name          // Bind
                , _visibility[vis].name     // Visibility
                , index->name               // Shdnx (never NULL)
                , sname                     // Name
               );
    }
    return true;
}

