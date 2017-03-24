/* COPYRIGHT (c) 2016-2017 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#include "hal.h"
#include <core/stm32_crc/CRC.hpp>

#if defined(STM32F303xx)
  #include <core/stm32_crc/stm32f30x.hpp>
static CRC_TypeDef* _CRC = ((CRC_TypeDef*)CRC_BASE);
#elif defined(STM32F091xC)
  #include <core/stm32_crc/stm32f0xx.hpp>
static CRC_TypeDef* _CRC = ((CRC_TypeDef*)CRC_BASE);
#elif defined(STM32F407xx)
  #include <core/stm32_crc/stm32f4xx.hpp>
static CRC_TypeDef* _CRC = ((CRC_TypeDef*)CRC_BASE);
#else
  #error "Chip not supported"
#endif

namespace core {
namespace stm32_crc {

void
CRC::init()
{
#if defined(STM32F303xx)
    rccEnableAHB(RCC_AHBENR_CRCEN, FALSE);
#elif defined(STM32F091xC)
    rccEnableAHB(RCC_AHBENR_CRCEN, FALSE);
#elif defined(STM32F407xx)
    rccEnableAHB1(RCC_AHB1ENR_CRCEN, FALSE);
#endif

#if defined(STM32F303xx) || defined(STM32F091xC)
    /* Set DR register to reset value: 0xFFFFFFFF */
    _CRC->DR = 0xFFFFFFFF;
    /* Set the POL register to the reset value: 0x04C11DB7 */
    _CRC->POL = 0x04C11DB7;
    /* Reset IDR register */
    _CRC->IDR = 0x00;
    /* Set INIT register to reset value:  0xFFFFFFFF  */
    _CRC->INIT = 0xFFFFFFFF;
    /* Reset the CRC calculation unit */
    _CRC->CR = CRC_CR_RESET;
#elif defined(STM32F407xx)
    /* Reset IDR register */
    _CRC->IDR = 0x00;
    /* Reset the CRC calculation unit */
    _CRC->CR = CRC_CR_RESET;
#endif
} // CRC::init

void
CRC::reset()
{
    _CRC->CR |= CRC_CR_RESET;
}

#if defined(STM32F303xx) || defined(STM32F091xC)
void
CRC::setPolynomialSize(
    PolynomialSize size
)
{
    uint32_t tmpcr = 0;

/* Get CR register value */
    tmpcr = _CRC->CR;

/* Reset POL_SIZE bits */
    tmpcr &= (uint32_t) ~((uint32_t)CRC_CR_POLYSIZE);

/* Set the polynomial size */
    switch (size) {
      case PolynomialSize::POLY_7:
          tmpcr |= (uint32_t)CRC_CR_POLYSIZE;
          break;
      case PolynomialSize::POLY_8:
          tmpcr |= (uint32_t)CRC_CR_POLYSIZE_1;
          break;
      case PolynomialSize::POLY_16:
          tmpcr |= (uint32_t)CRC_CR_POLYSIZE_0;
          break;
      case PolynomialSize::POLY_32:
          tmpcr |= (uint32_t)0;
          break;
    }

/* Write to CR register */
    _CRC->CR = (uint32_t)tmpcr;
} // CRC::setPolynomialSize
#endif // if defined(STM32F303xx) || defined(STM32F091xC)

uint32_t
CRC::CRC32(
    uint32_t data
)
{
    _CRC->DR = data;

    return _CRC->DR;
}

#if defined(STM32F303xx) || defined(STM32F091xC)
uint32_t
CRC::CRC16(
    uint16_t data
)
{
    *(uint16_t*)(CRC_BASE) = (uint16_t)data;

    return _CRC->DR;
}

uint32_t
CRC::CRC8bits(
    uint8_t data
)
{
    *(uint8_t*)(CRC_BASE) = (uint8_t)data;

    return _CRC->DR;
}
#endif // if defined(STM32F303xx) || defined(STM32F091xC)

uint32_t
CRC::CRCBlock(
    uint32_t buffer[],
    uint32_t length
)
{
    for (uint32_t index = 0; index < length; index++) {
        _CRC->DR = buffer[index];
    }

    return _CRC->DR;
}

uint32_t
CRC::getCRC(
    void
)
{
    return _CRC->DR;
}
}
}
