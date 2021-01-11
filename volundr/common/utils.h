#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FINFOUT 2
#define MAXFOPEN 128

#define ARG_MSG         ("invalid argument")
#define CON_MSG         ("contract prerequisite not attended")

#define ASSERT_RET(x,y,z) {             \
                if(!(x)) {              \
                    log_error((y));     \
                    return z;           \
                }                       \
    }

#define ASSERT_(x,y,z) {                \
                if(!(x))                \
                    log_error((y));     \
    }


#define ASSERT_RET_NULL(x,y)    ASSERT_RET(x,y,NULL)
#define ASSERT_RET_FALSE(x,y)   ASSERT_RET(x,y,false)
#define ASSERT_NULL(x,y)    ASSERT_(x,y,NULL)
#define ASSERT_FALSE(x,y)   ASSERT_(x,y,false)

// argument asserts
#define ASSERT_ARG_RET_NULL(x)  ASSERT_RET_NULL(x,ARG_MSG)
#define ASSERT_ARG_RET_FALSE(x) ASSERT_RET_FALSE(x,ARG_MSG)

// contract asserts
#define ASSERT_CON_RET_NULL(x)  ASSERT_RET_NULL(x,CON_MSG)
#define ASSERT_CON_RET_FALSE(x) ASSERT_RET_FALSE(x,CON_MSG)

#define ASSERT_CON_NULL(x)  ASSERT_NULL(x,CON_MSG)
#define ASSERT_CON_FALSE(x) ASSERT_FALSE(x,CON_MSG)

// smart file opens
#define file_open_rw(x) (file_open(x, "a+"))
#define file_open_ro(x) (file_open(x, "rb"))
#define file_open_ow(x) (file_open(x, "w+"))
#define file_close(x) (fclose(x))

/**
 *  @brief Holds some data about ELF raw file from the disk
 */
struct mapped_file{
    struct  stat    st;     /**< stat, currently only for file size */
    void    *mapaddr;       /**< ELF file */
};

off_t   file_size           (FILE*);
FILE*   file_open           (const sbyte *, const sbyte *mode);
bool    file_read_all       (struct mapped_file *, FILE *);
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
unsigned long hash_string(const unsigned char *);
#endif
