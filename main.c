/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Common example project which can run on all Mi-V soft processor variants.
 * Please refer README.TXT in the root folder of this project for more details.
 *
 */

#include "miv_rv32_hal.h"
#include "hal.h"
#include "hw_platform.h"
#include "core_gpio.h"

uint8_t External_30_IRQHandler()
{
    return(EXT_IRQ_KEEP_ENABLED);
}

uint8_t External_31_IRQHandler()
{
    return(EXT_IRQ_KEEP_ENABLED);
}

void Software_IRQHandler()
{
    MRV_clear_soft_irq();
}

void External_IRQHandler()
{
}

void MGEUI_IRQHandler(void)
{
}

void MGECI_IRQHandler(void)
{
}

void MSYS_EI0_IRQHandler(void)
{
}

void MSYS_EI1_IRQHandler(void)
{
}

void MSYS_EI2_IRQHandler(void)
{
}

void MSYS_EI3_IRQHandler(void)
{
}

void MSYS_EI4_IRQHandler(void)
{
}

void MSYS_EI5_IRQHandler(void)
{
}

void OPSRV_IRQHandler(void)
{
}

void SysTick_Handler(void)
{
}

int main(void)
{
    gpio_instance_t g_gpio_out;

    GPIO_init(&g_gpio_out, COREGPIO_OUT_BASE_ADDR, GPIO_APB_32_BITS_BUS);

    GPIO_config(&g_gpio_out, 0, GPIO_OUTPUT_MODE);
    GPIO_config(&g_gpio_out, 1, GPIO_OUTPUT_MODE);

    GPIO_set_outputs(&g_gpio_out, 0x0u);

    HAL_enable_interrupts();
    MRV_systick_config(SYS_CLK_FREQ);

    do {
        static uint32_t val = 0u;
        val ^= 0xFu;
        GPIO_set_outputs(&g_gpio_out, val);
    } while (1);

    return 0u;
}
