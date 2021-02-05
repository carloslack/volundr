/**
 *  MIT License
 *
 *  Copyright (c) 2021 Carlos Carvalho
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
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
    ASSERT_CON_RET_NULL(filein && mode);
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
    ASSERT_ARG_RET_NULL(file_data && fp);
    struct stat st;
    if (asm_fstat(fileno(fp), &st) < 0) {
        log_fatal("Error: asm_fstat\n");
        asm_exit(-1);
    }
    void *heap = scalloc(1, st.st_size);

    if (fread(heap, st.st_size, 1, fp) != 1) {
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
    ASSERT_ARG_RET_NULL(file_data);
    ASSERT_CON_RET_NULL(m == F_RW ||
            m == F_RO || m == F_OW );
    struct stat st;
    void *mapaddr = NULL;
    bool rc;

    if (asm_fstat(fileno(fp), &st) < 0) {
        log_fatal("Error: asm_fstat\n");
        asm_exit(-1);
    }

    rc = map_filemap(NULL, st.st_size, fileno(fp), &mapaddr, m);
    if (rc) {
        file_data->st = st;
        file_data->mapaddr = mapaddr;
    }

    return rc;
}

/**
 * msync
 */
bool file_sync_target(struct mapped_file *file_data)
{
    ASSERT_ARG_RET_NULL(file_data);
    ASSERT_CON_RET_NULL(file_data->mapaddr);
    return map_filesync((void*)file_data->mapaddr,
            file_data->st.st_size);
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
