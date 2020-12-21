#ifndef _ELF_H
#define _ELF_H

#include <elf.h>

/**
 * @defgroup elf LibELF
 */

#include "../common/common.h" // XXX : relative path please.

#if defined( PARANOID_CHECK ) && defined( LOOSE_CHECK )
# error "Conflicting pre-processor definition: PARANOID_CHECK, LOOSE_CHECK"
#endif

#define USEMAP // TODO: Remove it later. It is just for fread/mmap transition

//#include "print.h"
//#include "write.h"
//#include "validate.h"
//#include "e_names.h"

#endif

