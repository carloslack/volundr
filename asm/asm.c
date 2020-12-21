/**
 *    @file  asm.c
 *   @brief  Syscalls implementation.
 *   @author v00d00
 */

#include "asm.h"

void asm_exit(i32 sys, i32 code) {
    __asm__ __volatile__(
            "movl %0, %%eax\n\t"
            "movl %1, %%ebx\n\t"
            "int $0x80"
            :
            :"r"(sys), "b"(code)
    );
}

void asm_fork(i32 sys) {
    __asm__ __volatile__(
            "movl %0, %%eax\n\t"
            "int $0x80\n\t"
            :
            :"r"(sys)
    );
}

i32 asm_kill(i32 sys, pid_t pid
        , i32 sig) 
{
    i32 ret;
     __asm__ __volatile__(   
            "movl %1, %%eax\n\t"
            "movl %2, %%ebx\n\t"
            "movl %3, %%ecx\n\t"
            "int $0x80\n\t"
            "movl %%eax, %0"
            :"=r"(ret)
            :"r"(sys), "b"(pid), "c"(sig)
    );
    return ret;
}

i32 asm_mkdir(i32 sys, const sbyte* path
        , mode_t mode) 
{
    i32 ret;
    __asm__ __volatile__(
            "movl %1, %%eax\n\t"
            "movl %2, %%ebx\n\t"
            "movl %3, %%ecx\n\t"
            "int $0x80\n\t"
            "movl %%eax, %0"
            :"=r"(ret)
            :"r"(sys), "b"(path), "c"(mode)
    );
    return ret;
}

i32 asm_rmdir(i32 sys, const sbyte* path) {
    i32 ret;
    __asm__ __volatile__(
            "movl %1, %%eax\n\t"
            "movl %2, %%ebx\n\t"
            "int $0x80\n\t"
            "movl %%eax, %0"
            :"=r"(ret)
            :"r"(sys), "b"(path)
    );
    return ret;
}

i32 asm_chown(i32 sys, const sbyte* path
        , uid_t owner
        , gid_t group) 
{
    i32 ret;
    __asm__ __volatile__(
            "movl %1, %%eax\n\t"
            "movl %2, %%ebx\n\t"
            "movl %3, %%ecx\n\t"
            "movl %4, %%edx\n\t"
            "int $0x80\n\t"
            "movl %%eax, %0"
            :"=r"(ret)
            :"r"(sys), "b"(path), "c"(owner), "d"(group)
    );
    return ret;
}

i32 asm_open(i32 sys
        , const sbyte* path
        , i32 flags
        , mode_t mode) 
{
    i32 ret;
    __asm__ __volatile__(
            "movl %1, %%eax\n\t"
            "movl %2, %%ebx\n\t"
            "movl %3, %%ecx\n\t"
            "movl %4, %%edx\n\t"
            "int $0x80\n\t"
            "movl %%eax, %0"
            :"=r"(ret)
            :"r"(sys), "r"(path), "b"(flags), "c"(mode)
    );
    return ret;
}

i32 asm_write(i32 sys
        , i32 fd
        , const void* buf
        , size_t count) 
{
    i32 ret;
    __asm__ __volatile__(
            "movl %1, %%eax\n\t"
            "movl %2, %%ebx\n\t"
            "movl %3, %%ecx\n\t"
            "movl %4, %%edx\n\t"
            "int $0x80\n\t"
            "movl %%eax, %0"
            :"=r"(ret)
            :"r"(sys), "b"(fd), "c"(buf), "d"(count)
    );
    return ret;
}

i32 asm_mount(i32 sys
        , const sbyte* source
        , const sbyte* target
        , const sbyte* ftype
        , u32 flags
        , const void* data)
{
    i32 ret;
    __asm__ __volatile__(
            "movl %1, %%eax\n\t"
            "movl %2, %%ebx\n\t"
            "movl %3, %%ecx\n\t"
            "movl %4, %%edx\n\t"
            "movl %5, %%esi\n\t"
            "movl %6, %%edi\n\t"
            "int $0x80\n\t"
            "movl %%eax, %0"
            :"=r"(ret)
            :"r"(sys), "b"(source), "c"(target), "d"(ftype), "S"(flags), "D"(data)
    );
    return ret;
}

