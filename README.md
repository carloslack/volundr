[![Total alerts](https://img.shields.io/lgtm/alerts/g/carloslack/volundr.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/carloslack/volundr/alerts/)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/carloslack/volundr.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/carloslack/volundr/context:cpp)

<p align="left">
    <a href="https://github.com/carloslack/volundr/blob/master/LICENSE"><img alt="Software License" src="https://img.shields.io/badge/MIT-license-green.svg?style=flat-square"></a>
</p>

## Volundr

### Yet just another ELF64 library that provides basic ELF parsing and some code injection features

    This is one of those small projects that I wanted to write long ago but never actually got into doing, so here it is.

    ELF parasite/infection etc.. is nothing new and you can find many similar ones (and some very ancient) out there but still this
    this kind of tool is really fun to write, probably for this reason there are so many hanging around.

    This is currently a working in progress, and likely to be the case for a long time.

    Nevertheless... whatever :)

## Interfacing

### Parse some data

    // m stores the permissions for file, e.g.: RW, RO...
    open_mode_t m;
    // Open ELF binary file in read-only
    FILE *fp = file_open_ro(elf_filename, &m);

    // Optionally do some checks
    if(!elf_validate_filetype(fp)) { ...

    // After this call the whole file is parsed
    // elfo is the main "handler" (object)
    elf_t *elfo = elf_parse_file(file, fp, m);

    // Retrieve Section Header data
    // Here we print the secton .text offset
    elf_word_t idx = elf_parse_shdr_idx_byname(elfo, ".text");
    ... do some checks
    log_info("Section name '%s' is at offset %016llx\n", name, elfo->shdrs[idx]->sh_offset);

    For more details I wrote a bunch of actual examples, described below

### Example options

    There is the "run" script which I is helpful (it even has auto-completion!)

     $ source ./completion.sh
     $ ./run <TAB>
    Use <example code> [[parameters] [-h]]
    Parameters:
    example-headers
    example-infect01
    example-long
    example-sctidx

    Some of them:

#### example-headers
    Will dump some Elf header to stdout

#### example infect
    This is more fun, will actually inject a dummy trojan that will print a message to output
    Currently we support LSD shared and EXEC infections. See et_exec_trojan.S and et_dyn_trojan.S

#### example-long
    A more complete version of example-headers, will dump Elf headers in readelf-ish style.


![Screenshot](examples/elf.png)
