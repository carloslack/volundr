#include <sys/types.h>
#include <asm/unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

//#define asm_fork()              _asm_fork()
//#define asm_exit(a1)            _asm_exit(a1)
//#define asm_close(a1)           _asm_close(__NR_close, a1)
//#define asm_mkdir(a1,a2)        _asm_mkdir(a1, a2)
//#define asm_rmdir(a1)           _asm_rmdir(__NR_rmdir, a1)
//#define asm_kill(a1,a2)         _asm_kill(__NR_kill, a1, a2)
//#define asm_chown(a1,a2,a3)     _asm_chown(__NR_chown, a1, a2, a3)
//#define asm_open(a1,a2,a3)      _asm_open(__NR_open, a1, a2, a3)
//#define asm_write(a1,a2,a3)     _asm_write(__NR_write, a1, a2, a3)
//#define asm_fopen(a1,a2)        _asm_fopen(a1, a2)

void asm_fork       ();
void asm_exit       (int code);
int asm_close       (int fd);
int asm_mkdir       (const char* path, mode_t mode);
int asm_rmdir       (const char* path);
int asm_kill        (pid_t pid, int sig);
int asm_chown       (const char* path, uid_t owner, gid_t group);
int asm_open        (const char* path, int flags, mode_t mode);
int asm_write       (int fd, const void* buf, size_t count);
FILE *asm_fopen     (const char *path, const char *mode);
void *asm_mmap      (void *addr, size_t length, int prot,
        int fd, int flags, off_t offset);
int asm_mprotect(void *addr, size_t len, int prot);

