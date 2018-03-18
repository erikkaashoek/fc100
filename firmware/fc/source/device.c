/*
 * Reciprocal counter with vernier and serial peripheral interface
 * Copyright (c) 2018 rksdna, murych
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

#include <stm32/syscfg.h>
#include <stm32/usart.h>
#include <stm32/flash.h>
#include <stm32/gpio.h>
#include <stm32/nvic.h>
#include <stm32/rcc.h>
#include <stm32/des.h>
#include <stm32/spi.h>
#include <stm32/adc.h>
#include <stm32/tim.h>
#include <threads.h>
#include <timers.h>
#include <debug.h>
#include <tools.h>
#include "device.h"

static void debug_put(void *data, char value)
{
    while (~USART1->ISR & USART_ISR_TXE)
        continue;

    USART1->TDR = value;
}

struct stream debug_stream = {debug_put, 0};

void startup_device(void)
{
    FLASH->ACR = FLASH_ACR_LATENCY_48MHz | FLASH_ACR_PRFTBE;

    RCC->CR = RCC_CR_HSEON | RCC_CR_HSION;
    wait_for(&RCC->CR, RCC_CR_HSERDY, RCC_CR_HSERDY);

    RCC->CFGR = RCC_CFGR_PLLMUL12 | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLSRC_HSE_PREDIV;
    RCC->CR = RCC_CR_HSEON | RCC_CR_HSION | RCC_CR_PLLON;
    wait_for(&RCC->CR, RCC_CR_PLLRDY, RCC_CR_PLLRDY);

    RCC->CFGR = RCC_CFGR_PLLMUL12 | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLSRC_HSE_PREDIV | RCC_CFGR_SW_PLL;
    RCC->CFGR2 = 0;
    RCC->CFGR3 = 0;

    RCC->AHBENR = RCC_AHBENR_SRAMEN | RCC_AHBENR_FLITFEN | RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;
    RCC->APB1ENR = RCC_APB1ENR_USBEN | RCC_APB1ENR_TIM3EN;
    RCC->APB2ENR = RCC_APB2ENR_USART1EN | RCC_APB2ENR_TIM17EN | RCC_APB2ENR_ADCEN;

    GPIOA->ODR = 0;
    GPIOA->MODER =
            /* NC      */ GPIO_MODER_GPI(0) |
            /* NC      */ GPIO_MODER_GPI(1) |
            /* NC      */ GPIO_MODER_GPI(2) |
            /* MCU_RST */ GPIO_MODER_GPO(3) |
            /* MCU_NSS */ GPIO_MODER_AFO(4) |
            /* MCU_SCK */ GPIO_MODER_AFO(5) |
            /* MCU_SDI */ GPIO_MODER_AFO(6) |
            /* MCU_SDO */ GPIO_MODER_AFO(7) |
            /* NC      */ GPIO_MODER_GPI(8) |
            /* SYS_TXD */ GPIO_MODER_AFO(9) |
            /* SYS_RXD */ GPIO_MODER_GPI(10) |
            /* USB_D-  */ GPIO_MODER_GPI(11) |
            /* USB_D+  */ GPIO_MODER_GPI(12) |
            /* SYS_SWD */ GPIO_MODER_GPI(13) |
            /* SYS_SWC */ GPIO_MODER_GPI(14) |
            /* NC      */ GPIO_MODER_GPI(15);
    GPIOA->AFRL = 0;
    GPIOA->AFRH = GPIO_AFRH(9, 1);

    GPIOB->ODR = 0;
    GPIOB->MODER =
            /* NC      */ GPIO_MODER_GPI(0) |
            /* SUP_2V5 */ GPIO_MODER_ANA(1) |
            /* NC      */ GPIO_MODER_GPI(2) |
            /* NC      */ GPIO_MODER_GPI(3) |
            /* NC      */ GPIO_MODER_GPI(4) |
            /* NC      */ GPIO_MODER_GPI(5) |
            /* NC      */ GPIO_MODER_GPI(6) |
            /* PWM_PP1 */ GPIO_MODER_AFO(7) |
            /* NC      */ GPIO_MODER_GPI(8) |
            /* PWM_PP2 */ GPIO_MODER_AFO(9) |
            /* NC      */ GPIO_MODER_GPI(10) |
            /* NC      */ GPIO_MODER_GPI(11) |
            /* NC      */ GPIO_MODER_GPI(12) |
            /* NC      */ GPIO_MODER_GPI(13) |
            /* NC      */ GPIO_MODER_GPI(14) |
            /* NC      */ GPIO_MODER_GPI(15);
    GPIOB->AFRL = GPIO_AFRL(7, 2);
    GPIOB->AFRH = GPIO_AFRH(9, 2);

    USART1->CR1 = USART_CR1_UE | USART_CR1_TE;
    USART1->CR2 = 0;
    USART1->BRR = 48000000 / 115200 + 1;

    TIM3->PSC = 4800 - 1;
    TIM3->ARR = 200 - 1;
    TIM3->CR2 = TIM_CR2_MMS_1;
    TIM3->CR1 = TIM_CR1_CEN;

    TIM17->PSC = 8 - 1;
    TIM17->ARR = 10 - 1;
    TIM17->CCR1 = 5;
    TIM17->CCMR1 = TIM_CCMR1_OC1PE | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
    TIM17->CCER = TIM_CCER_CC1E | TIM_CCER_CC1NE;
    TIM17->BDTR = TIM_BDTR_MOE | 5;
    TIM17->CR1 = TIM_CR1_CEN;

    ADC1->CFGR1 = ADC_CFGR1_EXTEN_0 | ADC_CFGR1_EXTSEL_1 | ADC_CFGR1_EXTSEL_0;
    ADC1->CFGR2 = 0;
    ADC1->SMPR = ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2;
    ADC1->CHSELR = ADC_CHSELR_CHSEL9;

    ADC1->CR = ADC_CR_ADCAL;
    wait_for(&ADC1->CR, ADC_CR_ADCAL, 0);

    ADC1->IER = ADC_IER_EOCIE;
    NVIC->ISER = NVIC_ISER_SETENA_12;

    ADC1->CR = ADC_CR_ADEN;
    ADC1->CR = ADC_CR_ADSTART;

    start_timers_clock(48000);

    debug("hello\n");
    debug("id: %*m flash: %dKbytes\n", sizeof(DES->ID), DES->ID, DES->FSIZE & DES_FSIZE_FSIZE);
}

void irq12_handler(void)
{
    debug("%d\n", ADC1->DR * 330 / 2048);
    ADC1->ISR = ADC1->ISR;
}

