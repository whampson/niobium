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
 *    File: drivers/vga/vga.c                                                 *
 * Created: December 14, 2020                                                 *
 *  Author: Wes Hampson                                                       *
 *============================================================================*/

#include <drivers/vga.h>
#include <nb/interrupt.h>
#include <nb/io.h>

void vga_init(void)
{
    /* ensure Color Text Mode ports are being used by setting IOAS */
    outb(VGA_PORT_EXTL_MO_W, inb(VGA_PORT_EXTL_MO_R) | VGA_FLD_EXTL_MO_IOAS);
}

uint8_t vga_crtc_read(uint8_t reg)
{
    /* TODO: delay? */

    int flags;
    uint8_t data;

    cli_save(flags);
    outb(VGA_PORT_CRTC_ADDR, reg);
    data = inb(VGA_PORT_CRTC_DATA);
    restore_flags(flags);

    return data;
}

void vga_crtc_write(uint8_t reg, uint8_t data)
{
    /* TODO: delay? */

    int flags;

    cli_save(flags);
    outb(VGA_PORT_CRTC_ADDR, reg);
    outb(VGA_PORT_CRTC_DATA, data);
    restore_flags(flags);
}

uint8_t vga_attr_read(uint8_t reg)
{
    /* TODO: delay? */

    int flags;
    uint8_t addr = reg & VGA_FLD_ATTR_ADDR_ADDR;
    uint8_t data;
    
    cli_save(flags);
    (void) inb(VGA_PORT_EXTL_IS1);
    outb(VGA_PORT_ATTR_ADDR, VGA_FLD_ATTR_ADDR_PAS | addr);
    data = inb(VGA_PORT_ATTR_DATA_R);
    restore_flags(flags);

    return data;
}

void vga_attr_write(uint8_t reg, uint8_t data)
{
    /* TODO: delay? */

    int flags;
    uint8_t addr = reg & VGA_FLD_ATTR_ADDR_ADDR;
    
    cli_save(flags);
    (void) inb(VGA_PORT_EXTL_IS1);
    outb(VGA_PORT_ATTR_ADDR, VGA_FLD_ATTR_ADDR_PAS | addr);
    outb(VGA_PORT_ATTR_DATA_W, data);
    restore_flags(flags);
}
