#include "common.h"
#include "utils.h"
#include "log.h"
#include "elfo.h"
#include "destroy.h"
#include "asm.h"
#include "map.h"
#include <errno.h>

//bool destroy_meta(elf_t* elf) 
//{
//    bool ret = false;
//    if(elf) {
//        elf_t **e = &elf;
//        sfree((void**)&(*e)->meta);
//        if(&(*e)->meta)
//            ret = true;
//    }
//    return ret;
//}

bool elf_destroy_header(elf_t *elf)
{
    bool ret = false;
    if(elf) {
        elf_t **e = &elf;
        /** Ehdr here is just mapped memory
         * whereas mapaddr == data
         */
        if (!map_fileunmap((*e)->mapaddr, (*e)->fsize))
            ret = true;
    }
    return ret;
}

bool elf_destroy_program(elf_t *elf)
{
    bool ret = false;
    if(elf) {
        elf_t **e = &elf;
        sfree((void**)&(*e)->phdrs);
        if(!(*e)->phdrs)
            ret = true;
    }
    return ret;
}

bool elf_destroy_section(elf_t *elf)
{
    bool ret = false;
    if(elf) {
        elf_t **e = &elf;
        sfree((void**)&(*e)->shdrs);
        if(!(*e)->shdrs)
            ret = true;
    }
    return ret;
}

bool elf_destroy_elfo(elf_t *elf)
{
    bool ret = false;
    if(elf) {
        elf_t **e = &elf;
        sfree((void**)e);
        if(!(*e))
            ret = true;
    }
    return ret;
}

//bool destroy_file(elf_t* elf) 
//{
//    bool ret = false;
//    if(elf) {
//        elf_t **e = &elf;
//        sfree((void**)&(*e)->fp);
//        if(&(*e)->fp)
//            ret = true;
//    }
//    return ret;
//}
