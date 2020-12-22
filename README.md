[[  This is an old project of mine that I left behind at some point
and might play with and see how far I can go with it.
    Currently this code is quite useless and need some effort to get this moving forward. ]]

volundr-0.1/        = Evil agent ;)
volundr-0.1/elf/    = ELF library
volundr-0.1/common  = Common library

evil macros:

    __DEBUG__               : turn debug on    
    __ELF32__ || __ELF64__  : size of ELF word
    __ELF_SYSV__            : operating system specification

# Currently only printing stuff

## Example output

    -------[ Volundr v0.1:


    Volundr -> /bin/ls

    [D] [map.c:48] file mapped to 0x7fb8ab17d000
    [D] [map.c:133] file unmaped from 0x7fb8ab17d000
    **** ELF HEADER FILE : [126584 bytes] ****
    [Elf Header]
    e_ident: e_ident[EI_MAG0]: 7f
    e_ident[EI_MAG1]: E
    e_ident[EI_MAG2]: L
    e_ident[EI_MAG3]: F
    e_ident[EI_CLASS]: 2 64-Bit object
    e_ident[EI_DATA]: 1 LSB
    e_ident[EI_VERSION]: 1
    e_ident[EI_PAD]: 0
    e_type: 0x00000002  
    e_machine: Executable file 0x0000003e  
    e_version: Unknown 0x00000001  
    e_entry: Current version 0x004049a0
    e_phoff: 64
    e_shoff: 124728
    e_flags: 0x00000000
    e_ehsize: 64
    e_phentsize: 56
    e_phnum: 9
    e_shentsize: 64
    e_shnum: 29
    e_shstrndx: 28
    [N] [print.c:55] headers ok.

    [Elf Program 0]
    p_type: 0x00000006  Header table entry
    p_offset: 64
    p_vaddr: 0x00400040
    p_paddr: 0x00400040
    p_filez: 504
    p_memsz: 504
    p_flags: 0x00000008

    [Elf Program 1]
    p_type: 0x00000003  Program interpreter
    p_offset: 568
    p_vaddr: 0x00400238
    p_paddr: 0x00400238
    p_filez: 28
    p_memsz: 28
    p_flags: 0x00000001

    [Elf Program 2]
    p_type: 0x00000001  Loadable segment
    p_offset: 0
    p_vaddr: 0x00400000
    p_paddr: 0x00400000
    p_filez: 121444
    p_memsz: 121444
    p_flags: 0x00200000

    [Elf Program 3]
    p_type: 0x00000001  Loadable segment
    p_offset: 122368
    p_vaddr: 0x0061de00
    p_paddr: 0x0061de00
    p_filez: 2048
    p_memsz: 5480
    p_flags: 0x00200000

    [Elf Program 4]
    p_type: 0x00000002  Dynamic linking
    p_offset: 122392
    p_vaddr: 0x0061de18
    p_paddr: 0x0061de18
    p_filez: 480
    p_memsz: 480
    p_flags: 0x00000008

    [Elf Program 5]
    p_type: 0x00000004  Auxiliary information
    p_offset: 596
    p_vaddr: 0x00400254
    p_paddr: 0x00400254
    p_filez: 68
    p_memsz: 68
    p_flags: 0x00000004

    [Elf Program 6]
    p_type: 0x6474e550  GCC .eh_frame_hdr segment
    p_offset: 108020
    p_vaddr: 0x0041a5f4
    p_paddr: 0x0041a5f4
    p_filez: 2052
    p_memsz: 2052
    p_flags: 0x00000004

    [Elf Program 7]
    p_type: 0x6474e551  Stack executability
    p_offset: 0
    p_vaddr: 0x00000000
    p_paddr: 0x00000000
    p_filez: 0
    p_memsz: 0
    p_flags: 0x00000010

    [Elf Program 8]
    p_type: 0x6474e552  Read-only after relocation
    p_offset: 122368
    p_vaddr: 0x0061de00
    p_paddr: 0x0061de00
    p_filez: 512
    p_memsz: 512
    p_flags: 0x00000001
    [N] [print.c:62] programs ok.

    [Elf Section <NULL>]
    sh_name: 0x00000000  sh_type: 0x00000000
    sh_flags: 0x00000000
    sh_addr: 0x00000000
    sh_offset: 0
    sh_size: 0
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000000
    sh_entsize: 0

    [Elf Section .interp]
    sh_name: 0x0000000b  sh_type: 0x00000001
    sh_flags: 0x00000002
    sh_addr: 0x00400238
    sh_offset: 568
    sh_size: 28
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000001
    sh_entsize: 0

    [Elf Section .note.ABI-tag]
    sh_name: 0x00000013  sh_type: 0x00000007
    sh_flags: 0x00000002
    sh_addr: 0x00400254
    sh_offset: 596
    sh_size: 32
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000004
    sh_entsize: 0

    [Elf Section .note.gnu.build-id]
    sh_name: 0x00000021  sh_type: 0x00000007
    sh_flags: 0x00000002
    sh_addr: 0x00400274
    sh_offset: 628
    sh_size: 36
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000004
    sh_entsize: 0

    [Elf Section .gnu.hash]
    sh_name: 0x00000034  sh_type: 0x6ffffff6
    sh_flags: 0x00000002
    sh_addr: 0x00400298
    sh_offset: 664
    sh_size: 192
    sh_link: 0x00000005
    sh_info: 0x00000000
    sh_addralign: 0x00000008
    sh_entsize: 0

    [Elf Section .dynsym]
    sh_name: 0x0000003e  sh_type: 0x0000000b
    sh_flags: 0x00000002
    sh_addr: 0x00400358
    sh_offset: 856
    sh_size: 3288
    sh_link: 0x00000006
    sh_info: 0x00000001
    sh_addralign: 0x00000008
    sh_entsize: 24

    [Elf Section .dynstr]
    sh_name: 0x00000046  sh_type: 0x00000003
    sh_flags: 0x00000002
    sh_addr: 0x00401030
    sh_offset: 4144
    sh_size: 1500
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000001
    sh_entsize: 0

    [Elf Section .gnu.version]
    sh_name: 0x0000004e  sh_type: 0x6fffffff
    sh_flags: 0x00000002
    sh_addr: 0x0040160c
    sh_offset: 5644
    sh_size: 274
    sh_link: 0x00000005
    sh_info: 0x00000000
    sh_addralign: 0x00000002
    sh_entsize: 2

    [Elf Section .gnu.version_r]
    sh_name: 0x0000005b  sh_type: 0x6ffffffe
    sh_flags: 0x00000002
    sh_addr: 0x00401720
    sh_offset: 5920
    sh_size: 112
    sh_link: 0x00000006
    sh_info: 0x00000001
    sh_addralign: 0x00000008
    sh_entsize: 0

    [Elf Section .rela.dyn]
    sh_name: 0x0000006a  sh_type: 0x00000004
    sh_flags: 0x00000002
    sh_addr: 0x00401790
    sh_offset: 6032
    sh_size: 168
    sh_link: 0x00000005
    sh_info: 0x00000000
    sh_addralign: 0x00000008
    sh_entsize: 24

    [Elf Section .rela.plt]
    sh_name: 0x00000074  sh_type: 0x00000004
    sh_flags: 0x00000042
    sh_addr: 0x00401838
    sh_offset: 6200
    sh_size: 2688
    sh_link: 0x00000005
    sh_info: 0x00000018
    sh_addralign: 0x00000008
    sh_entsize: 24

    [Elf Section .init]
    sh_name: 0x0000007e  sh_type: 0x00000001
    sh_flags: 0x00000006
    sh_addr: 0x004022b8
    sh_offset: 8888
    sh_size: 26
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000004
    sh_entsize: 0

    [Elf Section .plt]
    sh_name: 0x00000079  sh_type: 0x00000001
    sh_flags: 0x00000006
    sh_addr: 0x004022e0
    sh_offset: 8928
    sh_size: 1808
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000010
    sh_entsize: 16

    [Elf Section .plt.got]
    sh_name: 0x00000084  sh_type: 0x00000001
    sh_flags: 0x00000006
    sh_addr: 0x004029f0
    sh_offset: 10736
    sh_size: 8
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000008
    sh_entsize: 0

    [Elf Section .text]
    sh_name: 0x0000008d  sh_type: 0x00000001
    sh_flags: 0x00000006
    sh_addr: 0x00402a00
    sh_offset: 10752
    sh_size: 70233
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000010
    sh_entsize: 0

    [Elf Section .fini]
    sh_name: 0x00000093  sh_type: 0x00000001
    sh_flags: 0x00000006
    sh_addr: 0x00413c5c
    sh_offset: 80988
    sh_size: 9
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000004
    sh_entsize: 0

    [Elf Section .rodata]
    sh_name: 0x00000099  sh_type: 0x00000001
    sh_flags: 0x00000002
    sh_addr: 0x00413c80
    sh_offset: 81024
    sh_size: 26996
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000020
    sh_entsize: 0

    [Elf Section .eh_frame_hdr]
    sh_name: 0x000000a1  sh_type: 0x00000001
    sh_flags: 0x00000002
    sh_addr: 0x0041a5f4
    sh_offset: 108020
    sh_size: 2052
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000004
    sh_entsize: 0

    [Elf Section .eh_frame]
    sh_name: 0x000000af  sh_type: 0x00000001
    sh_flags: 0x00000002
    sh_addr: 0x0041adf8
    sh_offset: 110072
    sh_size: 11372
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000008
    sh_entsize: 0

    [Elf Section .init_array]
    sh_name: 0x000000b9  sh_type: 0x0000000e
    sh_flags: 0x00000003
    sh_addr: 0x0061de00
    sh_offset: 122368
    sh_size: 8
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000008
    sh_entsize: 0

    [Elf Section .fini_array]
    sh_name: 0x000000c5  sh_type: 0x0000000f
    sh_flags: 0x00000003
    sh_addr: 0x0061de08
    sh_offset: 122376
    sh_size: 8
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000008
    sh_entsize: 0

    [Elf Section .jcr]
    sh_name: 0x000000d1  sh_type: 0x00000001
    sh_flags: 0x00000003
    sh_addr: 0x0061de10
    sh_offset: 122384
    sh_size: 8
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000008
    sh_entsize: 0

    [Elf Section .dynamic]
    sh_name: 0x000000d6  sh_type: 0x00000006
    sh_flags: 0x00000003
    sh_addr: 0x0061de18
    sh_offset: 122392
    sh_size: 480
    sh_link: 0x00000006
    sh_info: 0x00000000
    sh_addralign: 0x00000008
    sh_entsize: 16

    [Elf Section .got]
    sh_name: 0x00000088  sh_type: 0x00000001
    sh_flags: 0x00000003
    sh_addr: 0x0061dff8
    sh_offset: 122872
    sh_size: 8
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000008
    sh_entsize: 8

    [Elf Section .got.plt]
    sh_name: 0x000000df  sh_type: 0x00000001
    sh_flags: 0x00000003
    sh_addr: 0x0061e000
    sh_offset: 122880
    sh_size: 920
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000008
    sh_entsize: 8

    [Elf Section .data]
    sh_name: 0x000000e8  sh_type: 0x00000001
    sh_flags: 0x00000003
    sh_addr: 0x0061e3a0
    sh_offset: 123808
    sh_size: 608
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000020
    sh_entsize: 0

    [Elf Section .bss]
    sh_name: 0x000000ee  sh_type: 0x00000008
    sh_flags: 0x00000003
    sh_addr: 0x0061e600
    sh_offset: 124416
    sh_size: 3432
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000020
    sh_entsize: 0

    [Elf Section .gnu_debuglink]
    sh_name: 0x000000f3  sh_type: 0x00000001
    sh_flags: 0x00000000
    sh_addr: 0x00000000
    sh_offset: 124416
    sh_size: 52
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000001
    sh_entsize: 0

    [Elf Section .shstrtab]
    sh_name: 0x00000001  sh_type: 0x00000003
    sh_flags: 0x00000000
    sh_addr: 0x00000000
    sh_offset: 124468
    sh_size: 258
    sh_link: 0x00000000
    sh_info: 0x00000000
    sh_addralign: 0x00000001
    sh_entsize: 0
    [N] [print.c:69] sections ok.
    [SYMTAB] Symtab .dynsym has 137 symbols:

       Num        Val   Size   Info    (b,     t)  Other  Shndx 	  Name
         0          0      0      0(     0,     0)      0      0 	      
         1         12      0      0(     0,     0)      0      0 	__ctype_toupper_loc
         2         12      0      0(     0,     0)      0      0 	__uflow
         3         12      0      0(     0,     0)      0      0 	getenv
         4         12      0      0(     0,     0)      0      0 	sigprocmask
         5         12      0      0(     0,     0)      0      0 	 raise
         6         12      0      0(     0,     0)      0      0 	localtime
         7         12      0      0(     0,     0)      0      0 	__mempcpy_chk
         8         12      0      0(     0,     0)      0      0 	 abort
         9         12      0      0(     0,     0)      0      0 	__errno_location
        10         12      0      0(     0,     0)      0      0 	strncmp
        11         20      0      0(     0,     0)      0      0 	_ITM_deregisterTMCloneTable
        12         12      0      0(     0,     0)      0      0 	 _exit
        13         12      0      0(     0,     0)      0      0 	strcpy
        14         12      0      0(     0,     0)      0      0 	__fpending
        15         12      0      0(     0,     0)      0      0 	isatty
        16         12      0      0(     0,     0)      0      0 	sigaction
        17         12      0      0(     0,     0)      0      0 	iswcntrl
        18         12      0      0(     0,     0)      0      0 	wcswidth
        19         12      0      0(     0,     0)      0      0 	localeconv
        20         12      0      0(     0,     0)      0      0 	mbstowcs
        21         12      0      0(     0,     0)      0      0 	readlink
        22         12      0      0(     0,     0)      0      0 	clock_gettime
        23         12      0      0(     0,     0)      0      0 	setenv
        24         12      0      0(     0,     0)      0      0 	textdomain
        25         12      0      0(     0,     0)      0      0 	fclose
        26         12      0      0(     0,     0)      0      0 	opendir
        27         12      0      0(     0,     0)      0      0 	getpwuid
        28         12      0      0(     0,     0)      0      0 	bindtextdomain
        29         12      0      0(     0,     0)      0      0 	stpcpy
        30         12      0      0(     0,     0)      0      0 	dcgettext
        31         12      0      0(     0,     0)      0      0 	__ctype_get_mb_cur_max
        32         12      0      0(     0,     0)      0      0 	strlen
        33         12      0      0(     0,     0)      0      0 	__lxstat
        34         12      0      0(     0,     0)      0      0 	__stack_chk_fail
        35         12      0      0(     0,     0)      0      0 	getopt_long
        36         12      0      0(     0,     0)      0      0 	mbrtowc
        37         12      0      0(     0,     0)      0      0 	strchr
        38         12      0      0(     0,     0)      0      0 	getgrgid
        39         12      0      0(     0,     0)      0      0 	__overflow
        40         12      0      0(     0,     0)      0      0 	strrchr
        41         12      0      0(     0,     0)      0      0 	fgetfilecon
        42         12      0      0(     0,     0)      0      0 	gmtime_r
        43         12      0      0(     0,     0)      0      0 	 lseek
        44         12      0      0(     0,     0)      0      0 	gettimeofday
        45         12      0      0(     0,     0)      0      0 	__assert_fail
        46         12      0      0(     0,     0)      0      0 	__strtoul_internal
        47         12      0      0(     0,     0)      0      0 	fnmatch
        48         12      0      0(     0,     0)      0      0 	memset
        49         12      0      0(     0,     0)      0      0 	fscanf
        50         12      0      0(     0,     0)      0      0 	 ioctl
        51         12      0      0(     0,     0)      0      0 	 close
        52         12      0      0(     0,     0)      0      0 	closedir
        53         12      0      0(     0,     0)      0      0 	__libc_start_main
        54         12      0      0(     0,     0)      0      0 	memcmp
        55         12      0      0(     0,     0)      0      0 	_setjmp
        56         12      0      0(     0,     0)      0      0 	fputs_unlocked
        57         12      0      0(     0,     0)      0      0 	calloc
        58         12      0      0(     0,     0)      0      0 	lgetfilecon
        59         12      0      0(     0,     0)      0      0 	strcmp
        60         12      0      0(     0,     0)      0      0 	signal
        61         12      0      0(     0,     0)      0      0 	 dirfd
        62         12      0      0(     0,     0)      0      0 	getpwnam
        63         12      0      0(     0,     0)      0      0 	__memcpy_chk
        64         12      0      0(     0,     0)      0      0 	sigemptyset
        65         20      0      0(     0,     0)      0      0 	__gmon_start__
        66         12      0      0(     0,     0)      0      0 	memcpy
        67         12      0      0(     0,     0)      0      0 	getgrnam
        68         12      0      0(     0,     0)      0      0 	getfilecon
        69         12      0      0(     0,     0)      0      0 	 tzset
        70         12      0      0(     0,     0)      0      0 	fileno
        71         12      0      0(     0,     0)      0      0 	tcgetpgrp
        72         12      0      0(     0,     0)      0      0 	__xstat
        73         12      0      0(     0,     0)      0      0 	readdir
        74         12      0      0(     0,     0)      0      0 	wcwidth
        75         12      0      0(     0,     0)      0      0 	fflush
        76         12      0      0(     0,     0)      0      0 	nl_langinfo
        77         12      0      0(     0,     0)      0      0 	ungetc
        78         12      0      0(     0,     0)      0      0 	__fxstat
        79         12      0      0(     0,     0)      0      0 	strcoll
        80         12      0      0(     0,     0)      0      0 	__freading
        81         12      0      0(     0,     0)      0      0 	fwrite_unlocked
        82         12      0      0(     0,     0)      0      0 	realloc
        83         12      0      0(     0,     0)      0      0 	stpncpy
        84         12      0      0(     0,     0)      0      0 	fdopen
        85         12      0      0(     0,     0)      0      0 	setlocale
        86         12      0      0(     0,     0)      0      0 	__printf_chk
        87         12      0      0(     0,     0)      0      0 	timegm
        88         12      0      0(     0,     0)      0      0 	strftime
        89         12      0      0(     0,     0)      0      0 	mempcpy
        90         12      0      0(     0,     0)      0      0 	memmove
        91         12      0      0(     0,     0)      0      0 	 error
        92         12      0      0(     0,     0)      0      0 	  open
        93         12      0      0(     0,     0)      0      0 	fseeko
        94         20      0      0(     0,     0)      0      0 	_Jv_RegisterClasses
        95         12      0      0(     0,     0)      0      0 	unsetenv
        96         12      0      0(     0,     0)      0      0 	strtoul
        97         12      0      0(     0,     0)      0      0 	__cxa_atexit
        98         12      0      0(     0,     0)      0      0 	wcstombs
        99         12      0      0(     0,     0)      0      0 	getxattr
       100         12      0      0(     0,     0)      0      0 	freecon
       101         12      0      0(     0,     0)      0      0 	sigismember
       102         12      0      0(     0,     0)      0      0 	  exit
       103         12      0      0(     0,     0)      0      0 	fwrite
       104         12      0      0(     0,     0)      0      0 	__fprintf_chk
       105         20      0      0(     0,     0)      0      0 	_ITM_registerTMCloneTable
       106         12      0      0(     0,     0)      0      0 	fflush_unlocked
       107         12      0      0(     0,     0)      0      0 	mbsinit
       108         12      0      0(     0,     0)      0      0 	iswprint
       109         12      0      0(     0,     0)      0      0 	sigaddset
       110         12      0      0(     0,     0)      0      0 	strstr
       111         12      0      0(     0,     0)      0      0 	__ctype_tolower_loc
       112         12      0      0(     0,     0)      0      0 	__ctype_b_loc
       113         12      0      0(     0,     0)      0      0 	__sprintf_chk
       114     1a0011 6415872      0(     0,     0)      0      0 	__progname
       115      f0012 4275292      0(     0,     0)      0      0 	 _fini
       116     1a0011 6415888      0(     0,     0)      0      0 	optind
       117      b0012 4203192      0(     0,     0)      0      0 	 _init
       118         12 4203328      0(     0,     0)      0      0 	  free
       119     1a0021 6415904      0(     0,     0)      0      0 	program_invocation_name
       120     1a0010 6415872      0(     0,     0)      0      0 	__bss_start
       121     1a0010 6419304      0(     0,     0)      0      0 	  _end
       122     1a0011 6415904      0(     0,     0)      0      0 	__progname_full
       123      e0012 4270384      0(     0,     0)      0      0 	_obstack_memory_used
       124     190011 6415864      0(     0,     0)      0      0 	obstack_alloc_failed_handler
       125      e0012 4269904      0(     0,     0)      0      0 	_obstack_begin
       126     190010 6415872      0(     0,     0)      0      0 	_edata
       127     1a0011 6415936      0(     0,     0)      0      0 	stderr
       128      e0012 4270272      0(     0,     0)      0      0 	_obstack_free
       129     1a0021 6415872      0(     0,     0)      0      0 	program_invocation_short_name
       130         12 4203424      0(     0,     0)      0      0 	localtime_r
       131      e0012 4270224      0(     0,     0)      0      0 	_obstack_allocated_p
       132     1a0011 6415896      0(     0,     0)      0      0 	optarg
       133      e0012 4269936      0(     0,     0)      0      0 	_obstack_begin_1
       134      e0012 4269968      0(     0,     0)      0      0 	_obstack_newchunk
       135         12 4204432      0(     0,     0)      0      0 	malloc
       136     1a0011 6415880      0(     0,     0)      0      0 	stdout
       [N] [print.c:80] symbols ok.
