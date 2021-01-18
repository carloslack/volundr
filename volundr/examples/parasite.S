#
#   Hello world example that loads
# the relative address of the string into RSI
#   The address loaded into EBX is the fake
# that will be replaced by the infection module
# by the actual one representing the image on disk
#   - hash
#

.globl _start

.text

_start:
    # Save registers
    push %rax
    push %rcx
    push %rdx
    push %rsi
    push %rdi
    push %r11

    xor %rax, %rax

    # write syscall
    mov $1, %rax

    mov %rax, %rdi

    # Load relative address
    lea msg(%rip), %rsi

    # string length
    mov $0x27, %rdx
    syscall

    # Restore
    pop %r11
    pop %rdi
    pop %rsi
    pop %rdx
    pop %rcx
    pop %rax

    # Address to be replaced
    # when loading the "malware"
    # so we return control flow
    mov $0x1122334455667788, %rbx
    jmp *%rbx

    msg:
        .ascii "-= Objective reality doesn't exist! =-\n"

