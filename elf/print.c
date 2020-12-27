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
#include "../common/common.h"
#include "../common/log.h"
#include "../common/utils.h"

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

static void print_helper(bool *value, bool what) {
    if(what == true) {
        *value = true;
    }else{
        *value = false;
    }
}

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
    //TODO: remove the use of structures for printing to make it readable
    //
    bool ctrl = false;
    if(fout == NULL || elfo == NULL) {
        log_debug("elf_print_header : invalid argument\n");
        return false;
    }

    elf_ehdr_t *ehdr = elfo->ehdr;

    fprintf( fout, "**** ELF HEADER FILE : [%lu bytes] ****\n"
            , ehdr->e_shoff + ehdr->e_shnum * sizeof(elf_shdr_t));

    fprintf(fout, "[Elf Header]\n");

    fprintf(fout, "%s", _header[0].name);
    fprintf(fout, "e_ident[EI_MAG0]: %02x\n", ehdr->e_ident[EI_MAG0]);
    fprintf(fout, "e_ident[EI_MAG1]: %c\n", ehdr->e_ident[EI_MAG1]);
    fprintf(fout, "e_ident[EI_MAG2]: %c\n", ehdr->e_ident[EI_MAG2]);
    fprintf(fout, "e_ident[EI_MAG3]: %c\n", ehdr->e_ident[EI_MAG3]);
    fprintf(fout, "e_ident[EI_CLASS]: %d ", ehdr->e_ident[EI_CLASS]);

    ehdr->e_ident[EI_CLASS] == ELFCLASSNONE ? fprintf(fout, "Invalid class\n")
        : print_helper(&ctrl, true);

    ehdr->e_ident[EI_CLASS] == ELFCLASS32 ? fprintf(fout, "32-Bit object\n")
        : print_helper(&ctrl, true);

    ehdr->e_ident[EI_CLASS] == ELFCLASS64 ? fprintf(fout, "64-Bit object\n")
        : print_helper(&ctrl, true);

    !ctrl ? fprintf(fout, "Unknown\n") : print_helper(&ctrl, false);

    fprintf(fout, "e_ident[EI_DATA]: %d ", ehdr->e_ident[EI_DATA]);

    ( ehdr->e_ident[EI_DATA] == ELFDATANONE )
        ? fprintf(fout,"Invalid data encoding\n")
        : print_helper(&ctrl, true);

    ehdr->e_ident[EI_DATA] == ELFDATA2LSB ? fprintf(fout, "LSB\n")
        : print_helper(&ctrl, true);

    ehdr->e_ident[EI_DATA] == ELFDATA2MSB ? fprintf(fout, "MSB\n")
        : print_helper(&ctrl, true);
    !ctrl ? fprintf(fout, "Unknown\n") : 0;
    fprintf(fout, "e_ident[EI_VERSION]: %d\n", ehdr->e_ident[EI_VERSION]);
    fprintf(fout, "e_ident[EI_PAD]: %d\n", ehdr->e_ident[EI_PAD]);
    fprintf(fout, "%s", _header[1].name);
    fprintf(fout, _header[1].format,ehdr->e_type);
    fprintf(fout, "\n%s", _header[2].name);
    fprintf(fout, "%s ", e_header_type(ehdr->e_type));
    fprintf(fout, _header[2].format, ehdr->e_machine);
    fprintf(fout, "\n%s", _header[3].name);
    fprintf(fout, "%s ", e_header_machine(ehdr->e_machine));
    fprintf(fout, _header[3].format,ehdr->e_version);
    fprintf(fout, "\n%s", _header[4].name);
    fprintf(fout, "%s ", e_header_version(ehdr->e_version));
    fprintf(fout, _header[4].format,ehdr->e_entry);
    fprintf(fout, "%s", _header[5].name);
    fprintf(fout, _header[5].format,ehdr->e_phoff);
    fprintf(fout, "%s", _header[6].name);
    fprintf(fout, _header[6].format,ehdr->e_shoff);
    fprintf(fout, "%s", _header[7].name);
    fprintf(fout,  _header[7].format,ehdr->e_flags);
    fprintf(fout, "%s", _header[8].name);
    fprintf(fout, _header[8].format,ehdr->e_ehsize);
    fprintf(fout, "%s", _header[9].name);
    fprintf(fout, _header[9].format,ehdr->e_phentsize);
    fprintf(fout, "%s", _header[10].name);
    fprintf(fout, _header[10].format,ehdr->e_phnum);
    fprintf(fout, "%s", _header[11].name);
    fprintf(fout, _header[11].format,ehdr->e_shentsize);
    fprintf(fout, "%s", _header[12].name);
    fprintf(fout, _header[12].format,ehdr->e_shnum);
    fprintf(fout, "%s", _header[13].name);
    fprintf(fout, _header[13].format,ehdr->e_shstrndx);

    return true;
}

bool elf_print_programs(FILE* fout, const elf_t *elfo) // XXX : either programs
                                                 // or phdrs...
{
    if( !(elfo && elfo->ehdr && elfo->phdrs)) {
        log_debug("elf_print_program : invalid argument");
        return false;
    }

    u16 y,j;
    elf_ehdr_t *ehdr = elfo->ehdr;
    elf_phdr_t **phdrs = elfo->phdrs;

    //TODO :
    //       elf = (elf_phdr_t*) &filein[h + ehdr->e_phoff];
    //       h+=ehdr->e_phentsize;

    int phnum = ehdr->e_phnum;
    for(y = 0, j = 0; y < phnum; y++, j=0) {

        fprintf(fout, "\n[Elf Program %d]\n", y);

        fprintf(fout, "%s", _program[j].name);
        fprintf(fout, _program[j++].format, phdrs[y]->p_type);
        fprintf(fout, "%s\n", e_program_type(phdrs[y]->p_type));

        fprintf(fout, "%s", _program[j].name);
        fprintf(fout, _program[j++].format, phdrs[y]->p_offset);

        fprintf(fout, "%s", _program[j].name);
        fprintf(fout, _program[j++].format, phdrs[y]->p_vaddr);

        fprintf(fout, "%s", _program[j].name);
        fprintf(fout, _program[j++].format, phdrs[y]->p_paddr);

        fprintf(fout, "%s", _program[j].name);
        fprintf(fout, _program[j++].format, phdrs[y]->p_filesz);

        fprintf(fout, "%s", _program[j].name);
        fprintf(fout, _program[j++].format, phdrs[y]->p_memsz);

        fprintf(fout, "%s", _program[j].name);
        fprintf(fout, _program[j++].format, phdrs[y]->p_align);
    }
    return true;
}

elf_info_t *get_type(int type_nr) {
    static elf_info_t type;
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

    u16 i,j;
    elf_shdr_t **shdrs = elfo->shdrs;
    i32 n_secs = elfo->ehdr->e_shnum;

    for(i=0; i<n_secs; i++) {
        sbyte *shname = elf_parse_shname(elfo, shdrs[i]);
        if(strlen(shname) == 0)
            shname = "<NULL>";
        fprintf(fout, "\n[Elf Section %s]\n", shname);
        for(j=0; _section[j].name != NULL; j++) {
            fprintf(fout, "%s", _section[j].name);

            // XXX TODO : remove switch?
            switch(_section[j].i) {
                case 0: fprintf(fout, _section[j].format,shdrs[i]->sh_name);
                        break;
                case 1: fprintf(fout, _section[j].format,shdrs[i]->sh_type);
                        break;
                case 2: fprintf(fout, _section[j].format,shdrs[i]->sh_flags);
                        break;
                case 3: fprintf(fout, _section[j].format,shdrs[i]->sh_addr);
                        break;
                case 4: fprintf(fout, _section[j].format,shdrs[i]->sh_offset);
                        break;
                case 5: fprintf(fout, _section[j].format,shdrs[i]->sh_size);
                        break;
                case 6: fprintf(fout, _section[j].format,shdrs[i]->sh_link);
                        break;
                case 7: fprintf(fout, _section[j].format,shdrs[i]->sh_info);
                        break;
                case 8: fprintf(fout,_section[j].format,shdrs[i]->sh_addralign);
                        break;
                case 9: fprintf(fout, _section[j].format,shdrs[i]->sh_entsize);
                        break;
                default:
                        break;
            }
        }
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
    fprintf(fout, "%6s %10s %6s %6s %6s %6s %6s %6s \t%6s\n",
            "Num", "Val", "Size", "Info", "(b,", "t)", "Other", "Shndx", "Name");

    sbyte *dynstr = elf_parse_dynstr(elfo);
    if(dynstr == NULL) {
        log_error("Could not read dynamic string table");
        return false;
    }

    int i;
    for(i=0; i<SENTNUM(symtab) && syms[i] != NULL; i++) {
        elf_sym_t *sym = syms[i];
        fprintf( fout
                , "     %d: %016x %6ld %s %s %s %d %s\n"
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
