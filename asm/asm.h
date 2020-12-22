#include <sys/types.h>
#include <asm/unistd.h>
#include <unistd.h>
#include <sys/types.h>
#include "../common/common.h"

#define _EXIT(a1)               asm_exit(__NR_exit, a1)
#define _FORK()                 asm_fork(__NR_fork)
#define _KILL(a1,a2)            asm_kill(__NR_kill, a1, a2)
#define _MKDIR(a1,a2)           asm_mkdir(__NR_mkdir, a1, a2)
#define _RMDIR(a1)              asm_rmdir(__NR_rmdir, a1)
#define _UNLINK(a1)             asm_rmdir(__NR_unlink, a1) 
#define _CHOWN(a1,a2,a3)        asm_chown(__NR_chown, a1, a2, a3)
#define _OPEN(a1,a2,a3)         asm_open(__NR_open, a1, a2, a3)
#define _WRITE(a1,a2,a3)        asm_write(__NR_write, a1, a2, a3)
#define _CLOSE(a1)              asm_exit(__NR_close, a1)
#define _MOUNT(a1,a2,a3,a4,a5)  asm_mount(__NR_mount, a1, a2, a3, a4, a5)
#define _UMOUNT(a1)             asm_rmdir(__NR_umount, a1)

/**
 *
 * @brief Sysasms implementation  
 *
 *
 */
#if 0
void asm_exit      (i32 sys, i32 code);
void asm_fork      (i32 sys);
i32 asm_kill       (i32 sys, pid_t pid, i32 sig);
i64 asm_mkdir      (i32 sys, const sbyte* path, mode_t mode);
i32 asm_rmdir      (i32 sys, const sbyte* path);
i32 asm_chown      (i32 sys, const sbyte* path, uid_t owner, gid_t group);
i32 asm_open       (i32 sys, const sbyte* path, i32 flags, mode_t mode);
i32 asm_write      (i32 sys, i32 fd, const void* buf, size_t count);
i32 asm_mount(i32 sys, const sbyte* source, const sbyte* target
        , const sbyte* ftype, u32 flags, const void* data);
#endif
