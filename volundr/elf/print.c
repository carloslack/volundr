/**
 *  MIT License
 *
 *  Copyright (c) 2021 Carlos Carvalho
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */
/**
 *    @file  print.c
 *   @brief  This module prints user messages.
 *
 * TODO: implementation of mutex to handle asynchronous calls
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

/**
 * @defgroup print Print
 * @ingroup elf

 * This module is responsible for all user output messages.
 */

#include "common.h"
#include "utils.h"
#include "log.h"
#include "elfo.h"
#include "print.h"
#include "parse.h"
#include "log.h"
#include "utils.h"

/**
 * @brief
 * This is a special case because we'll always want to print
 * Shndx, it can be either a number or a string, e.g.: Either ABS, UND or <#n>
 * in the same fashion as readelf.
 * @Note: Not thread-safe.
 */
static elf_info_t *get_index(int index_nr) {
    int index;
    ELF_DICT(&index, index, index_nr);
    if (index != ELFDEF)
        return &_index[index];

    /* Not there - return same value */
    static elf_info_t rv;
    static char buf[32];
    memset(buf, 0, sizeof(buf)); /* Let Valgrind know this is indeed initialized */
    snprintf(buf, sizeof(buf), "%d", index_nr);
    rv.i = 0;
    rv.name = buf;
    return &rv;
}

static void inline _elf_print_fname(const elf_t *elfo, FILE *fout) {
    static bool p = false;
    if (!p) {
        logf_it( fout, "-= Volundr reading from %s of %lu bytes =-\n",
                elfo->filename, elfo->elf_size);
        p = true;
    }
}

bool elf_print_header(const elf_t *elfo, FILE* fout) {
    if(fout == NULL || elfo == NULL) {
        log_debug("elf_print_header : invalid argument\n");
        return false;
    }

    const elf_ehdr_t *ehdr = elfo->ehdr;
    int index;

    _elf_print_fname(elfo, fout);
    logf_it(fout, "Elf Header:\n");
    logf_it(fout, "Magic:                               %x %c%c%c\n", ehdr->e_ident[EI_MAG0],
            ehdr->e_ident[EI_MAG1], ehdr->e_ident[EI_MAG2], ehdr->e_ident[EI_MAG3]);
    ELF_DICT(&index, file_class, ehdr->e_ident[EI_CLASS]);
    logf_it(fout, "Class:                               %s\n", _file_class[index].name);
    ELF_DICT(&index, endianness, ehdr->e_ident[EI_DATA]);
    logf_it(fout, "Data:                                %s\n", _endianness[index].name);
    ELF_DICT(&index, version, ehdr->e_version);
    logf_it(fout, "Version:                             %d %s\n", ehdr->e_ident[EI_VERSION],
            _version[index].name);
    ELF_DICT(&index, osabi, ehdr->e_ident[EI_OSABI]);
    logf_it(fout, "OS ABI:                              %s\n", _osabi[index].name);
    logf_it(fout, "ABI Version:                         %d\n", ehdr->e_ident[EI_ABIVERSION]);
    ELF_DICT(&index, header, ehdr->e_type);
    logf_it(fout, "Type:                                %s\n", _header[index].name);
    ELF_DICT(&index, machine, ehdr->e_machine);
    logf_it(fout, "Machine:                             %s\n", _machine[index].name);
    logf_it(fout, "Version:                             0x%x\n", ehdr->e_version);
    logf_it(fout, "Entry point address:                 %016llx\n", ehdr->e_entry);
    logf_it(fout, "Start of the program header:         %lld (bytes into file)\n", ehdr->e_phoff);
    logf_it(fout, "Start of the section header:         %lld (bytes into file)\n", ehdr->e_shoff);
    logf_it(fout, "Flags:                               %016x\n", ehdr->e_flags);
    logf_it(fout, "Size of this header:                 %d (bytes)\n", ehdr->e_ehsize);
    logf_it(fout, "Size of program headers:             %d (bytes)\n", ehdr->e_phentsize);
    logf_it(fout, "Number of program headers:           %d\n", ehdr->e_phnum);
    logf_it(fout, "Size of section header:              %d (bytes)\n", ehdr->e_shentsize);
    logf_it(fout, "Number of section headers:           %d\n", ehdr->e_shnum);
    logf_it(fout, "Section heander string table index:  %d\n", ehdr->e_shstrndx);

    return true;
}

bool elf_print_programs(const elf_t *elfo, FILE *fout) // XXX : either programs
                                                 // or phdrs...
{
    const elf_ehdr_t *ehdr = (const elf_ehdr_t *)elfo->ehdr;
    const elf_phdr_t **phdr = (const elf_phdr_t **)elfo->phdrs;

    if( !(ehdr && phdr)) {
        log_debug("elf_print_program : invalid argument");
        return false;
    }

    _elf_print_fname(elfo, fout);

    if (ehdr->e_phnum > 0)
        logf_it(fout, "Program Headers:\n%14s% 19s% 17s% 16s% 17s% 18s %15s %15s",
                "Type", "Offset", "Filesz", "Vaddr", "Paddr", "Namesz", "Align", "Name" );
    for(int i = 0; i < ehdr->e_phnum; ++i) {
        const elf_phdr_t *prog = (const elf_phdr_t *)phdr[i];
        int index;

        ELF_DICT(&index, program, prog->p_type);
        logf_it(fout, "\n%6d: %016x ", i, prog->p_type);
        logf_it(fout, "%016llx ", prog->p_offset);
        logf_it(fout, "%016llx ", prog->p_filesz);
        logf_it(fout, "%016llx ", prog->p_vaddr);
        logf_it(fout, "%016llx ", prog->p_paddr);
        logf_it(fout, "%016llx ", prog->p_memsz);
        logf_it(fout, "%016llx ", prog->p_align);
        logf_it(fout, "%s ", _program[index].name);
    }
    if (ehdr->e_phnum > 0)
        puts("");
    return true;
}

bool elf_print_sections(const elf_t *elfo, FILE *fout)
{
    const elf_ehdr_t *ehdr = (const elf_ehdr_t *)elfo->ehdr;
    const elf_shdr_t **shdrs = (const elf_shdr_t **)elfo->shdrs;

    if( !(ehdr && shdrs)) {
        log_debug("elf_print_sections : invalid argument");
        return false;
    }


    _elf_print_fname(elfo, fout);

    if (ehdr->e_shnum)
        logf_it(fout, "Section Headers:\n%14s% 18s% 16s% 19s% 15s% 17s %16s %17s %18s",
                "Type", "Flags", "Addr", "Offset", "Size", "Link", "Info", "Align", "Entsize", "Name" );
    for(int i=0; i < ehdr->e_shnum; i++) {
        char *shname = elf_get_section_header_name(elfo, shdrs[i]);
        logf_it(fout, "\n%6d: %016x ", i, shdrs[i]->sh_type);
        logf_it(fout, "%016llx ",shdrs[i]->sh_flags);
        logf_it(fout, "%016llx ", shdrs[i]->sh_addr);
        logf_it(fout, "%016llx ", shdrs[i]->sh_offset);
        logf_it(fout, "%016llx ", shdrs[i]->sh_size);
        logf_it(fout, "%016x ", shdrs[i]->sh_link);
        logf_it(fout, "%016x ", shdrs[i]->sh_info);
        logf_it(fout, "%016llx ", shdrs[i]->sh_addralign);
        logf_it(fout, "%016llx " ,shdrs[i]->sh_entsize);
        logf_it(fout, "%s ", shname);
    }
    if (ehdr->e_shnum)
        puts("");
    return true;
}

bool elf_print_symtab(const elf_t *elfo, FILE *fout, const elf_shdr_t *symtab, const elf_sym_t **syms)
{
    ASSERT_ARG_RET_FALSE(fout && symtab && syms);

    unsigned char *name_off = NULL;
    int sentnum = SENTNUM(symtab);
    char *symtab_name = elf_get_section_header_name(elfo, symtab);
    if(symtab_name == NULL) {
        log_error("Could not read symbol table's name");
        return false;
    }

    if (!strcmp(symtab_name, ".dynsym"))
        name_off = elf_get_symname_offset(elfo, ".dynstr");
    else if (!strcmp(symtab_name, ".symtab"))
        name_off = elf_get_symname_offset(elfo, ".strtab");

    _elf_print_fname(elfo, fout);
    // introduction
    logf_it(fout, "%s has %d symbols:\n"
            , symtab_name, sentnum);

    // output table header
    logf_it(fout, "%6s %10s %13s %7s %12s %4s %10s %s\n",
            "Num", "Val", "Size", "Type", "Bind", "Vis", "Shndx", "Name");

    for(int i=0; i < sentnum && syms[i]; i++) {
        int type,vis,bind;
        const elf_sym_t *sym = syms[i];
        elf_info_t *index = get_index(sym->st_shndx);
        ELF_DICT(&type, type, ST_TYPE(sym->st_info));
        ELF_DICT(&bind, bind, ST_BIND(sym->st_info));
        ELF_DICT(&vis, visibility, sym->st_other);

        logf_it(fout, "%6d: %016lx %6ld %10s %10s %6s %6s %6s\n"
                , i                         // Num
                , sym->st_value             // Val
                , sym->st_size              // Size
                , _type[type].name          // Type
                , _bind[bind].name          // Bind
                , _visibility[vis].name     // Visibility
                , index->name               // Shdnx (never NULL)
                , SYMNAME(sym, name_off)    // Name
               );
    }

    return true;
}

