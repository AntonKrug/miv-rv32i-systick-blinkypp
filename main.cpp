/* SPDX-License-Identifier: MIT
 *
 * Revision history:
 *  2021/04/26 v1.1 anton.krug@gmail.com (first release)
 */

#include "miv_rv32_hal.h"
#include "hal.h"
#include "hw_platform.h"

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


namespace HAL {

    __attribute__((optimize("-O1"),always_inline))
    inline void set32bitReg(uint32_t registerAddress, uint32_t addressOffset, uint32_t value) {
        *(volatile uint32_t *)(registerAddress + addressOffset) = value;
    }

    __attribute__((optimize("-O1"),always_inline))
    inline void set16bitReg(uint32_t registerAddress, uint32_t addressOffset, uint16_t value)  {
        *(volatile uint16_t *)(registerAddress + addressOffset) = value;
    }

    __attribute__((optimize("-O1"),always_inline))
    inline void set8bitReg(uint32_t registerAddress, uint32_t addressOffset, uint8_t value)  {
        *(volatile uint8_t *)(registerAddress + addressOffset)  = value;
    }

}


namespace GPIO {

    enum class apbBusWidth_e: uint32_t {
        bits8        = 0,
        bits16       = 1,
        bits32       = 2,
        unknownWidth = 3
    };

    struct instance_s {
        uint32_t      baseAddress;
        apbBusWidth_e apbWidth;
        uint32_t      numberOfIOs;
    };

    enum class outOffset_e: uint32_t {
        offset0 = 0xA0,
        offset1 = 0xA4,
        offset2 = 0xA8,
        offset3 = 0xAC
    };

    enum class irqOffset_e: uint32_t {
        offset0 = 0x80,
        offset1 = 0x84,
        offset2 = 0x88,
        offset3 = 0x8C
    };

    enum class irqClearing_e: uint32_t {
        clearAll32 = 0xFFFFFFFF,
        clearAll16 = 0xFFFF,
        clearAll8  = 0xFF
    };

    enum class port_e:uint32_t {
        io0  = 0,
        io1  = 1,
        io2  = 2,
        io3  = 3,
        io4  = 4,
        io5  = 5,
        io6  = 6,
        io7  = 7,
        io8  = 8,
        io9  = 9,
        io10 = 10,
        io11 = 11,
        io12 = 12,
        io13 = 13,
        io14 = 14,
        io15 = 15,
        io16 = 16,
        io17 = 17,
        io18 = 18,
        io19 = 19,
        io20 = 20,
        io21 = 21,
        io22 = 22,
        io23 = 23,
        io24 = 24,
        io25 = 25,
        io26 = 26,
        io27 = 27,
        io28 = 28,
        io29 = 29,
        io30 = 30,
        io31 = 31
    };

    enum class portMode_e:uint32_t {
        input  = 0b010,
        output = 0b101,
        inout  = 0b011
    };

    template<uint32_t baseAddress, apbBusWidth_e apbWidth, uint32_t numberOfIOs>
    constexpr instance_s makeInstance() {
        // Check for valid base address
        static_assert((baseAddress % 4) == 0,                                          "The GPIO base port address needs to be 32-bit aligned");
        static_assert((baseAddress >= 0x7000'0000UL) && (baseAddress < 0x8000'2000UL), "The GPIO has to be within the APB bus range");

        // Check for correct bus width
        static_assert(apbWidth != apbBusWidth_e::unknownWidth,                         "The APB bus width has to be 8-bit, 16-bit or 32-bit");

        // Check for valid number of IOs
        static_assert(numberOfIOs>0 && numberOfIOs<=32,                                "The number of IOs in this GPIO port_s needs to be between 1-32");

        return {baseAddress, apbWidth, numberOfIOs};
    }

    template<const instance_s* thisGpio>
    void init() {
        // Clear previous configuration
        for (uint32_t i = 0; i < thisGpio->numberOfIOs; i++ ) {
            HAL::set8bitReg(thisGpio->baseAddress, i * 4, 0);
        }

        // Clear any pending interrupts
        switch(thisGpio->apbWidth) {
            case apbBusWidth_e::bits32:
                HAL::set32bitReg(thisGpio->baseAddress, static_cast<uint32_t>(irqOffset_e::offset0), static_cast<uint32_t>(irqClearing_e::clearAll32));
                break;

            case apbBusWidth_e::bits16:
                HAL::set16bitReg(thisGpio->baseAddress, static_cast<uint32_t>(irqOffset_e::offset0), static_cast<uint16_t>(irqClearing_e::clearAll16));
                HAL::set16bitReg(thisGpio->baseAddress, static_cast<uint32_t>(irqOffset_e::offset1), static_cast<uint16_t>(irqClearing_e::clearAll16));
                break;

            case apbBusWidth_e::bits8:
                HAL::set8bitReg( thisGpio->baseAddress, static_cast<uint32_t>(irqOffset_e::offset0), static_cast<uint8_t>(irqClearing_e::clearAll8));
                HAL::set8bitReg( thisGpio->baseAddress, static_cast<uint32_t>(irqOffset_e::offset1), static_cast<uint8_t>(irqClearing_e::clearAll8));
                HAL::set8bitReg( thisGpio->baseAddress, static_cast<uint32_t>(irqOffset_e::offset2), static_cast<uint8_t>(irqClearing_e::clearAll8));
                HAL::set8bitReg( thisGpio->baseAddress, static_cast<uint32_t>(irqOffset_e::offset3), static_cast<uint8_t>(irqClearing_e::clearAll8));
                break;

            default:
                // we asserted this when we got the instance_s, this shouldn't be reached
                break;
        }
    }

    template<const instance_s* thisGpio, port_e portId>
    void config(portMode_e mode) {
        static_assert(thisGpio->numberOfIOs > static_cast<uint32_t>(portId), "You are trying to access IOs above your numberOfIos configured in the instance");

        HAL::set32bitReg(thisGpio->baseAddress, static_cast<uint32_t>(portId)*4, static_cast<uint32_t>(mode));
    }

    template<const instance_s* thisGpio>
    void set(uint32_t value) {
        switch(thisGpio->apbWidth) {
            case apbBusWidth_e::bits32:
                HAL::set32bitReg(thisGpio->baseAddress, static_cast<uint32_t>(outOffset_e::offset0), value);
                break;

            case apbBusWidth_e::bits16:
                HAL::set16bitReg(thisGpio->baseAddress, static_cast<uint32_t>(outOffset_e::offset1), (value));
                HAL::set16bitReg(thisGpio->baseAddress, static_cast<uint32_t>(outOffset_e::offset2), (value >> 16));
                break;

            case apbBusWidth_e::bits8:
                HAL::set8bitReg(thisGpio->baseAddress, static_cast<uint32_t>(outOffset_e::offset0), (value));
                HAL::set8bitReg(thisGpio->baseAddress, static_cast<uint32_t>(outOffset_e::offset1), (value >> 8));
                HAL::set8bitReg(thisGpio->baseAddress, static_cast<uint32_t>(outOffset_e::offset2), (value >> 16));
                HAL::set8bitReg(thisGpio->baseAddress, static_cast<uint32_t>(outOffset_e::offset3), (value >> 24));
                break;

            default:
                // we asserted this when we got the instance_s, this shouldn't be reached
                break;
        }
    }

}


constexpr auto gpioOut = GPIO::makeInstance<0x7000'5000UL, GPIO::apbBusWidth_e::bits32, 2>();


int main(void) {
    GPIO::init<&gpioOut>();

    GPIO::config<&gpioOut, GPIO::port_e::io0>(GPIO::portMode_e::output);
    GPIO::config<&gpioOut, GPIO::port_e::io1>(GPIO::portMode_e::output);

    GPIO::set<&gpioOut>(0u);

    HAL_enable_interrupts();
    MRV_systick_config(SYS_CLK_FREQ);

    while (1) {
        static uint32_t val = 0u;
        val ^= 0xFu;
        GPIO::set<&gpioOut>(val);
    }

    return 0u;
}
