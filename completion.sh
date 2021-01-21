#!/usr/bin/env bash

_run()
{
    local cmds cur ff
    if (($COMP_CWORD == 1))
    then
        cur="${COMP_WORDS[1]}"
        list=($(find examples/example-* ! -name "*.c" ! -name "*~"|cut -d "/" -f2))
        COMPREPLY=($(compgen -W "$(echo ${list[@]})" -- "$cur"))
        COMPREPLY=( "${COMPREPLY[@]/%/ }" )   # add trailing space to each
    else
        # get all matching files and directories
        COMPREPLY=($(compgen -f  -- "${COMP_WORDS[$COMP_CWORD]}"))

        for ((ff=0; ff<${#COMPREPLY[@]}; ff++)); do
            [[ -d ${COMPREPLY[$ff]} ]] && COMPREPLY[$ff]+='/'
            [[ -f ${COMPREPLY[$ff]} ]] && COMPREPLY[$ff]+=' '
        done
    fi
}

complete -o bashdefault -o default -o nospace -F _run ./run $@
