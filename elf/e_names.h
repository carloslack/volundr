#ifndef _ENAMES_H
#define _ENAMES_H

#include <elf.h>
#include "types.h"

#define E_ET_NONE   "No file type"
#define E_ET_REL    "Relocatable file"
#define E_ET_EXEC   "Executable file"
#define E_ET_DYN    "Shared object file"
#define E_ET_CORE   "Core file"
#define E_ET_NUM    "Number of defined types"
#define E_ET_LOOS   "OS-specific range start"
#define E_ET_HIOS   "OS-specific range end"
#define E_ET_LOPROC "Processor-specific range start"
#define E_ET_HIPROC "Processor-specific range end"

#define E_EM_NONE   "No machine"
#define E_EM_M32    "AT&T WE32100"
#define E_EM_SPARC  "SPARC"
#define E_EM_386    "Intel Arch"
#define E_EM_68K    "Motorola 6800"
#define E_EM_88K    "Motorola 88000"
#define E_EM_860    "Intel 80860"
#define E_EM_MIPS   "MIPS RS3000 big-endian"
#define E_EM_MIPS_RS3_LE "MIPS RS4000 little-endian"
 
#define E_PT_NULL   "Unused"
#define E_PT_LOAD   "Loadable segment"
#define E_PT_DYNAMIC "Dynamic linking"
#define E_PT_INTERP "Program interpreter"
#define E_PT_NOTE   "Auxiliary information"
#define E_PT_SHLIB  "Reserved"
#define E_PT_PHDR   "Header table entry"
#define E_PT_TLS    "Thread-local storage segment"
#define E_PT_NUM    "Number of defined types"
#define E_PT_LOOS   "Start of OS-specific"
#define E_PT_GNU_EH_FRAME "GCC .eh_frame_hdr segment"
#define E_PT_GNU_STACK "Stack executability"
#define E_PT_GNU_RELRO "Read-only after relocation"
#define E_PT_PAX_FLAGS "PaX flag markings"

const sbyte* e_header_type(i32 type);
const sbyte* e_header_machine(i32 machine);
const sbyte* e_header_version(i32 version);
const sbyte* e_program_type(i32 type);

#endif
