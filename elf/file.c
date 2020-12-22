/**
 *
 *  Völundr / file.c
 *
 * 	File/Type utilities
 *
 */

#include "file.h"
static fileInfo_t fileInfo;

void set_output_file_perm(const sbyte* filein) {
    struct stat st;
    stat(filein, &st);
    fileInfo.mode = st.st_mode;
    fileInfo.uid = st.st_uid;
    fileInfo.gid = st.st_gid;
}

fileInfo_t get_output_file_perm() {
    fileInfo_t info = (fileInfo_t)fileInfo;
    return info;
}

#if 0
i32 file_close(file_counter_struct *fct) {
    //do any cleanup and then closes file descriptor
    int n;
    int ret = 0;
    for(n = 0; n < fct->closen; n++) {
        if((ret = fclose(fct->desc_to_close[n])) < 0)
            break;
    }
    return ret;
}
#endif

