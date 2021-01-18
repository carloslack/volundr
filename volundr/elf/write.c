#include "common.h"
#include "utils.h"
#include "log.h"
#include "elfo.h"
#include "write.h"
#include "log.h"

/**
 * @sav: the semantics of this module is somehow wrong.
 * elf_write* should be responsible for writting data
 * to a new ELF object; not to output files.
 */

char* strTableEntry; // XXX ???
uint32_t strTableOffset; // XXX ???

void elf_write_strtab(int32_t offset, int32_t len, const char* buf) {
    Elf32_Shdr* e = (Elf32_Shdr*)&buf[offset+len];
    char str[len];
    int32_t i;

    for(i = 0; i < e->sh_size; i++)
        str[i] = buf[e->sh_offset + i];

    strTableEntry = str;
}
 
void elf_write_stringname(FILE* fout, int32_t index) {
    while(strTableEntry[index] != '\0') 
        logf_it(fout, "%c",strTableEntry[index++]);
    logf_it(fout, "%s\n", "");
}

