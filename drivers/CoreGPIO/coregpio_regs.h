/*******************************************************************************
 * (c) Copyright 2009-2018 Microsemi SoC Products Group. All rights reserved.
 * 
 * SVN $Revision: 9743 $
 * SVN $Date: 2018-02-12 15:45:09 +0530 (Mon, 12 Feb 2018) $
 */
#ifndef __CORE_GPIO_REGISTERS_H
#define __CORE_GPIO_REGISTERS_H    1

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
 * 
 */
#define IRQ_REG_OFFSET          0x80

#define IRQ0_REG_OFFSET         0x80
#define IRQ1_REG_OFFSET         0x84
#define IRQ2_REG_OFFSET         0x88
#define IRQ3_REG_OFFSET         0x8C

/*------------------------------------------------------------------------------
 * 
 */
#define GPIO_IN_REG_OFFSET      0x90

#define GPIO_IN0_REG_OFFSET     0x90
#define GPIO_IN1_REG_OFFSET     0x94
#define GPIO_IN2_REG_OFFSET     0x98
#define GPIO_IN3_REG_OFFSET     0x9C

/*------------------------------------------------------------------------------
 * 
 */
#define GPIO_OUT_REG_OFFSET     0xA0

#define GPIO_OUT0_REG_OFFSET    0xA0
#define GPIO_OUT1_REG_OFFSET    0xA4
#define GPIO_OUT2_REG_OFFSET    0xA8
#define GPIO_OUT3_REG_OFFSET    0xAC

#ifdef __cplusplus
}
#endif

#endif /* __CORE_GPIO_REGISTERS_H */
