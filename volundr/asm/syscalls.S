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
.equ SYS_write,     1
.equ SYS_open,      2
.equ SYS_close,     3
.equ SYS_mmap,      9
.equ SYS_mprotect,  10
.equ SYS_fork,      57
.equ SYS_exit,      60
.equ SYS_kill,      62
.equ SYS_mkdir,     83
.equ SYS_rmdir,     84
.equ SYS_chown,     92

.text

#   void asm_exit      (int, int);
FN asm_exit
    movl $SYS_exit, %eax                # NR syscall
    syscall                             # x64 ABI: first 6 args are in registers
    ret

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

#   void *mmap(void *, size_t, int, int, int, off_t);
FN asm_mmap
    movl $SYS_mmap, %eax
    mov %rcx, %r10                      # copy %rcx to %r10 - rcx is userlevel, %r10 kernel
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

#   FILE *asm_fopen     (const char*, const char*);
FN asm_fopen
    movl $SYS_open, %eax
    syscall
    ret

