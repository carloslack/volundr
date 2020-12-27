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

#include "types.h"
#include "print.h"
#include "e_names.h"
#include "write.h"
#include "parse.h"
#include "common.h"
#include "log.h"
#include "utils.h"

#if 0
#define defout def_output_once;
static bool is_set;
static FILE *def_output_once;

bool elf_print_init(FILE *fp)
{
    ASSERT_ARG_RET_FALSE(fp);
    ASSERT_RET_FALSE(!is_set, "Already set.");

    def_output_once = fp;
    return true;
}
#endif

bool elf_print_elf(FILE *fout, const elf_t *elfo) {
    if(!elf_print_header(fout, elfo)) {
        log_error("failed reading headers");
        return false;
    } else {
        log_info("headers ok.");
    }

    if(!elf_print_programs(fout, elfo)) {
        log_error("failed reading programs");
        return false;
    } else {
        log_info("programs ok.");
    }

    if(!elf_print_sections(fout, elfo)) {
        log_error("failed reading sections");
        return false;
    } else {
        log_info("sections ok.");
    }

    //if(!elf_print_shstrtab(fout,elfo))
    //    return false;
    //log_info("strtab ok.");

    if(!elf_print_all_symtabs(fout,elfo)) {
        log_error("failed reading symtabs");
        return false;
    } else {
        log_info("symbols ok.");
    }
    return true;
}

bool elf_print_header(FILE* fout, const elf_t* elfo) {
    if(fout == NULL || elfo == NULL) {
        log_debug("elf_print_header : invalid argument\n");
        return false;
    }

    elf_ehdr_t *ehdr = elfo->ehdr;

    fprintf( fout, "**** ELF HEADER FILE : [%lu bytes] ****\n"
            , ehdr->e_shoff + ehdr->e_shnum * sizeof(elf_shdr_t));

    fprintf(fout, "[Elf Header]\n");

    fprintf(fout, "e_ident[EI_MAG0]: %02x\n", ehdr->e_ident[EI_MAG0]);
    fprintf(fout, "e_ident[EI_MAG1]: %c\n", ehdr->e_ident[EI_MAG1]);
    fprintf(fout, "e_ident[EI_MAG2]: %c\n", ehdr->e_ident[EI_MAG2]);
    fprintf(fout, "e_ident[EI_MAG3]: %c\n", ehdr->e_ident[EI_MAG3]);
    fprintf(fout, "e_ident[EI_CLASS]: %d ", ehdr->e_ident[EI_CLASS]);

    if (ehdr->e_ident[EI_CLASS] == ELFCLASSNONE)
        fprintf(fout, "Invalid class\n");
    else if (ehdr->e_ident[EI_CLASS] == ELFCLASS32)
        fprintf(fout, "32-Bit object\n");
    else if (ehdr->e_ident[EI_CLASS] == ELFCLASS64)
        fprintf(fout, "64-Bit object\n");
    else
        fprintf(fout, "Class unknown\n");

    fprintf(fout, "e_ident[EI_DATA]: %d ", ehdr->e_ident[EI_DATA]);

    if (ehdr->e_ident[EI_DATA] == ELFDATANONE)
        fprintf(fout,"Invalid data encoding\n");
    else if (ehdr->e_ident[EI_DATA] == ELFDATA2LSB)
        fprintf(fout, "LSB\n");
    else if (ehdr->e_ident[EI_DATA] == ELFDATA2MSB)
        fprintf(fout, "MSB\n");
    else
        fprintf(fout, "Unknown - %d\n", ehdr->e_ident[EI_DATA]);

    fprintf(fout, "e_ident[EI_VERSION]: %d\n", ehdr->e_ident[EI_VERSION]);
    fprintf(fout, "e_ident[EI_PAD]: %d\n", ehdr->e_ident[EI_PAD]);
    fprintf(fout, "e_type: 0x%x ", ehdr->e_type);
    fprintf(fout, " [%s]\n", e_header_type(ehdr->e_type));
    fprintf(fout, "e_machine: 0x%x", ehdr->e_machine);
    fprintf(fout, " [%s]\n", e_header_machine(ehdr->e_machine));
    fprintf(fout, "e_version: 0x%x\n", ehdr->e_version);
    fprintf(fout, "e_entry: %s 0x%lx\n", e_header_version(ehdr->e_version), ehdr->e_entry);
    fprintf(fout, "e_phoff: 0x%lx\n", ehdr->e_phoff);
    fprintf(fout, "e_shoff: 0x%lx\n", ehdr->e_shoff);
    fprintf(fout, "e_flags: 0x%x\n", ehdr->e_flags);
    fprintf(fout, "e_ehsize: %d\n", ehdr->e_ehsize);
    fprintf(fout, "e_phentsize: %d\n", ehdr->e_phentsize);
    fprintf(fout, "e_phnum: %d\n", ehdr->e_phnum);
    fprintf(fout, "e_shentsize: %d\n", ehdr->e_shentsize);
    fprintf(fout, "e_shnum: %d\n", ehdr->e_shnum);
    fprintf(fout, "e_shstrndx: 0x%x\n", ehdr->e_shstrndx);

    return true;
}

bool elf_print_programs(FILE* fout, const elf_t *elfo) // XXX : either programs
                                                 // or phdrs...
{
    if( !(elfo && elfo->ehdr && elfo->phdrs)) {
        log_debug("elf_print_program : invalid argument");
        return false;
    }

    u16 y;
    elf_ehdr_t *ehdr = elfo->ehdr;
    elf_phdr_t **phdrs = elfo->phdrs;

    //TODO :
    //       elf = (elf_phdr_t*) &filein[h + ehdr->e_phoff];
    //       h+=ehdr->e_phentsize;

    int phnum = ehdr->e_phnum;
    for(y = 0; y < phnum; y++) {
        fprintf(fout, "\n[Elf Program %d]\n", y);
        fprintf(fout, "p_type 0x%x [%s]\n", phdrs[y]->p_type, e_program_type(phdrs[y]->p_type));
        fprintf(fout, "p_offset 0x%lx\n", phdrs[y]->p_offset);
        fprintf(fout, "p_vaddr 0x%lx\n", phdrs[y]->p_vaddr);
        fprintf(fout, "p_paddr 0x%lx\n", phdrs[y]->p_paddr);
        fprintf(fout, "p_filesz 0x%lx\n", phdrs[y]->p_filesz);
        fprintf(fout, "p_namesz 0x%lx\n", phdrs[y]->p_memsz);
        fprintf(fout, "p_align 0x%lx\n", phdrs[y]->p_align);
    }
    return true;
}

elf_info_t *get_type(int type_nr) {
    static elf_info_t type;
    memset(&type, 0, sizeof(elf_info_t));
    for (int i = 0; _type[i].i != -1; ++i) {
        if (type_nr == _type[i].i) {
            type = _type[i];
            break;
        }
    }
    return &type;
}

elf_info_t *get_info(int info_nr) {
    static elf_info_t info;
    memset(&info, 0, sizeof(elf_info_t));
    for (int i = 0; _info[i].i != -1; ++i) {
        if (info_nr == _info[i].i) {
            info = _info[i];
            break;
        }
    }
    return &info;
}

elf_info_t *get_visibility(int visibility_nr) {
    static elf_info_t visibility;
    memset(&visibility, 0, sizeof(elf_info_t));
    for (int i = 0; _visibility[i].i != -1; ++i) {
        if (visibility_nr == _visibility[i].i) {
            visibility = _visibility[i];
            break;
        }
    }
    return &visibility;
}

bool elf_print_sections(FILE *fout, const elf_t* elfo)
{
    //TODO: verify which and why specific data should be kept
    //      so this parser could feed the parasite

    if( !(elfo&&elfo->ehdr&&elfo->shdrs)) {
        log_debug("elf_print_sections : invalid argument");
        return false;
    }

    u16 i;
    elf_shdr_t **shdrs = elfo->shdrs;
    i32 n_secs = elfo->ehdr->e_shnum;

    for(i=0; i<n_secs; i++) {
        sbyte *shname = elf_parse_shname(elfo, shdrs[i]);
        if(!strlen(shname))
            shname = "NULL";
        fprintf(fout, "\n[Elf Section %s]\n", shname);
        fprintf(fout, "sh_name: 0x%x\n", shdrs[i]->sh_name);
        fprintf(fout, "sh_type: 0x%x\n", shdrs[i]->sh_type);
        fprintf(fout, "sh_flags: 0x%lx\n",shdrs[i]->sh_flags);
        fprintf(fout, "sh_addr: 0x%lx\n", shdrs[i]->sh_addr);
        fprintf(fout, "sh_offset: 0x%lx\n", shdrs[i]->sh_offset);
        fprintf(fout, "sh_size: 0x%lx\n", shdrs[i]->sh_size);
        fprintf(fout, "sh_link: 0x%x\n", shdrs[i]->sh_link);
        fprintf(fout, "sh_info: 0x%x\n", shdrs[i]->sh_info);
        fprintf(fout, "sh_addralign: 0x%lx\n", shdrs[i]->sh_addralign);
        fprintf(fout, "sh_entsize: 0x%lu\n" ,shdrs[i]->sh_entsize);
    }
    return true;
}

/**
 * Prints Unix System V Section .shtrtab.
 *
 * CONTRACT:
 * this function depends on calling elf_parse_shstrtab
 * at least once, so shstrtab meta-data can be fetched.
 */
bool elf_print_shstrtab(FILE *fout, const elf_t *elfo)
{
    return false;
}


bool elf_print_strtab(FILE *fout, const elf_t *elfo)
{
    // XXX FIXME
#if 0
    char * strings = elfo->meta.strtab;
    if(!elfo||!elfo->shdrs||!strings) {
        log_debug("invalid argument");
        return false;
    }
    size_t size = (size_t) _STRTAB(elfo)->sh_size;
    char *p = strings;

    fprintf(fout, "\n[Elf String Table]\n");
    while(p && (p-strings)<size) {
        p = strchr(p, '\0');
        if(p) {
            p++;
            if(*p != '\0') {
                fprintf(fout, "strtab[%d] = %s\n",
                        (unsigned int)(p-strings),p);
            }
        } else { p++; }
    }
    fprintf(fout, "\n");
#endif
    return true;
}

/**
 *
 */
bool elf_print_symtab(FILE *fout, const elf_t* elfo, const elf_shdr_t *symtab)
{
    ASSERT_ARG_RET_FALSE(fout && elfo && symtab);

    elf_sym_t **syms = elf_parse_all_syms(elfo, symtab);
    if(syms == NULL) {
        log_error("Could not read symbols from provided symbol table");
        return false;
    }

    sbyte *symtab_name = elf_parse_shname(elfo, symtab);
    if(symtab_name == NULL) {
        symtab_name = "NULL";
        log_warning("Could not read symbol table's name");
    }

    // introduction
    fprintf(fout, "[SYMTAB] Symtab %s has %lu symbols:\n\n"
            , symtab_name, SENTNUM(symtab));

    // output table header
    // Try to format it nicely
    fprintf(fout, "%6s %10s %13s %7s %12s %4s %10s %s\n",
            "Num", "Val", "Size", "Type", "Bind", "Vis", "Shndx", "Name");

    sbyte *dynstr = elf_parse_dynstr(elfo);
    if(dynstr == NULL) {
        log_error("Could not read dynamic string table");
        return false;
    }

    int i;
    for(i=0; i<SENTNUM(symtab) && syms[i] != NULL; i++) {
        elf_sym_t *sym = syms[i];
        fprintf( fout
                , "%6d: %016x %6ld %10s %10s %6s %6d %6s\n"
                , i                        // Num
                , sym->st_value            // Val
                , sym->st_size             // Size
                , get_type(ST_TYPE(sym->st_info))->name            // Info
                , get_info(ST_BIND(sym->st_info))->name// ( Bind
                , get_visibility(sym->st_other)->name            // Other
                , sym->st_shndx            // Shdnx
                , SYMNAME(sym, dynstr)     // Name  FIXME buggy on unstripped symbols
               );
    }
    return true;
}


/**
 *
 */
bool elf_print_all_symtabs(FILE *fout, const elf_t *elfo)
{
    ASSERT_ARG_RET_FALSE(fout && elfo);
    ASSERT_CON_RET_FALSE(elfo->data);

    elf_shdr_t **sym_tables = elf_parse_all_symtabs(elfo);
    if(sym_tables == NULL)
        return false;

    // print all symbol tables
    int i;
    for(i=0; sym_tables[i] != NULL; i++) {
        elf_print_symtab(fout, elfo, sym_tables[i]);
    }
    return true;
}
