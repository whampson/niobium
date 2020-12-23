/*============================================================================*
 * Copyright (C) 2020-2021 Wes Hampson. All Rights Reserved.                  *
 *                                                                            *
 * This file is part of the OHWES Operating System.                           *
 * OHWES is free software; you may redistribute it and/or modify it under the *
 * terms of the license agreement provided with this software.                *
 *                                                                            *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    *
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    *
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        *
 * DEALINGS IN THE SOFTWARE.                                                  *
 *============================================================================*
 *    File: kernel/interrupt.c                                                *
 * Created: December 19, 2020                                                 *
 *  Author: Wes Hampson                                                       *
 *============================================================================*/

#include <ohwes/init.h>
#include <ohwes/kernel.h>
#include <ohwes/except.h>
#include <ohwes/interrupt.h>
#include <ohwes/syscall.h>
#include <ohwes/irq.h>
#include <ohwes/thunk.h>
#include <x86/desc.h>
#include <x86/cntrl.h>
#include <x86/flags.h>

static const ivt_thunk thunk_except[NUM_EXCEPT] =
{
    thunk_except_00,    thunk_except_01,    thunk_except_02,    thunk_except_03,
    thunk_except_04,    thunk_except_05,    thunk_except_06,    thunk_except_07,
    thunk_except_08,    thunk_except_09,    thunk_except_10,    thunk_except_11,
    thunk_except_12,    thunk_except_13,    thunk_except_14,    thunk_except_15,
    thunk_except_16,    thunk_except_17,    thunk_except_18,    thunk_except_19,
    thunk_except_20,    thunk_except_21,    thunk_except_22,    thunk_except_23,
    thunk_except_24,    thunk_except_25,    thunk_except_26,    thunk_except_27,
    thunk_except_28,    thunk_except_29,    thunk_except_30,    thunk_except_31
};

static const ivt_thunk thunk_irq[NUM_IRQ] =
{
    thunk_irq_00,       thunk_irq_01,       thunk_irq_02,       thunk_irq_03,
    thunk_irq_04,       thunk_irq_05,       thunk_irq_06,       thunk_irq_07,
    thunk_irq_08,       thunk_irq_09,       thunk_irq_10,       thunk_irq_11,
    thunk_irq_12,       thunk_irq_13,       thunk_irq_14,       thunk_irq_15
};

void idt_init(void)
{
    struct x86_desc *idt;
    struct x86_desc *desc;
    int count;

    idt = (struct x86_desc *) IDT_BASE;
    memset(idt, 0, IDT_SIZE);

    count = IDT_SIZE / sizeof(struct x86_desc);
    for (int idx = 0, e_num = 0, i_num = 0; idx < count; idx++) {
        e_num = idx - INT_EXCEPT;
        i_num = idx - INT_IRQ;
        desc = idt + idx;

        if (idx >= INT_EXCEPT && e_num < NUM_EXCEPT) {
            set_trap_desc(desc, KERNEL_CS, KERNEL_PL, thunk_except[e_num]);
        }
        if (idx >= INT_IRQ && i_num < NUM_IRQ) {
            set_intr_desc(desc, KERNEL_CS, KERNEL_PL, thunk_irq[i_num]);
        }
        if (idx == INT_SYSCALL) {
            set_trap_desc(desc, KERNEL_CS, USER_PL, thunk_syscall);
        }
    }

    struct descreg *idtr = (struct descreg *) IDT_REGPTR;
    idtr->base = IDT_BASE;
    idtr->limit = IDT_SIZE - 1;
    lidt(*idtr);
}

__fastcall void handle_except(struct iframe *regs)
{
    uint32_t cr0, cr2, cr3, cr4;
    struct eflags *eflags;

    rdcr0(cr0); rdcr2(cr2); rdcr3(cr3); rdcr4(cr4);
    eflags = (struct eflags *) &regs->eflags;

    printk("Exception 0x%02X!\n", regs->vec_num);
    printk("Error Code: %p\n", regs->err_code);
    printk("EAX=%p EBX=%p ECX=%p EDX=%p\n", regs->eax, regs->ebx, regs->ecx, regs->edx);
    printk("ESI=%p EDI=%p EBP=%p EIP=%p\n", regs->esi, regs->edi, regs->ebp, regs->eip);
    printk("CR0=%p CR2=%p CR3=%p CR4=%p\n", cr0, cr2, cr3, cr4);
    printk("CS=%02x IOPL=%d EFLAGS=%p [ ", regs->cs, eflags->iopl, eflags->_value);
    if (eflags->cf) printk("CF "); if (eflags->pf) printk("PF ");
    if (eflags->af) printk("AF "); if (eflags->zf) printk("ZF ");
    if (eflags->sf) printk("SF "); if (eflags->tf) printk("TF ");
    if (eflags->if_) printk("IF "); if (eflags->df) printk("DF ");
    if (eflags->of) printk("OF "); if (eflags->nt) printk("NT ");
    if (eflags->rf) printk("RF "); if (eflags->vm) printk("VM ");
    if (eflags->ac) printk("AC "); if (eflags->vif) printk("VIF ");
    if (eflags->vip) printk("VIP "); if (eflags->zf) printk("ID ");
    printk("]\n\n");
    panic("You done goofed!");
}

__fastcall void handle_irq(struct iframe *regs)
{
    printk("Device IRQ %d!\n", ~regs->vec_num);
}
