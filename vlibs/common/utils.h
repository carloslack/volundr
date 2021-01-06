#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <string.h>
#include "common.h"

#define FINFOUT 2
#define MAXFOPEN 128

#define ARG_MSG         ("invalid argument")
#define CON_MSG         ("contract prerequisite not attended")

#define ASSERT_RET(x,y,z) {             \
                if(!(x)) {              \
                    log_debug((y));     \
                    return z;           \
                }                       \
    }


#define ASSERT_RET_NULL(x,y)    ASSERT_RET(x,y,NULL)
#define ASSERT_RET_FALSE(x,y)   ASSERT_RET(x,y,false)

// argument asserts
#define ASSERT_ARG_RET_NULL(x)  ASSERT_RET_NULL(x,ARG_MSG)
#define ASSERT_ARG_RET_FALSE(x) ASSERT_RET_FALSE(x,ARG_MSG)

// contract asserts
#define ASSERT_CON_RET_NULL(x)  ASSERT_RET_NULL(x,CON_MSG)
#define ASSERT_CON_RET_FALSE(x) ASSERT_RET_FALSE(x,CON_MSG)

// smart file opens
#define file_open_rw(x) (file_open(x, "a+"))
#define file_open_ro(x) (file_open(x, "rb"))
#define file_open_ow(x) (file_open(x, "w+"))

off_t   file_size           (FILE*);
FILE*   file_open           (const sbyte *, const sbyte *mode);
void*   file_read_all       (FILE *);
i16     file_read           (void*, off_t, off_t, FILE*);

FILE*   open_output         (const sbyte *); // XXX rename to file_open_output?
char*   get_output_name     (const sbyte *, const sbyte*);
sbyte*  get_binary_name     (const sbyte*);

void* smalloc       (size_t);
void* scalloc       (size_t, size_t);
void* srealloc      (void *, size_t);
void  sfree         (void**);

char*   sstrdup             (const char*);

void    dump_buff           (void *, u32);
void    dump_buff_hex       (void *, u32);
#endif
