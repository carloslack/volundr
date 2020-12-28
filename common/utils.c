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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>

#include "utils.h"
#include "log.h"
#include "common.h"
#include "asm.h"

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

void *file_read_all(FILE *fp)
{
    struct stat st;

    if (fstat(fileno(fp), &st) < 0) {
        log_fatal("Error: fstat\n");
        asm_exit(-1);
    }

    void *data = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fileno(fp), 0);
    if (MAP_FAILED == (unsigned int*)data) {
        log_fatal("Error: mmap\n");
        asm_exit(-1);
    }

    return data;
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
