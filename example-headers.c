#include <string.h>
#include <errno.h>
#include <assert.h>

#include "common.h"
#include "utils.h"
#include "log.h"
#include "elfo.h"
#include "parse.h"
#include "map.h"
#include "asm.h"
#include "print.h"
#include "validate.h"
#include "destroy.h"

extern elf_info_t   _program[];

int main(int argc, char **argv)
{
    if (argc < 2) {
        log_info("Use %s <elf file>\n", argv[0]);
        asm_exit(0);
    }
    const char *file = argv[1];
    FILE *fp = file_open_ro(file);

    if(!elf_validate_filetype(fp)) {
        log_error("Not a valid ELF file");
        asm_exit(1);
    }

    elf_t *elfo = elf_parse_file(file, fp);

    elf_ehdr_t *ehdr = elf_get_elf_header(elfo);
    if (ehdr)
        log_info("Ehdr=%p %c%c%c\n",
                ehdr, ehdr->e_ident[EI_MAG1], ehdr->e_ident[EI_MAG2], ehdr->e_ident[EI_MAG3]);

    elf_phdr_t **phdrs = elf_get_elf_programs(elfo);
    if (phdrs) {
        log_info("Phdrs=%p Number of Programs %d:\n", phdrs, ehdr->e_phnum);

        for (int i = 0, phidx; i < ehdr->e_phnum; ++i, (*phdrs)++) {
            const elf_phdr_t *phdr = (const elf_phdr_t *)*phdrs;
            ELF_DICT(&phidx, program, phdr->p_type);
            log_info("\t%d: %s\n",i,  _program[phidx].name);
        }
        free(phdrs);
    }

    elf_shdr_t **shdrs = elf_get_elf_sections(elfo);
    if (shdrs) {
        log_info("Shdrs=%p Number of Sections %d:\n", phdrs, ehdr->e_shnum);

        for(int i=0; i < ehdr->e_shnum; i++, (*shdrs)++) {
            char *shname = elf_get_section_header_name(elfo, *shdrs);
            log_info("\t%d: %s\n",i, strlen(shname) ? shname : "--");
        }
        free(shdrs);
    }

    assert(elf_destroy_all(elfo));
    file_close(fp);

    return 0;
}
