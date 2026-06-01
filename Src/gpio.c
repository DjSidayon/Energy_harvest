#include "gpio.h"



void initGpio(void)
{
    // Enable clocks for all GPIO ports and components we use.
    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOAEN |
                          RCC_AHB2ENR_GPIOBEN |
                          RCC_AHB2ENR_GPIOCEN |
                          RCC_AHB2ENR_GPIODEN |
                          RCC_AHB2ENR_GPIOFEN |
                          RCC_AHB2ENR_GPIOGEN);

    GPIOA->AFR[0] =       GPIO_AF7 (2) |      // USART2_TX
                          GPIO_AF7 (3);       // USART2_RX

    GPIOA->AFR[1] =       GPIO_AF13(8)  |     // HRTIM1_CHA1
                          GPIO_AF13(9)  |     // HRTIM1_CHA2
                          GPIO_AF13(10) |     // HRTIM1_CHB1
                          GPIO_AF13(11) |     // HRTIM1_CHB2
                          GPIO_AF0 (13) |     // SWDIO
                          GPIO_AF0 (14) |     // SWCLK
                          GPIO_AF1 (15);      // TIM2_CH1
    GPIOA->MODER =
            GPIO_ANALOG_MODE(0)     |   GPIO_ANALOG_MODE(1)     |   // ADC2_IN1             COMP1_INP
            GPIO_AF_MODE(2)         |   GPIO_AF_MODE(3)         |   // USART2_TX[STLK_TX]   USART2_RX[STLK_RX]
            GPIO_ANALOG_MODE(4)     |   GPIO_OUTPUT_MODE(5)     |   // DAC1_OUT1            LED_GREEN
            GPIO_ANALOG_MODE(6)     |   GPIO_OUTPUT_MODE(7)     |   // DAC2_OUT1            ADC2_IN4
            GPIO_AF_MODE(8)         |   GPIO_AF_MODE(9)         |   // HRTIM1_CHA1          HRTIM1_CHA2
            GPIO_AF_MODE(10)        |   GPIO_AF_MODE(11)        |   // HRTIM1_CHB1          HRTIM1_CHB2
            GPIO_OUTPUT_MODE(12)    |   GPIO_AF_MODE(13)        |   // nc.                  SWDIO
            GPIO_AF_MODE(14)        |   GPIO_AF_MODE(15);

}
