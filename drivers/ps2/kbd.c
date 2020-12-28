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
 *    File: drivers/ps2/kbd.c                                                 *
 * Created: December 25, 2020                                                 *
 *  Author: Wes Hampson                                                       *
 *============================================================================*/

#include <stddef.h>
#include <drivers/ps2.h>
#include <ohwes/kernel.h>

void kbd_init(void)
{
    uint8_t ps2cfg;

    ps2_cmd(PS2_CMD_RDCFG);
    ps2cfg = ps2_inb();
    ps2cfg |= PS2_CFG_P1INTON;
    ps2_cmd(PS2_CMD_WRCFG);
    ps2_outb(ps2cfg);
    ps2_cmd(PS2_CMD_P1ON);
    kbd_cmd(KBD_CMD_SCANOFF, NULL, 0);
}

bool kbd_test(void)
{
    kbd_cmd(KBD_CMD_SELFTEST, NULL, 0);
    return ps2_inb() == KBD_RES_PASS;
}

int kbd_cmd(uint8_t cmd, uint8_t *data, size_t n)
{
    const int NUM_RETRIES = 3;
    uint8_t res;
    int i, r;

    i = 0; r = 0;
    while (r++ < NUM_RETRIES)
    {
        ps2_outb(cmd);
        goto check_resp;

    send_data:
        if (i >= (int) n) break;
        ps2_outb(data[i++]);

    check_resp:
        switch (res = ps2_inb()) {
            case KBD_RES_ACK:
                goto send_data;
            case KBD_RES_RESEND:
                continue;
            default:
                /* unexpected result */
                return res;
        }
    }

    if (i == NUM_RETRIES) {
        return -1;
    }

    return 0;
}
