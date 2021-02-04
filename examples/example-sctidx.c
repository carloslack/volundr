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

#include "common.h"
#include "utils.h"
#include "log.h"
#include "elfo.h"
#include "parse.h"
#include "map.h"
#include "asm.h"
#include "validate.h"
#include "destroy.h"

static bool doit(const char *binfile, const char *name) {
    ASSERT_CON_RET_FALSE(binfile && name);
    open_mode_t m;
    struct mapped_file map = {0};
    const char *file = binfile;
    FILE *fp = file_open_ro(file, &m);

    if (!fp) {
        log_error("Can't open file");
        return false;
    }

    if(!elf_validate_filetype(fp)) {
        log_error("Not a valid ELF file");
        file_close(fp);
        return false;
    }

    if (!file_load_target(&map, fp, m)) {
        log_error("Error loading target ELF\n");
        return false;
    }

    elf_t *elfo = elf_parse_file(file, &map);
    elf_word_t idx = elf_parse_shdr_idx_byname(elfo, name);
    if (idx != -1)
        log_info("Section name '%s' is at offset %016llx\n", name, elfo->shdrs[idx]->sh_offset);
    else
        log_info("Section name '%s' not found\n", name);

    assert(elf_destroy_all(elfo));
    file_close(fp);

    return true;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        log_info("Use %s <elf file> <shdr name>\n", argv[0]);
        asm_exit(0);
    }

    (void)doit(argv[1], argv[2]);

    return 0;
}
