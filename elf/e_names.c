#include "e_names.h"

const sbyte* e_header_type(i32 type) {
    sbyte* ret;
    switch(type) {
        case ET_NONE: ret = E_ET_NONE; break;
        case ET_REL:  ret = E_ET_REL; break;
        case ET_EXEC: ret = E_ET_EXEC; break;
        case ET_DYN:  ret = E_ET_DYN; break;  
        case ET_CORE: ret = E_ET_CORE; break;  
        case ET_NUM:  ret = E_ET_NUM; break;
        case ET_LOOS: ret = E_ET_LOOS; break;  
        case ET_HIOS: ret = E_ET_HIOS; break; 
        case ET_LOPROC: ret = E_ET_LOPROC; break;
        case ET_HIPROC: ret = E_ET_HIPROC; break; 
        default: ret = "unknown?"; break;

    }
    return ret;
}

const sbyte* e_header_machine(i32 machine) {
    sbyte* ret;
    bool notfound = false; 

    switch(machine) {
        case EM_NONE:  ret = E_EM_NONE; break;
        case EM_M32:   ret = E_EM_M32; break;
        case EM_SPARC: ret = E_EM_SPARC; break;
        case EM_386:   ret = E_EM_386; break;
        case EM_68K:   ret = E_EM_68K; break;
        case EM_88K:   ret = E_EM_88K; break;
        case EM_860:   ret = E_EM_860; break;
        case EM_MIPS:  ret = E_EM_MIPS; break;
        case EM_MIPS_RS3_LE: ret = E_EM_MIPS_RS3_LE; break;
        default:
                             notfound = true;
    }

    if(notfound) {
        if(machine >= 11 && machine <= 16) 
            ret = "Reserved";
        else
            ret = "Unknown";
    }
    return ret;
}

const sbyte* e_header_version(i32 version) {
    if(version == 0) return "Invalid version";
    else if(version == 1) return "Current version";
    else return "unknown";
}

const sbyte* e_program_type(i32 type) {
    sbyte* ret;

    switch(type) {
        case PT_NULL:    ret = E_PT_NULL; break;
        case PT_LOAD:    ret = E_PT_LOAD; break;
        case PT_DYNAMIC: ret = E_PT_DYNAMIC; break;
        case PT_INTERP:  ret = E_PT_INTERP; break;
        case PT_NOTE:    ret = E_PT_NOTE; break;
        case PT_SHLIB:   ret = E_PT_SHLIB; break;
        case PT_PHDR:    ret = E_PT_PHDR; break;
        case PT_TLS:     ret = E_PT_TLS; break;
        case PT_NUM:     ret = E_PT_NUM; break;
        case PT_LOOS:    ret = E_PT_LOOS; break;
        case PT_GNU_EH_FRAME: ret = E_PT_GNU_EH_FRAME; break;
        case PT_GNU_STACK:    ret = E_PT_GNU_STACK; break;
        case PT_GNU_RELRO:    ret = E_PT_GNU_RELRO; break;
        case PT_PAX_FLAGS:    ret = E_PT_PAX_FLAGS; break;
        default:  ret = "Unknown"; break;
    }

    return ret;
}
