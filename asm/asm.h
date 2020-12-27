#include <sys/types.h>
#include <asm/unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

#define asm_fork()              _asm_fork(__NR_fork)
#define asm_exit(a1)            _asm_exit(__NR_exit, a1)
#define asm_close(a1)           _asm_close(__NR_close, a1)
#define asm_mkdir(a1,a2)        _asm_mkdir(__NR_mkdir, a1, a2)
#define asm_rmdir(a1)           _asm_rmdir(__NR_rmdir, a1)
#define asm_kill(a1,a2)         _asm_kill(__NR_kill, a1, a2)
#define asm_chown(a1,a2,a3)     _asm_chown(__NR_chown, a1, a2, a3)
#define asm_open(a1,a2,a3)      _asm_open(__NR_open, a1, a2, a3)
#define asm_write(a1,a2,a3)     _asm_write(__NR_write, a1, a2, a3)
#define asm_fopen(a1,a2)        _asm_fopen(__NR_open, a1, a2)

void _asm_fork      (int nr);
void _asm_exit      (int nr, int code);
int _asm_close      (int nr, int fd);
int _asm_mkdir      (int nr, const char* path, mode_t mode);
int _asm_rmdir      (int nr, const char* path);
int _asm_kill       (int nr, pid_t pid, int sig);
int _asm_chown      (int nr, const char* path, uid_t owner, gid_t group);
int _asm_open       (int nr, const char* path, int flags, mode_t mode);
int _asm_write      (int nr, int fd, const void* buf, size_t count);
FILE *_asm_fopen    (int nr, const char *path, const char *mode);
