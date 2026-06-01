#ifndef GPIO_UTIL_H_
#define GPIO_UTIL_H_

#include <stdbool.h>
#include <stdint.h>
/************************************MODER*************************************************/
#define GPIO_MODER_MSK(mode, pin)       moderMsk((mode), (pin))

static inline uint32_t moderMsk(const uint32_t mode, const uint32_t pin)
{
    uint32_t shift = (2U * pin);
    uint32_t result = 0U;

    if (shift < (4U * (uint8_t)8))
    {
        result = (mode << shift);
    }

    return result;
}

#define GPIO_INPUT_MODE(pin)        GPIO_MODER_MSK(0U, (pin))
#define GPIO_OUTPUT_MODE(pin)       GPIO_MODER_MSK(1U, (pin))
#define GPIO_AF_MODE(pin)           GPIO_MODER_MSK(2U, (pin))
#define GPIO_ANALOG_MODE(pin)       GPIO_MODER_MSK(3U, (pin))
/**************************************************************************************/

/***********************************O_TYPER*********************************************/
#define GPIO_OTYPER_MSK(mode, pin)      otyperMsk((mode), (pin))
#define GPIO_PUSH_PULL(pin)             GPIO_OTYPER_MSK(0U, (pin))
#define GPIO_OPEN_DRAIN(pin)            GPIO_OTYPER_MSK(1U, (pin))

static inline uint32_t otyperMsk(const uint32_t mode, const uint32_t pin)
{
    uint32_t result = 0U;

    if (pin < (4U * (uint8_t)8))
    {
        result = (mode << pin);
    }

    return result;
}
/**************************************************************************************/

/*************************************O_SPEED*******************************************/
#define GPIO_OSPEEDR_MSK(mode, pin)     ospeedrMsk((mode), (pin))

static inline uint32_t ospeedrMsk(const uint32_t mode, const uint32_t pin)
{
    uint32_t shift = (2U * pin);/* parasoft-suppress PULS-07 "Definition of macro does not make sense for this single used shift operator" */
    uint32_t result = 0U;

    if (shift < (4U * (uint8_t)8))
    {
        result = (mode << shift);
    }

    return result;
}
/**************************************************************************************/

/***********************************PULL UP/DOWN*******************************************/
#define GPIO_PUPDR_MSK(mode, pin)       pupdMsk((mode), (pin))

#define GPIO_NO_PULL_UP(pin)            GPIO_PUPDR_MSK(0U, (pin))
#define GPIO_PULL_UP(pin)               GPIO_PUPDR_MSK(1U, (pin))
#define GPIO_PULL_DOWN(pin)             GPIO_PUPDR_MSK(2U, (pin))

static inline uint32_t pupdMsk(const uint32_t mode, const uint32_t pin)
{
    uint32_t shift = (2U * pin);
    uint32_t result = 0U;

    if (shift < (4U * (uint8_t)8))
    {
        result = (mode << shift);
    }

    return result;
}
/**************************************************************************************/

/**************************************AFR************************************************/
#define GPIO_AFR_MSK(mode, pin)         afrMsk((mode), (pin))

static inline uint32_t afrMsk(const uint32_t mode, const uint32_t pin)
{
    uint32_t shift = (4U * ((pin < 8U) ? pin : (pin - 8U)));
    uint32_t result = 0U;

    if (shift < (4U * (uint8_t)8))
    {
        result = (mode << shift);
    }

    return result;
}

#define GPIO_AF0(pin)               GPIO_AFR_MSK(0U, (pin))
#define GPIO_AF1(pin)               GPIO_AFR_MSK(1U, (pin))
#define GPIO_AF2(pin)               GPIO_AFR_MSK(2U, (pin))
#define GPIO_AF3(pin)               GPIO_AFR_MSK(3U, (pin))
#define GPIO_AF4(pin)               GPIO_AFR_MSK(4U, (pin))
#define GPIO_AF5(pin)               GPIO_AFR_MSK(5U, (pin))
#define GPIO_AF6(pin)               GPIO_AFR_MSK(6U, (pin))
#define GPIO_AF7(pin)               GPIO_AFR_MSK(7U, (pin))
#define GPIO_AF8(pin)               GPIO_AFR_MSK(8U, (pin))
#define GPIO_AF9(pin)               GPIO_AFR_MSK(9U, (pin))
#define GPIO_AF10(pin)              GPIO_AFR_MSK(10U, (pin))
#define GPIO_AF11(pin)              GPIO_AFR_MSK(11U, (pin))
#define GPIO_AF12(pin)              GPIO_AFR_MSK(12U, (pin))
#define GPIO_AF13(pin)              GPIO_AFR_MSK(13U, (pin))
#define GPIO_AF14(pin)              GPIO_AFR_MSK(14U, (pin))
#define GPIO_AF15(pin)              GPIO_AFR_MSK(15U, (pin))
/**************************************************************************************/

/************************************FUNCTIONS********************************************/
static inline void setGpioOutput( GPIO_TypeDef* const port, const uint32_t pin)
{
    if (pin < (4U * (uint8_t)8))
    {
        port->BSRR = (1UL << pin);
    }
}


static inline void resetGpioOutput(GPIO_TypeDef* const port, const uint32_t pin)
{
    if (pin < (4U * (uint8_t)8))
    {
        port->BRR = (1UL << pin);
    }
}


static inline void setGpioOutputValue(GPIO_TypeDef* const port, const uint32_t pin, const bool on)
{
    (on) ? setGpioOutput(port, pin) : resetGpioOutput(port, pin);
}


static inline void toggleGpioOutput(GPIO_TypeDef* const port, const uint32_t pin)
{
    if (pin < (4U * (uint8_t)8))
    {
        setGpioOutputValue(port, pin, !((port->ODR & (1UL << pin)) > 0U));
    }
}


#define getGpioInputValue(port, pin) (((port)->IDR >> (pin)) & (1U))
#define getGpioOutputValue(port, pin) (((port)->ODR >> (pin)) & (1U))

#endif /* GPIO_UTIL_H_ */
