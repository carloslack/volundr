#ifndef _FILE_H
#define _FILE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "elfo.h"

typedef struct {
    __mode_t mode;
    __uid_t uid;
    __gid_t gid;
} fileInfo_t;


void set_output_file_perm(const sbyte* filein);
fileInfo_t get_output_file_perm();

#endif
