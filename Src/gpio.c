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

    //GPIOA->AFR[0] =       GPIO_AF2(6) |      // TIM3_CH1
    //                      GPIO_AF2(7);       // TIM3_CH2

    GPIOA->AFR[1] =       GPIO_AF0(13) |     // SWDIO
                          GPIO_AF0(14);      // SWCLK

    GPIOB->AFR[0] =       GPIO_AF5(6);      // TIM8_CH1
    GPIOB->AFR[1] =       GPIO_AF10(8);      // TIM8_CH2


    GPIOA->MODER =
            GPIO_ANALOG_MODE(0)     |   // ADC1_IN1 //does only reach 2050
            GPIO_ANALOG_MODE(1)     |   // ADC1_IN2
            //GPIO_ANALOG_MODE(2)     |   // ADC1_IN3
            //GPIO_ANALOG_MODE(3)     |   // ADC1_IN4
            //GPIO_AF_MODE(6)         |   // TIM3_CH1
            //GPIO_AF_MODE(7)         |   // TIM3_CH2
            GPIO_AF_MODE(13)        |   // SWDIO
            GPIO_AF_MODE(14);           // SWCLK

    GPIOB->MODER =
            GPIO_ANALOG_MODE(0) | //ADC1_IN15
            GPIO_AF_MODE(6) | //TIM8_CH1 (D10)
            GPIO_AF_MODE(8); //TIM8_CH2 (D15)
}
