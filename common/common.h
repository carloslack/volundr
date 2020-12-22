#ifndef _COMMON_TYPES_H
#define _COMMON_TYPES_H

#include <stdint.h>

#if defined( OPTIMIZE_SIZE ) && defined( OPTIMIZE_SPEED )
# error "Conflicting pre-processor definition: OPTIMIZE_SIZE, OPTIMIZE_SPEED"
#endif

// TODO : implement utils.c:str_common_errno()

typedef enum {
    CMM_ERR=-1,
    CMM_OK=0,
    CMM_ERR_READ,
    CMM_ERR_EINVLD,
    CMM_ERR_ERRLEN
} common_err_t;

typedef int word;
typedef unsigned int uword;

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef int16_t   i16;
typedef int32_t   i32;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef int64_t  i64;
typedef int16_t   s16;
typedef char      sbyte;
typedef int       bool;

#ifndef _OPTMIZE_FOR_SIZE
#define _INLINE inline
#else
#define _INLINE
#endif

#ifndef _DECLARE_DEFITIONS
#define _CMM_CNDXTRN
#else
#define _CMM_CNDXTRN    extern
#endif

_CMM_CNDXTRN i32 common_errno;

#endif
