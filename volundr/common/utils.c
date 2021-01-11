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

#include "common.h"
#include "utils.h"
#include "log.h"
#include "elfo.h"
#include "asm.h"
#include "map.h"

FILE* file_open(const sbyte* filein, const sbyte *mode)
{
    FILE *fin = fopen(filein, mode);
    if(!fin) {
        log_error("error opening %s : %s", filein, strerror(errno));
        return NULL;
    }
    return fin;
}

bool file_read_all(struct mapped_file *user_data, FILE *fp)
{
    struct stat st;
    void *mapaddr = NULL;

    if (fstat(fileno(fp), &st) < 0) {
        log_fatal("Error: fstat\n");
        asm_exit(-1);
    }

     map_filemap(NULL, st.st_size, fileno(fp), &mapaddr);

     user_data->st = st;
     user_data->mapaddr = mapaddr;

    return true;
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
    if(NULL != base)
    {
        sbyte* biname = get_binary_name(base);
        new_name = malloc(strlen(biname)+strlen(radix)+1);
        strcpy(new_name,biname);
        strcat(new_name,radix);
    }
    return new_name;
}

void *smalloc(size_t size)
{
    void *p = malloc(size);
    if(NULL == p) {
        perror("malloc");
        abort();
    }
    return p;
}

void *scalloc(size_t size, size_t n)
{
    void *p = calloc(size,n);
    if(NULL == p) {
        perror("calloc");
        abort();
    }
    return p;
}

void sfree(void **ptr)
{
    if(ptr && *ptr) {
        free(*ptr);
        *ptr = NULL;
    }
}

void *srealloc(void *ptr, size_t size)
{
    void *tmp = realloc(ptr, size);
    if(NULL == tmp && size)
        log_fatal("realloc failed");
    return tmp;
}

char *sstrdup(const char *str)
{
    char *p = strdup(str);
    if(NULL == p)
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

    for(i = 0; i < len; i++)
        log_info("\\x%x", p[i]);
    log_info("\n");
}

unsigned long hash_string(const unsigned char *name) {
    unsigned long h = 0;
    while (*name) {
        unsigned long g;
        h = (h << 4) + *name++;
        if ((g = h) & 0xf0000000)
            h ^= g >> 24;
        h &= ~g;
    }
    return h;
}
