#include "common.h"
#include "utils.h"
#include "log.h"
#include "elfo.h"
#include "validate.h"

bool elf_validate_eheader(const elf_ehdr_t* ehdr)
{
    if(ehdr == NULL) {
        log_debug("Invalid argument");
        return false;
    }
    if((ehdr->e_ident[0] != ELFMAG0) &&
            (ehdr->e_ident[1] != ELFMAG1) &&
            (ehdr->e_ident[2] != ELFMAG2) &&
            (ehdr->e_ident[3] != ELFMAG3)) {
        return false;
    }
    return true;
}

bool elf_validate_filetype(FILE *f)
{
    size_t len = sizeof(Elf32_Ehdr);
    char buf[len+1];
    Elf32_Ehdr *elfHeader;
    fpos_t pos;

    if(f == NULL)
        return false;

    // save current position
    fgetpos(f, &pos);
    rewind(f);

    // TODO: create elf_get_hdr()
    fread(buf, len, 1, f);
    buf[len] = '\0';

    elfHeader = (Elf32_Ehdr*)buf;

    // restore initial position
    fsetpos(f, &pos);

    return elf_validate_eheader((const elf_ehdr_t*)elfHeader);
}

bool elf_validate_index(const elf_t *elfo, elf_half_t index)
{
    if(elfo == NULL || elfo->ehdr == NULL) {
        log_debug("Invalid argument");
        return false;
    }
    if(index >= SHNUM(elfo)) {
        return false;
    }
    return true;
}
