[[  This is an old project of mine that I left behind at some point
and might play with and see how far I can go with it.
    Currently this code is quite useless and need some effort to get this moving forward. ]]

volundr-0.1/        = Evil agent ;)
volundr-0.1/elf/    = ELF library
volundr-0.1/common  = Common library

evil macros:

    __DEBUG__               : turn debug on
    __ELF_SYSV__            : operating system specification

# Currently only printing stuff

## Example output

    -------[ Volundr v0.1:


    Volundr -> /bin/ls

    [D] [map.c:49] file mapped to 0x7f2725925000
    [D] [map.c:134] file unmaped from 0x7f2725925000
    **** ELF HEADER FILE : [126584 bytes] ****
    [Elf Header]
    e_ident[EI_MAG0]: 7f
    e_ident[EI_MAG1]: E
    e_ident[EI_MAG2]: L
    e_ident[EI_MAG3]: F
    e_ident[EI_CLASS]: 2 64-Bit object
    e_ident[EI_DATA]: 1 LSB
    e_ident[EI_VERSION]: 1
    e_ident[EI_PAD]: 0
    e_type: 0x2  [Executable file]
    e_machine: 0x3e [Unknown]
    e_version: 0x1
    e_entry: Current version 0x4049a0
    e_phoff: 0x40
    e_shoff: 0x1e738
    e_flags: 0x0
    e_ehsize: 64
    e_phentsize: 56
    e_phnum: 9
    e_shentsize: 64
    e_shnum: 29
    e_shstrndx: 0x1c
    [N] [print.c:55] headers ok.

    [Elf Program 0]
    p_type 0x6 [Header table entry]
    p_offset 0x40
    p_vaddr 0x400040
    p_paddr 0x400040
    p_filesz 0x1f8
    p_namesz 0x1f8
    p_align 0x8

    [Elf Program 1]
    p_type 0x3 [Program interpreter]
    p_offset 0x238
    p_vaddr 0x400238
    p_paddr 0x400238
    p_filesz 0x1c
    p_namesz 0x1c
    p_align 0x1

    [Elf Program 2]
    p_type 0x1 [Loadable segment]
    p_offset 0x0
    p_vaddr 0x400000
    p_paddr 0x400000
    p_filesz 0x1da64
    p_namesz 0x1da64
    p_align 0x200000

    [Elf Program 3]
    p_type 0x1 [Loadable segment]
    p_offset 0x1de00
    p_vaddr 0x61de00
    p_paddr 0x61de00
    p_filesz 0x800
    p_namesz 0x1568
    p_align 0x200000

    [Elf Program 4]
    p_type 0x2 [Dynamic linking]
    p_offset 0x1de18
    p_vaddr 0x61de18
    p_paddr 0x61de18
    p_filesz 0x1e0
    p_namesz 0x1e0
    p_align 0x8

    [Elf Program 5]
    p_type 0x4 [Auxiliary information]
    p_offset 0x254
    p_vaddr 0x400254
    p_paddr 0x400254
    p_filesz 0x44
    p_namesz 0x44
    p_align 0x4

    [Elf Program 6]
    p_type 0x6474e550 [GCC .eh_frame_hdr segment]
    p_offset 0x1a5f4
    p_vaddr 0x41a5f4
    p_paddr 0x41a5f4
    p_filesz 0x804
    p_namesz 0x804
    p_align 0x4

    [Elf Program 7]
    p_type 0x6474e551 [Stack executability]
    p_offset 0x0
    p_vaddr 0x0
    p_paddr 0x0
    p_filesz 0x0
    p_namesz 0x0
    p_align 0x10

    [Elf Program 8]
    p_type 0x6474e552 [Read-only after relocation]
    p_offset 0x1de00
    p_vaddr 0x61de00
    p_paddr 0x61de00
    p_filesz 0x200
    p_namesz 0x200
    p_align 0x1
    [N] [print.c:62] programs ok.

    [Elf Section NULL]
    sh_name: 0x0
    sh_type: 0x0
    sh_flags: 0x0
    sh_addr: 0x0
    sh_offset: 0x0
    sh_size: 0x0
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x0
    sh_entsize: 0x0

    [Elf Section .interp]
    sh_name: 0xb
    sh_type: 0x1
    sh_flags: 0x2
    sh_addr: 0x400238
    sh_offset: 0x238
    sh_size: 0x1c
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x1
    sh_entsize: 0x0

    [Elf Section .note.ABI-tag]
    sh_name: 0x13
    sh_type: 0x7
    sh_flags: 0x2
    sh_addr: 0x400254
    sh_offset: 0x254
    sh_size: 0x20
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x4
    sh_entsize: 0x0

    [Elf Section .note.gnu.build-id]
    sh_name: 0x21
    sh_type: 0x7
    sh_flags: 0x2
    sh_addr: 0x400274
    sh_offset: 0x274
    sh_size: 0x24
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x4
    sh_entsize: 0x0

    [Elf Section .gnu.hash]
    sh_name: 0x34
    sh_type: 0x6ffffff6
    sh_flags: 0x2
    sh_addr: 0x400298
    sh_offset: 0x298
    sh_size: 0xc0
    sh_link: 0x5
    sh_info: 0x0
    sh_addralign: 0x8
    sh_entsize: 0x0

    [Elf Section .dynsym]
    sh_name: 0x3e
    sh_type: 0xb
    sh_flags: 0x2
    sh_addr: 0x400358
    sh_offset: 0x358
    sh_size: 0xcd8
    sh_link: 0x6
    sh_info: 0x1
    sh_addralign: 0x8
    sh_entsize: 0x24

    [Elf Section .dynstr]
    sh_name: 0x46
    sh_type: 0x3
    sh_flags: 0x2
    sh_addr: 0x401030
    sh_offset: 0x1030
    sh_size: 0x5dc
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x1
    sh_entsize: 0x0

    [Elf Section .gnu.version]
    sh_name: 0x4e
    sh_type: 0x6fffffff
    sh_flags: 0x2
    sh_addr: 0x40160c
    sh_offset: 0x160c
    sh_size: 0x112
    sh_link: 0x5
    sh_info: 0x0
    sh_addralign: 0x2
    sh_entsize: 0x2

    [Elf Section .gnu.version_r]
    sh_name: 0x5b
    sh_type: 0x6ffffffe
    sh_flags: 0x2
    sh_addr: 0x401720
    sh_offset: 0x1720
    sh_size: 0x70
    sh_link: 0x6
    sh_info: 0x1
    sh_addralign: 0x8
    sh_entsize: 0x0

    [Elf Section .rela.dyn]
    sh_name: 0x6a
    sh_type: 0x4
    sh_flags: 0x2
    sh_addr: 0x401790
    sh_offset: 0x1790
    sh_size: 0xa8
    sh_link: 0x5
    sh_info: 0x0
    sh_addralign: 0x8
    sh_entsize: 0x24

    [Elf Section .rela.plt]
    sh_name: 0x74
    sh_type: 0x4
    sh_flags: 0x42
    sh_addr: 0x401838
    sh_offset: 0x1838
    sh_size: 0xa80
    sh_link: 0x5
    sh_info: 0x18
    sh_addralign: 0x8
    sh_entsize: 0x24

    [Elf Section .init]
    sh_name: 0x7e
    sh_type: 0x1
    sh_flags: 0x6
    sh_addr: 0x4022b8
    sh_offset: 0x22b8
    sh_size: 0x1a
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x4
    sh_entsize: 0x0

    [Elf Section .plt]
    sh_name: 0x79
    sh_type: 0x1
    sh_flags: 0x6
    sh_addr: 0x4022e0
    sh_offset: 0x22e0
    sh_size: 0x710
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x10
    sh_entsize: 0x16

    [Elf Section .plt.got]
    sh_name: 0x84
    sh_type: 0x1
    sh_flags: 0x6
    sh_addr: 0x4029f0
    sh_offset: 0x29f0
    sh_size: 0x8
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x8
    sh_entsize: 0x0

    [Elf Section .text]
    sh_name: 0x8d
    sh_type: 0x1
    sh_flags: 0x6
    sh_addr: 0x402a00
    sh_offset: 0x2a00
    sh_size: 0x11259
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x10
    sh_entsize: 0x0

    [Elf Section .fini]
    sh_name: 0x93
    sh_type: 0x1
    sh_flags: 0x6
    sh_addr: 0x413c5c
    sh_offset: 0x13c5c
    sh_size: 0x9
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x4
    sh_entsize: 0x0

    [Elf Section .rodata]
    sh_name: 0x99
    sh_type: 0x1
    sh_flags: 0x2
    sh_addr: 0x413c80
    sh_offset: 0x13c80
    sh_size: 0x6974
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x20
    sh_entsize: 0x0

    [Elf Section .eh_frame_hdr]
    sh_name: 0xa1
    sh_type: 0x1
    sh_flags: 0x2
    sh_addr: 0x41a5f4
    sh_offset: 0x1a5f4
    sh_size: 0x804
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x4
    sh_entsize: 0x0

    [Elf Section .eh_frame]
    sh_name: 0xaf
    sh_type: 0x1
    sh_flags: 0x2
    sh_addr: 0x41adf8
    sh_offset: 0x1adf8
    sh_size: 0x2c6c
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x8
    sh_entsize: 0x0

    [Elf Section .init_array]
    sh_name: 0xb9
    sh_type: 0xe
    sh_flags: 0x3
    sh_addr: 0x61de00
    sh_offset: 0x1de00
    sh_size: 0x8
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x8
    sh_entsize: 0x0

    [Elf Section .fini_array]
    sh_name: 0xc5
    sh_type: 0xf
    sh_flags: 0x3
    sh_addr: 0x61de08
    sh_offset: 0x1de08
    sh_size: 0x8
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x8
    sh_entsize: 0x0

    [Elf Section .jcr]
    sh_name: 0xd1
    sh_type: 0x1
    sh_flags: 0x3
    sh_addr: 0x61de10
    sh_offset: 0x1de10
    sh_size: 0x8
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x8
    sh_entsize: 0x0

    [Elf Section .dynamic]
    sh_name: 0xd6
    sh_type: 0x6
    sh_flags: 0x3
    sh_addr: 0x61de18
    sh_offset: 0x1de18
    sh_size: 0x1e0
    sh_link: 0x6
    sh_info: 0x0
    sh_addralign: 0x8
    sh_entsize: 0x16

    [Elf Section .got]
    sh_name: 0x88
    sh_type: 0x1
    sh_flags: 0x3
    sh_addr: 0x61dff8
    sh_offset: 0x1dff8
    sh_size: 0x8
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x8
    sh_entsize: 0x8

    [Elf Section .got.plt]
    sh_name: 0xdf
    sh_type: 0x1
    sh_flags: 0x3
    sh_addr: 0x61e000
    sh_offset: 0x1e000
    sh_size: 0x398
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x8
    sh_entsize: 0x8

    [Elf Section .data]
    sh_name: 0xe8
    sh_type: 0x1
    sh_flags: 0x3
    sh_addr: 0x61e3a0
    sh_offset: 0x1e3a0
    sh_size: 0x260
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x20
    sh_entsize: 0x0

    [Elf Section .bss]
    sh_name: 0xee
    sh_type: 0x8
    sh_flags: 0x3
    sh_addr: 0x61e600
    sh_offset: 0x1e600
    sh_size: 0xd68
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x20
    sh_entsize: 0x0

    [Elf Section .gnu_debuglink]
    sh_name: 0xf3
    sh_type: 0x1
    sh_flags: 0x0
    sh_addr: 0x0
    sh_offset: 0x1e600
    sh_size: 0x34
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x1
    sh_entsize: 0x0

    [Elf Section .shstrtab]
    sh_name: 0x1
    sh_type: 0x3
    sh_flags: 0x0
    sh_addr: 0x0
    sh_offset: 0x1e634
    sh_size: 0x102
    sh_link: 0x0
    sh_info: 0x0
    sh_addralign: 0x1
    sh_entsize: 0x0
    [N] [print.c:69] sections ok.
    [SYMTAB] Symtab .dynsym has 137 symbols:

       Num        Val          Size    Type         Bind  Vis      Shndx Name
         0: 0000000000000000      0     NOTYPE      LOCAL DEFAULT      0       
         1: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __ctype_toupper_loc
         2: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __uflow
         3: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 getenv
         4: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 sigprocmask
         5: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0  raise
         6: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 localtime
         7: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __mempcpy_chk
         8: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0  abort
         9: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __errno_location
        10: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 strncmp
        11: 0000000000000000      0     NOTYPE       WEAK DEFAULT      0 _ITM_deregisterTMCloneTable
        12: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0  _exit
        13: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 strcpy
        14: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __fpending
        15: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 isatty
        16: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 sigaction
        17: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 iswcntrl
        18: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 wcswidth
        19: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 localeconv
        20: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 mbstowcs
        21: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 readlink
        22: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 clock_gettime
        23: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 setenv
        24: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 textdomain
        25: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 fclose
        26: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 opendir
        27: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 getpwuid
        28: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 bindtextdomain
        29: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 stpcpy
        30: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 dcgettext
        31: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __ctype_get_mb_cur_max
        32: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 strlen
        33: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __lxstat
        34: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __stack_chk_fail
        35: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 getopt_long
        36: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 mbrtowc
        37: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 strchr
        38: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 getgrgid
        39: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __overflow
        40: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 strrchr
        41: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 fgetfilecon
        42: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 gmtime_r
        43: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0  lseek
        44: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 gettimeofday
        45: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __assert_fail
        46: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __strtoul_internal
        47: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 fnmatch
        48: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 memset
        49: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 fscanf
        50: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0  ioctl
        51: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0  close
        52: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 closedir
        53: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __libc_start_main
        54: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 memcmp
        55: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 _setjmp
        56: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 fputs_unlocked
        57: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 calloc
        58: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 lgetfilecon
        59: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 strcmp
        60: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 signal
        61: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0  dirfd
        62: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 getpwnam
        63: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __memcpy_chk
        64: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 sigemptyset
        65: 0000000000000000      0     NOTYPE       WEAK DEFAULT      0 __gmon_start__
        66: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 memcpy
        67: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 getgrnam
        68: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 getfilecon
        69: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0  tzset
        70: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 fileno
        71: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 tcgetpgrp
        72: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __xstat
        73: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 readdir
        74: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 wcwidth
        75: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 fflush
        76: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 nl_langinfo
        78: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __fxstat
        79: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 strcoll
        80: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __freading
        81: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 fwrite_unlocked
        82: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 realloc
        83: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 stpncpy
        84: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 fdopen
        85: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 setlocale
        86: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __printf_chk
        87: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 timegm
        88: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 strftime
        89: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 mempcpy
        90: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 memmove
        91: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0  error
        92: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0   open
        93: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 fseeko
        94: 0000000000000000      0     NOTYPE       WEAK DEFAULT      0 _Jv_RegisterClasses
        95: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 unsetenv
        96: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 strtoul
        97: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __cxa_atexit
        98: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 wcstombs
        99: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 getxattr
       100: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 freecon
       101: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 sigismember
       102: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0   exit
       103: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 fwrite
       104: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __fprintf_chk
       105: 0000000000000000      0     NOTYPE       WEAK DEFAULT      0 _ITM_registerTMCloneTable
       106: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 fflush_unlocked
       107: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 mbsinit
       108: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 iswprint
       109: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 sigaddset
       110: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 strstr
       111: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __ctype_tolower_loc
       112: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __ctype_b_loc
       113: 0000000000000000      0       FUNC     GLOBAL DEFAULT      0 __sprintf_chk
       114: 000000000061e600      8     OBJECT     GLOBAL DEFAULT     26 __progname
       115: 0000000000413c5c      0       FUNC     GLOBAL DEFAULT     15  _fini
       116: 000000000061e610      4     OBJECT     GLOBAL DEFAULT     26 optind
       117: 00000000004022b8      0       FUNC     GLOBAL DEFAULT     11  _init
       118: 0000000000402340      0       FUNC     GLOBAL DEFAULT      0   free
       119: 000000000061e620      8     OBJECT       WEAK DEFAULT     26 program_invocation_name
       120: 000000000061e600      0     NOTYPE     GLOBAL DEFAULT     26 __bss_start
       121: 000000000061f368      0     NOTYPE     GLOBAL DEFAULT     26   _end
       122: 000000000061e620      8     OBJECT     GLOBAL DEFAULT     26 __progname_full
       123: 0000000000412930     38       FUNC     GLOBAL DEFAULT     14 _obstack_memory_used
       124: 000000000061e5f8      8     OBJECT     GLOBAL DEFAULT     25 obstack_alloc_failed_handler
       125: 0000000000412750     17       FUNC     GLOBAL DEFAULT     14 _obstack_begin
       126: 000000000061e600      0     NOTYPE     GLOBAL DEFAULT     25 _edata
       127: 000000000061e640      8     OBJECT     GLOBAL DEFAULT     26 stderr
       128: 00000000004128c0    106       FUNC     GLOBAL DEFAULT     14 _obstack_free
       129: 000000000061e600      8     OBJECT       WEAK DEFAULT     26 program_invocation_short_name
       130: 00000000004023a0      0       FUNC     GLOBAL DEFAULT      0 localtime_r
       131: 0000000000412890     48       FUNC     GLOBAL DEFAULT     14 _obstack_allocated_p
       132: 000000000061e618      8     OBJECT     GLOBAL DEFAULT     26 optarg
       133: 0000000000412770     21       FUNC     GLOBAL DEFAULT     14 _obstack_begin_1
       134: 0000000000412790    245       FUNC     GLOBAL DEFAULT     14 _obstack_newchunk
       135: 0000000000402790      0       FUNC     GLOBAL DEFAULT      0 malloc
       136: 000000000061e608      8     OBJECT     GLOBAL DEFAULT     26 stdout
    [N] [print.c:80] symbols ok.
