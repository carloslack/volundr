/**
 *
 *  Völundr / utils.c
 *
 * 	Genereal utilities
 *
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "log.h"
#include "common.h"

_INLINE
FILE* file_open(const sbyte* filein, const sbyte *mode)
{
    FILE* fin = fopen(filein, mode);
    if(!fin) {
        log_error("error opening %s : %s", filein, strerror(errno));
        return NULL;
    }
    return fin;
}

off_t file_size(FILE* fp)
{
    off_t size;
    int r = fseeko(fp, 0L, SEEK_END);
    if(r<0)
        return 0;
    size = ftello(fp);
    rewind(fp);
    return (off_t)size;
}

i16 file_read(void *to, off_t from, off_t n, FILE *fp)
{
    off_t cur;
    if(file_read_norewind(to, from, n, fp, &cur)==CMM_OK) {
        if(fseeko(fp, cur, SEEK_SET)==0) {
            return CMM_OK;
        }
    }
    log_debug("could not read file : %s\n", strerror(errno));
    return CMM_ERR;
}

i16 file_read_norewind(void *to, off_t from, off_t n, FILE *fp, off_t *bkp)
{
    if(!fp||!to) {
        log_debug("invalid argument : %s", (!fp) ? "fp" : "to");
        common_errno = CMM_ERR_EINVLD;
        //errno = EINVAL; // XXX??
        return CMM_ERR;
    }
    off_t cur = ftello(fp); // get current offset
    if(cur<0 || fseeko(fp, from, SEEK_SET)<0) { // goto desired offset
        log_debug("seek error : %s", strerror(errno));
        return CMM_ERR;
    }

    if(fread(to, n, 1, fp)<0) {
        log_debug("read error : %s", strerror(errno));
        return CMM_ERR;
    }

    // FUXOR DEBUG
    if(n==1240)
        dump_buff(to, n);
    // FUXOR DEBUG

    if(bkp)
        *bkp=cur;
    return CMM_OK;
}

void *file_read_all(FILE *fp, int *n)
{
    off_t size = file_size(fp);
    if(size<0)
        log_fatal("No readable data in ELF file");

    void *data = smalloc(size);
    if(file_read(data,0,size-1,fp) != CMM_OK)
        log_fatal("Could not read from ELF file");

    if(n != NULL)
        *n = size;
    return data;
}

_INLINE
off_t file_get_pos(FILE *file)
{
    return file ? (off_t)ftello(file) : CMM_ERR;
}

_INLINE
i16 file_set_pos(FILE *file, off_t offset)
{
    if( fseeko(file, offset, SEEK_SET)<0) {
        perror("file_set_post:fseek");
        return CMM_ERR;
    }
    return CMM_OK;
}


sbyte* get_binary_name(const sbyte* name)
{
    sbyte* qq = strdup(name);
    sbyte* ret = NULL;

    while(strsep(&qq, "/")) {
        if(qq)
            ret = qq;
        else
            break;
    }

    if(!ret)
        return (sbyte*)name;
    else
        return ret;
}

sbyte *get_output_name(const sbyte* base, const sbyte* radix)
{
    char *new_name = NULL;
    if(base != NULL)
    {
        sbyte* biname = get_binary_name(base);
        new_name = malloc(strlen(biname)+strlen(radix)+1);
        strcpy(new_name,biname);
        strcat(new_name,radix);
    }
    return new_name;
}

_INLINE
void *smalloc(size_t size)
{
    void *p = malloc(size);
    if(p == NULL) {
        perror("malloc");
        abort();
    }
    return p;
}

_INLINE
void *scalloc(size_t size, size_t n)
{
    void *p = calloc(size,n);
    if(p == NULL) {
        perror("calloc");
        abort();
    }
    return p;
}

_INLINE
void sfree(void **ptr)
{
    if(ptr && *ptr) {
        free(*ptr);
        *ptr = NULL;
    }
}

_INLINE
void *srealloc(void *ptr, size_t size)
{
    void *tmp = realloc(ptr, size);
    if(tmp == NULL && size)
        log_fatal("realloc failed");
    return tmp;
}

char *sstrdup(const char *str)
{
    char *p = strdup(str);
    if(p==NULL)
        log_fatal("strdup failed"); // noreturn
    return p;
}

void dump_buff(void *str, u32 len) {
    i32 i,j;
    sbyte *s = (sbyte*)str;

    sbyte str_out[len];

    for(i = 0, j = 0; i < len; i++) {
        if(s[i] >= 33 && s[i] <= 126)
            str_out[j++] = (unsigned char)s[i];
        else
            str_out[j++] = '.';
    }
    log_debug_buff(str_out);
}

void dump_buff_hex(void *buf, u32 len) {
    i32 i;
    unsigned char *p = (unsigned char*)buf;

    for(i = 0; i < len; i++) {
        printf("\\x%x", p[i]);
    }
    printf("\n");
}
