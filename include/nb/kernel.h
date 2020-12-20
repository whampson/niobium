/*============================================================================*
 * Copyright (C) 2020 Wes Hampson. All Rights Reserved.                       *
 *                                                                            *
 * This file is part of the Niobium Operating System.                         *
 * Niobium is free software; you may redistribute it and/or modify it under   *
 * the terms of the license agreement provided with this software.            *
 *                                                                            *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    *
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    *
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        *
 * DEALINGS IN THE SOFTWARE.                                                  *
 *============================================================================*
 *    File: include/nb/kernel.h                                               *
 * Created: December 13, 2020                                                 *
 *  Author: Wes Hampson                                                       *
 *============================================================================*/

#ifndef __KERNEL_H
#define __KERNEL_H

#include <stdio.h>

#define KERNEL_CS               0x10    /* Kernel Code Segment */
#define KERNEL_DS               0x18    /* Kernel Data Segment */
#define USER_CS                 0x23    /* User-space Code Segment */
#define USER_DS                 0x2B    /* User-space Data Segment */
#define TSS_SEG                 0x30    /* TSS Segment */
#define LDT_SEG                 0x38    /* LDT Segment */

/**
 * Prints a message to the kernel console.
 */
#define printk(...) printf(__VA_ARGS__)

/**
 * Uh oh, something bad happened!
 * Prints a message then halts the system.
 */
#define panic(x)                \
do {                            \
    printk("KERNEL PANIC: " x); \
    for (;;);                   \
} while (0)

/* main.c */
void gdt_init(void);
void ldt_init(void);
void tss_init(void);

/* console.c */
void con_init(void);

/* memory.c */
void mem_init(void);

/* interrupt.c */
void idt_init(void);

#endif /* __KERNEL_H */
