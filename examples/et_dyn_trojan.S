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

/* 227 bytes */
.globl _start

.equ SYS_read,      0
.equ SYS_write,     1
.equ SYS_open,      2
.equ    MAXLEN,     17

.text

_start:
    # Save registers
    push %rax
    push %rcx
    push %rdx
    push %rsi
    push %rdi
    push %r11

    # Write out greetz
    movl $SYS_write, %eax
    movq $1, %rdi
    leaq msg(%rip), %rsi
    movq $msglen, %rdx
    syscall

    xor %rax, %rax

    // Open path in read-only
    movl $SYS_open, %eax
    leaq path(%rip), %rdi
    movq $0, %rsi
    syscall

    // Allocate buffer space
    subq $16, %rsp

    // Read the first MAXLEN bytes from path
    movq %rax, %rdi
    movl $SYS_read, %eax
    movq %rsp, %rsi
    movq $MAXLEN, %rdx
    syscall

    // scan the buffer for the fist '-' character
scan:
    // initialize counter
    movq $MAXLEN+1, %rcx
    // store the wild-card in %al byte
    movb $'-', %al
    movq %rsp, %rdi
    cld
    repne scasb

    // rcx = (MAXLEN-rcx)
    movq $MAXLEN, %r13
    subq %rcx, %r13
    movq %r13, %rcx

    // Prepare to read from
    // result buffer
    xor %rbx, %rbx
    movq %rsp, %rsi
    movq %rsi, %rdi
    cld
    // Convert the result buffer
    // into hex real values
readchars:
    lodsb
    cmpb $0x39, %al
    jle digit
alpha:
    subb $0x57, %al
    jmp load_ebx
digit:
    subb $0x30, %al
load_ebx:
    shl $4, %rbx
    or %rax, %rbx
    stosb
    loop readchars

    // At this point %rbx holds the offset

    // realign rsp
    addq $16, %rsp

    // Dummy jump address
    movq $0x1122334455667788, %r8

    // Add result offset into %r8
    addq %rbx, %r8

    # Restore
    pop %r11
    pop %rdi
    pop %rsi
    pop %rdx
    pop %rcx
    pop %rax

    // This is it
    jmpq *%r8

    path:   .asciz "/proc/self/maps"
    msg:    .asciz "-= Quantum junction get in both lanes =-\n"
    msglen= .-msg

