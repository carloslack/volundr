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

.text

#   Shift arguments backwards because
#   of syscall number that comes first

#   void _asm_exit      (int, int);
FN _asm_exit
    movl %edi, %eax             # NR syscall
    movl %esi, %edi             # status
    xor %rsi, %rsi
    syscall
    ret

#   void _asm_fork      (int);
FN _asm_fork
    movl %edi, %eax             # NR syscall
    syscall
    ret

#   int _asm_close      (int, int);
FN _asm_close
    movl %edi, %eax             # NR syscall
    movl %esi, %edi             # int fd
    xor %rsi, %rsi
    syscall
    ret

#   int _asm_mkdir      (int, const sbyte*, mode_t);
FN _asm_mkdir
    movl %edi, %eax             # NR syscall
    movq %rsi, %rdi             # const char *pathname
    movl %edx, %esi             # mode_t mode
    xorl %edx, %edx
    syscall
    ret

#   int _asm_kill       (int, pid_t, int);
FN _asm_kill
    movl %edi, %eax             # NR syscall
    movl %esi, %edi             # pid_t pid
    movl %edx, %esi             # signal
    xorl %edx, %edx
    syscall
    ret

#   int _asm_rmdir      (int, const sbyte*);
FN _asm_rmdir
    movl %edi, %eax             # NR syscall
    movq %rsi, %rdi             # const char *pathname
    xorq %rsi, %rsi
    syscall
    ret

#   int _asm_chown      (int, const sbyte*, uid_t, gid_t);
FN _asm_chown
    movl %edi, %eax             # NR syscall
    movq %rsi, %rdi             # const char *pathname
    movl %edx, %esi             # uid_t owner
    movl %ecx, %edx             # gid_t group
    xorl %ecx, %ecx
    syscall
    ret

#   int _asm_chown      (int, const sbyte*, uid_t, gid_t);
FN _asm_open
    movl %edi, %eax             # NR syscall
    movq %rsi, %rdi             # const char *pathname
    movl %edx, %esi             # int flags
    movl %ecx, %edx             # int mode
    xorl %ecx, %ecx
    syscall
    ret

#   int _asm_write      (int, int, const void*, size_t);
FN _asm_write
    movl %edi, %eax             # NR syscall
    movl %esi, %edi             # int fd
    movq %rdx, %rsi             # const void *buf
    movq %rcx, %rdx             # size_t count
    xorq %rcx, %rcx
    syscall
    ret

