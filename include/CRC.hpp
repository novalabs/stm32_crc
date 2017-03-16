/* COPYRIGHT (c) 2016-2017 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <cstddef>
#include <stdint.h>
#include <core/os/Mutex.hpp>

#undef CRC

namespace core {
namespace stm32_crc {
class CRC
{
public:
    static void
    init();

    static void
    reset();


#if defined(STM32F303xx) || defined(STM32F091xC)
    enum class PolynomialSize {
        POLY_7,
        POLY_8,
        POLY_16,
        POLY_32,
    };

    static void
    setPolynomialSize(
        PolynomialSize size
    );
#endif

    static uint32_t
    CRC32(
        uint32_t data
    );


#if defined(STM32F303xx) || defined(STM32F091xC)
    static uint32_t
    CRC16(
        uint16_t data
    );

    static uint32_t
    CRC8bits(
        uint8_t data
    );
#endif

    static uint32_t
    CRCBlock(
        uint32_t buffer[],
        uint32_t length
    );

    static uint32_t
    getCRC(
        void
    );

    static
    void
    lock();

    static
    void
    unlock();


private:
    static core::os::Mutex _lock;
};
}
}
