/*
 * Swamp - cooperative multitasking operating system
 * Copyright (c) 2016 rksdna
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <types.h>

extern u32_t stack_section_end;

extern void startup_handler(void);
extern void nmi_handler(void);
extern void fault_handler(void);
extern void svc_handler(void);
extern void pending_svc_handler(void);
extern void sys_tick_handler(void);
extern void null_handler(void);

__attribute__((naked))
static void irq_handler(void) { null_handler(); }

__attribute__ ((weak, alias("irq_handler")))
extern void irq0_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq1_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq2_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq3_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq4_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq5_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq6_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq7_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq8_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq9_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq10_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq11_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq12_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq13_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq14_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq15_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq16_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq17_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq18_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq19_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq20_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq21_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq22_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq23_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq24_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq25_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq26_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq27_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq28_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq29_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq30_handler(void);

__attribute__ ((weak, alias("irq_handler")))
extern void irq31_handler(void);

__attribute__ ((section(".startup")))
void * const startup_table[] =
{
    &stack_section_end,
    startup_handler,
    nmi_handler,
    fault_handler,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    svc_handler,
    0,
    0,
    pending_svc_handler,
    sys_tick_handler,
    irq0_handler,
    irq1_handler,
    irq2_handler,
    irq3_handler,
    irq4_handler,
    irq5_handler,
    irq6_handler,
    irq7_handler,
    irq8_handler,
    irq9_handler,
    irq10_handler,
    irq11_handler,
    irq12_handler,
    irq13_handler,
    irq14_handler,
    irq15_handler,
    irq16_handler,
    irq17_handler,
    irq18_handler,
    irq19_handler,
    irq20_handler,
    irq21_handler,
    irq22_handler,
    irq23_handler,
    irq24_handler,
    irq25_handler,
    irq26_handler,
    irq27_handler,
    irq28_handler,
    irq29_handler,
    irq30_handler,
    irq31_handler
};

__attribute__((naked))
u32_t __aeabi_uidivmod(u32_t num, u32_t den)
{
    asm volatile ("cmp r1, #0\n"
                  "bne label_no_div_by_zero\n"
                  "bx lr\n"
                  "label_no_div_by_zero:\n"
                  "movs r2, #1\n"
                  "movs r3, #0\n"
                  "cmp r0, r1\n"
                  "bls label_sub_loop\n"
                  "add r1, #0\n"
                  "bmi label_sub_loop\n"
                  "label_den_shift_loop:\n"
                  "lsl r2, #1\n"
                  "lsl r1, #1\n"
                  "bmi label_sub_loop\n"
                  "cmp r0, r1\n"
                  "bhi label_den_shift_loop\n"
                  "label_sub_loop:\n"
                  "cmp r0, r1\n"
                  "bcc label_dont_sub\n"
                  "sub r0, r1\n"
                  "orr r3, r2\n"
                  "label_dont_sub:\n"
                  "lsr r1, #1\n"
                  "lsr r2, #1\n"
                  "bne label_sub_loop\n"
                  "mov r1, r0\n"
                  "mov r0, r3\n"
                  "bx lr\n" : : : );
}

__attribute__ ((naked, alias("__aeabi_uidivmod")))
s32_t __aeabi_uidiv(s32_t num, s32_t den);

__attribute__((naked))
s32_t __aeabi_idivmod(s32_t num, s32_t den)
{
    asm volatile ("cmp r0, #0\n"
                  "bge label_num_pos\n"
                  "neg r0, r0\n"
                  "cmp r1, #0\n"
                  "bge label_neg_both\n"
                  "neg r1, r1\n"
                  "push {lr}\n"
                  "bl __aeabi_uidivmod\n"
                  "neg r1, r1\n"
                  "pop {pc}\n"
                  "label_num_pos:\n"
                  "cmp r1, #0\n"
                  "bge __aeabi_uidivmod\n"
                  "neg r1, r1\n"
                  "push {lr}\n"
                  "bl __aeabi_uidivmod\n"
                  "neg r0, r0\n"
                  "pop {pc}\n"
                  "label_neg_both:\n"
                  "push {lr}\n"
                  "bl __aeabi_uidivmod\n"
                  "neg r0, r0\n"
                  "neg r1, r1\n"
                  "pop {pc}\n" : : : );
}

__attribute__ ((naked, alias("__aeabi_idivmod")))
s32_t __aeabi_idiv(s32_t num, s32_t den);
