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
#include <stdbool.h>

#include "common.h"
#include "utils.h"
#include "log.h"
#include "elfo.h"
#include "asm.h"
#include "map.h"

FILE* file_open(const char* filein, const char *mode,
        open_mode_t m, open_mode_t *rv)
{
    ASSERT_CON_RET_NULL(rv != NULL);
    FILE *fin = fopen(filein, mode);
    if(!fin) {
        log_error("error opening %s : %s", filein, strerror(errno));
        return NULL;
    }
    if (rv)
        *rv = m;
    return fin;
}

bool file_load_source(struct mapped_file *file_data, FILE *fp)
{
    struct stat st;
    if (fstat(fileno(fp), &st) < 0) {
        log_fatal("Error: fstat\n");
        asm_exit(-1);
    }
    void *heap = scalloc(1, st.st_size);

    if (read(fileno(fp), heap, st.st_size) != st.st_size) {
        free(heap);
        log_fatal("Error: fread\n");
    }

    file_data->st = st;
    file_data->heap = heap;

    return true;
}

/**
 * mmap
 */
bool file_load_target(struct mapped_file *file_data, FILE *fp, open_mode_t m)
{
    struct stat st;
    void *mapaddr = NULL;
    bool rc;

    if (fstat(fileno(fp), &st) < 0) {
        log_fatal("Error: fstat\n");
        asm_exit(-1);
    }

    rc = map_filemap(NULL, st.st_size, fileno(fp), &mapaddr, m);
    if (rc) {
        file_data->st = st;
        file_data->mapaddr = mapaddr;
    }

    return rc;
}

char* get_binary_name(const char* name)
{
    char* qq = strdup(name);
    char* ret = NULL;

    while(strsep(&qq, "/")) {
        if(qq)
            ret = qq;
        else
            break;
    }

    if(!ret)
        return (char*)name;
    else
        return ret;
}

char *get_output_name(const char* base, const char* radix)
{
    char *new_name = NULL;
    if(NULL != base)
    {
        char* biname = get_binary_name(base);
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

void dump_buff(void *str, uint32_t len) {
    int32_t i,j;
    char *s = (char*)str;

    char str_out[len];

    for(i = 0, j = 0; i < len; i++) {
        if(s[i] >= 33 && s[i] <= 126)
            str_out[j++] = (unsigned char)s[i];
        else
            str_out[j++] = '.';
    }
    log_debug_buff(str_out);
}

void dump_buff_hex(void *buf, uint32_t len) {
    int32_t i;
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
