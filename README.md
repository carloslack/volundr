[![Total alerts](https://img.shields.io/lgtm/alerts/g/carloslack/volundr.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/carloslack/volundr/alerts/)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/carloslack/volundr.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/carloslack/volundr/context:cpp)

<p align="left">
    <a href="https://github.com/carloslack/volundr/blob/master/LICENSE"><img alt="Software License" src="https://img.shields.io/badge/MIT-license-green.svg?style=flat-square"></a>
</p>

## Volundr

### Yet just another ELF64 library that provides basic ELF parsing and some code injection features

    This is one of those small projects that I wanted to write long ago but never actually
    got into doing, so here it is.

    ELF parasite/infection etc.. is nothing new and you can find many similar ones
    (and some very ancient) out there but still this kind of tool is somehow fun to
    write, probably for this reason there are so many hanging around.

    This is currently a work in progress, and likely to be the case for a long time.

    Nevertheless... whatever :)

## Interfacing

### Parsing some data

    // m stores the permissions for file, e.g.: RW, RO...
    open_mode_t m;
    // mapped file data
    struct mapped_file map = {0};
    // Open ELF binary file in read-only
    FILE *fp = file_open_ro(elf_filename, &m);

    // Optionally do some checks
    if(!elf_validate_filetype(fp)) { ...

    // Load target file to make
    // it ready for elf_parse_file()
    if (!file_load_target(&map, fp, m)) { ...

    // After this call the whole file is parsed
    // elfo is the main "handler" (object)
    elf_t *elfo = elf_parse_file(binfile, &map);

    // Retrieve Section Header data
    // Here we print the secton .text offset
    elf_word_t idx = elf_parse_shdr_idx_byname(elfo, ".text");
    ... do some checks
    log_info("Section name .text is at offset %016llx\n", elfo->shdrs[idx]->sh_offset);

    For more details I wrote a bunch of actual examples, described below

### Example options

    There is a "run" script which is a bit handy (it even has crafted auto-completion!)

     $ source ./completion.sh
     $ ./run
    Use <example code> [[example] [-h]]
    examples:
        example-headers
        example-infect-text
        example-infect-note
        example-long
        example-sctidx

    Some of them:

#### example-headers
    Will dump some Elf header to stdout

#### example infect
    This is more fun!
    Currently we support:
        - LSB shared and EXEC infections for .text section padding
        - LSB shared and EXEC infections for SHT_NOTE/PT_NOTE injection (if trojan binary fits)

    See trojan examples et_exec_trojan.S and et_dyn_trojan.S

#### example-long
    A more complete version of example-headers, will dump Elf headers in readelf-ish style.


![Screenshot](examples/elf.png)
