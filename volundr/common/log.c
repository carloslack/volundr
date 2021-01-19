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
/**
 *    @file  log.c
 *   @brief  Log wrapper interface
 */

#include <stdio.h>
#include <stdarg.h>

#include "common.h"
#include "utils.h"
#include "asm.h"
#include "log.h"
#include "elfo.h"

int32_t _log_it(const char *f, int l, const char* fmt, ...)
{
    int32_t r = 0;
    va_list ap;
    va_start(ap, fmt);
    r = vfprintf(stdout, fmt, ap);
    fprintf(stdout, "\n");
    va_end(ap);
    return r;
}


int32_t _log_info(const char *f, int l, const char* fmt, ...)
{
    int32_t r = 0;
    va_list ap;
    va_start(ap, fmt);
#ifdef DEBUG
    fprintf(stdout, "[N] ");
#endif
    r = vfprintf(stdout, fmt, ap);
    va_end(ap);
    return r;
}

int32_t logf_it(FILE *f, const char* fmt, ...)
{
    int32_t r = 0;
    va_list ap;
    va_start(ap, fmt);
    r = vfprintf(f, fmt, ap);
    //XXX: decide if there will be newline
    va_end(ap);
    return r;
}

int32_t logf_info(FILE *f, const char* fmt, ...) // XXX: add __LINE__
{
    int32_t r = 0;
    va_list ap;
    va_start(ap, fmt);
#ifdef DEBUG
    fprintf(f, "[N] ");
#endif
    r = vfprintf(f, fmt, ap);
    fprintf(stdout, "\n");
    va_end(ap);
    return r;
}

int32_t logf_debug(FILE *f, const char* fmt, ...)
{
    int32_t r = 0;
#ifdef DEBUG
    va_list ap;
    va_start(ap, fmt);
    fprintf(f, "[D] ");
    r = vfprintf(f, fmt, ap);
    fprintf(stdout, "\n");
    va_end(ap);
#endif
    return r;
}

int32_t _log_debug(const char *f, int l, const char* fmt, ...)
{
    int32_t r = 0;
#ifdef DEBUG
    va_list ap;
    va_start(ap, fmt);
    fprintf(stdout, "[D] ");
    fprintf(stdout, "[%s:%d] ", f, l);
    r = vfprintf(stdout, fmt, ap);
    fprintf(stdout, "\n");
    va_end(ap);
#endif
    return r;
}

int32_t _log_warning(const char *f, int l, const char* fmt, ...)
{
    int32_t r;
    va_list ap;
    va_start(ap, fmt);
#ifdef DEBUG
    fprintf(stdout, "[W] ");
    fprintf(stdout, "[%s:%d] ", f, l);
#endif
    r = vfprintf(stderr, fmt, ap);
    fprintf(stdout, "\n");
    va_end(ap);
    return r;
}

int32_t _log_error(const char *f, int l, const char* fmt, ...)
{
    int32_t r;
    va_list ap;
    va_start(ap, fmt);
#ifdef DEBUG
    fprintf(stderr, "[E] ");
    fprintf(stderr, "[%s:%d] ", f, l);
#endif
    r = vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
#ifdef ABORT_ON_ERROR
    abort();
#endif
    return r;
}

void _log_fatal(const char *f, int l, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
#ifdef DEBUG
    fprintf(stdout, "[!] ");
    fprintf(stdout, "[%s:%d] ", f, l);
#endif
    vfprintf(stderr, fmt, ap);
    fprintf(stdout, " : ending execution...\n");
    va_end(ap);
    asm_exit(1);
}

int32_t _log_info_buff(const char *f, int l, const char* fmt, ...)
{
    int32_t r;
    va_list ap;
    va_start(ap, fmt);
#ifdef DEBUG
    fprintf(stdout, "[NB] ");
    fprintf(stdout, "[%s:%d] ", f, l);
    fprintf(stdout, "\n");
#endif
    r = vfprintf(stderr, fmt, ap);
    fprintf(stdout, "\n");
    va_end(ap);
    return r;
}

int32_t _log_debug_buff(const char *f, int l, const char* fmt, ...)
{
    int32_t r;
    va_list ap;
    va_start(ap, fmt);
#ifdef DEBUG
    fprintf(stdout, "[DB] ");
    fprintf(stdout, "[%s:%d] ", f, l);
    fprintf(stdout, "\n");
#endif
    r = vfprintf(stderr, fmt, ap);
    fprintf(stdout, "\n");
    va_end(ap);
    return r;
}

