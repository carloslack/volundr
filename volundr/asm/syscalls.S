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
 *    @file  asm.S
 *   @brief  x64 syscalls implementation.
 *   @author hash
 */

/**
 * Macro
 *  function declaration
 */
.macro FN fn
    .globl \fn
    .type \fn, @function;\fn:
.endm

/**
 * x86_64-linux-gnu/asm/unistd_64.h
 */
.equ SYS_read,      0
.equ SYS_write,     1
.equ SYS_open,      2
.equ SYS_close,     3
.equ SYS_fstat,     5
.equ SYS_mmap,      9
.equ SYS_mprotect,  10
.equ SYS_munmap,    11
.equ SYS_msync,     26
.equ SYS_fork,      57
.equ SYS_exit,      60
.equ SYS_kill,      62
.equ SYS_mkdir,     83
.equ SYS_rmdir,     84
.equ SYS_chown,     92

.equ MAP_FAILED,    -1

.text

#   void asm_exit      (int, int);
FN asm_exit
    movl $SYS_exit, %eax                # NR syscall
    syscall                             # x64 ABI: first 6 args are in registers
    ret

#   ssize_t read(int, void *, size_t);
FN asm_read
    movl $SYS_read, %eax
    syscall
    retq

#   void asm_fork      (int);
FN asm_fork
    movl $SYS_fork, %eax
    syscall
    ret

#   int asm_close      (int);
FN asm_close
    movl $SYS_close, %eax
    syscall
    ret

#   int asm_close      (int);
FN asm_fstat
    movl $SYS_fstat, %eax
    syscall
    ret

#   void *mmap(void *, size_t, int, int, int, off_t);
FN asm_mmap
    movl $SYS_mmap, %eax
    movq %rcx, %r10                     # copy %rcx to %r10 - rcx is userlevel, %r10 kernel
    syscall
    cmp $0xffffffffffffffea, %rax       # MAP_FAILED
    je mmap_error
    retq
mmap_error:
    mov $MAP_FAILED, %rax
    retq

#   int asm_munmap(void *, size_t);
FN asm_munmap
    movl $SYS_munmap, %eax
    syscall
    ret

#   int msync(void *addr, size_t length, int flags);
FN asm_msync
    movl $SYS_msync, %eax
    syscall
    ret

#   int mprotect(void *, size_t, int)
FN asm_mprotect
    movl $SYS_mkdir, %eax
    syscall
    ret

#   int asm_mkdir      (int, const char*, mode_t);
FN asm_mkdir
    movl $SYS_mkdir, %eax
    syscall
    ret

#   int asm_kill       (int, pid_t, int);
FN asm_kill
    movl $SYS_kill, %eax
    syscall
    ret

#   int asm_rmdir      (int, const char*);
FN asm_rmdir
    movl $SYS_rmdir, %eax
    syscall
    ret

#   int asm_chown      (int, const char*, uid_t, gid_t);
FN asm_chown
    movl $SYS_chown, %eax
    syscall                             # up to 3 arguments registers are already in place
    ret

#   int asm_chown      (int, const char*, uid_t, gid_t);
FN asm_open
    movl $SYS_open, %eax
    syscall
    ret

#   int asm_write      (int, int, const void*, size_t);
FN asm_write
    movl $SYS_write, %eax
    syscall
    ret

