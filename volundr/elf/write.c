#include "write.h"
#include "log.h"

/**
 * @sav: the semantics of this module is somehow wrong.
 * elf_write* should be responsible for writting data
 * to a new ELF object; not to output files.
 */

sbyte* strTableEntry; // XXX ???
u32 strTableOffset; // XXX ???

void elf_write_strtab(i32 offset, i32 len, const sbyte* buf) {
    Elf32_Shdr* e = (Elf32_Shdr*)&buf[offset+len];
    sbyte str[len];
    i32 i;

    for(i = 0; i < e->sh_size; i++)
        str[i] = buf[e->sh_offset + i];

    strTableEntry = str;
}
 
void elf_write_stringname(FILE* fout, i32 index) {
    while(strTableEntry[index] != '\0') 
        logf_it(fout, "%c",strTableEntry[index++]);
    logf_it(fout, "%s\n", "");
}

