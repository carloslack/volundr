#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

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


#define ASSERT_RET_NULL(x,y)        ASSERT_RET(x,y,NULL)
#define ASSERT_RET_FALSE(x,y)       ASSERT_RET(x,y,false)
#define ASSERT_RET_VAL(x,y,val)     ASSERT_RET(x,y,val)
#define ASSERT_NULL(x,y)            ASSERT_(x,y,NULL)
#define ASSERT_FALSE(x,y)           ASSERT_(x,y,false)

// argument asserts
#define ASSERT_ARG_RET_NULL(x)      ASSERT_RET_NULL(x,ARG_MSG)
#define ASSERT_ARG_RET_FALSE(x)     ASSERT_RET_FALSE(x,ARG_MSG)
#define ASSERT_ARG_RET_VAL(x, val)  ASSERT_RET_VAL(x,ARG_MSG, val)

// contract asserts
#define ASSERT_CON_RET_NULL(x)      ASSERT_RET_NULL(x,CON_MSG)
#define ASSERT_CON_RET_FALSE(x)     ASSERT_RET_FALSE(x,CON_MSG)
#define ASSERT_CON_RET_VAL(x, val)  ASSERT_RET_VAL(x,CON_MSG, val)

#define ASSERT_CON_NULL(x)          ASSERT_NULL(x,CON_MSG)
#define ASSERT_CON_FALSE(x)         ASSERT_FALSE(x,CON_MSG)

// smart (O'Rlly??) file opens
typedef enum { F_RW = 1, F_RO, F_OW } open_mode_t;

/**! XXX: Be careful, files will be created if they do not exist ! */
#define file_open_rw(x,y) (file_open(x, "a+", F_RW, y))
#define file_open_ro(x,y) (file_open(x, "rb", F_RO, y))
#define file_open_ow(x,y) (file_open(x, "w+", F_OW, y))
#define file_close(x) (fclose(x))

/**
 *  @brief Holds some data about ELF raw file from the disk
 */
struct mapped_file{
    struct  stat    st;     /**< stat, currently only for file size */
    void    *mapaddr;       /**< file from mmap() */
    void    *heap;          /**< file from heap */
};

off_t   file_size           (FILE*);
FILE    *file_open          (const char* filein, const char *, open_mode_t, open_mode_t *);
bool    file_load_target    (struct mapped_file *, FILE *, open_mode_t);
bool    file_load_source    (struct mapped_file *file_data, FILE *fp);
bool    file_sync_target    (struct mapped_file *);
FILE*   open_output         (const char *); // XXX rename to file_open_output?
char*   get_output_name     (const char *, const char*);
char*   get_binary_name     (const char*);

void* smalloc       (size_t);
void* scalloc       (size_t, size_t);
void* srealloc      (void *, size_t);
void  sfree         (void**);

char*   sstrdup             (const char*);

void    dump_buff           (void *, uint32_t);
void    dump_buff_hex       (void *, uint32_t);
unsigned long hash_string(const unsigned char *);
#endif
