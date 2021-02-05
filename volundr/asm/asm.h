#include <sys/types.h>
#include <asm/unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

/**
 * @file  asm.h
 * @brief  Implement x84 syscalls for general use
 * and to avoid going through glibc
 * @see syscalls.S
 */

/*!
 * sys_fork x64 syscall implementation
 */
void asm_fork       (void);

/*!
 * sys_exit x64 syscall implementation
 * @param code exit status
 * @see syscalls.S
 */
ssize_t asm_read    (int fd, void *buf, size_t count);

/*!
 * sys_exit x64 syscall implementation
 * @param code exit status
 * @see syscalls.S
 */
void asm_exit       (int code);

/*!
 * sys_close x64 syscall implementation
 * @param fd file descriptor to close
 * @return 0 on success
 * @see syscalls.S
 */
int asm_close       (int fd);
/*!
 * sys_fstat x64 syscall implementation
 * @param fd file descriptor to close
 * @param buf result file stat
 * @return 0 on success
 * @see syscalls.S
 */
int asm_fstat       (int fd, struct stat *buf);

/*!
 * sys_mkdir x64 syscall implementation
 * @param path path
 * @param mode mode
 * @return 0 on success
 * @see syscalls.S
 */
int asm_mkdir       (const char* path, mode_t mode);

/*!
 * sys_rmdir x64 syscall implementation
 * @param path path
 * @return 0 on success
 * @see syscalls.S
 */
int asm_rmdir       (const char* path);

/*!
 * sys_kill x64 syscall implementation
 * @param pid pid
 * @param sig signal to send
 * @return 0 on success
 * @see syscalls.S
 */
int asm_kill        (pid_t pid, int sig);

/*!
 * sys_chown x64 syscall implementation
 * @param path path
 * @param owner owner uid
 * @param group group gid
 * @return 0 on success
 * @see syscalls.S
 */
int asm_chown       (const char* path, uid_t owner, gid_t group);

/*!
 * sys_open x64 syscall implementation
 * @param path path
 * @param flags flags
 * @param mode mode
 * @return file descriptor
 * @see syscalls.S
 */
int asm_open        (const char* path, int flags, mode_t mode);

/*!
 * sys_write x64 syscall implementation
 * @param fd file fd
 * @param buf buffer to write
 * @param count buffer size
 * @return number of bytes written
 * @see syscalls.S
 */
int asm_write       (int fd, const void* buf, size_t count);

/*!
 * sys_mmap x64 syscall implementation
 * @param addr start address to map
 * @param length size of the mapping
 * @param pror protections
 * @param fd file descriptor
 * @param flags mapping flags
 * @param len size of mapping
 * @param offset offset of mapping - usually 0
 * @return pointer to mapped area
 * @see syscalls.S
 */
void *asm_mmap      (void *addr, size_t length, int prot,
        int fd, int flags, off_t offset);

/*!
 * sys_munmap x64 syscall implementation
 * @param addr address to unmap
 * @return 0 on success
 * @see syscalls.S
 */
int asm_munmap(void *addr, size_t length);

/*!
 * sys_msync x64 syscall implementation
 * @param addr address to sync
 * @param length size of mapping
 * @param flags flags
 *
 * @return 0 on success
 * @see syscalls.S
 */
int asm_msync(void *addr, size_t length, int flags);

/*!
 * sys_mprotect x64 syscall implementation
 * @param addr address to mprotect
 * @param prot protection flags
 * @return 0 on success
 * @see syscalls.S
 */
int asm_mprotect(void *addr, size_t len, int prot);

