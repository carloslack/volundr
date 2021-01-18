#ifndef _COMMON_TYPES_H
#define _COMMON_TYPES_H

#include <stdint.h>

#if defined( OPTIMIZE_SIZE ) && defined( OPTIMIZE_SPEED )
# error "Conflicting pre-processor definition: OPTIMIZE_SIZE, OPTIMIZE_SPEED"
#endif

typedef enum {
    CMM_ERR=-1,
    CMM_OK=0,
    CMM_ERR_READ,
    CMM_ERR_EINVLD,
    CMM_ERR_ERRLEN
} common_err_t;

typedef int word;
typedef unsigned int uword;

#ifndef _DECLARE_DEFITIONS
#define _CMM_CNDXTRN
#else
#define _CMM_CNDXTRN    extern
#endif

_CMM_CNDXTRN int32_t common_errno;

#endif
