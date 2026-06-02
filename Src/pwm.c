#include "pwm.h"


#define TIM_PSC             (100U - 1U)
#define TIM_FREQ            ((double) 100.0)

#define TIM_PWM_PERIOD      ROUND_TO(uint32_t, ((double) SYSTEM_CLOCK_HZ/(TIM_FREQ*(double)(TIM_PSC+1U)) ))

#define TIM_PWM_DUTY_1     ROUND_TO(uint16_t , (double)TIM_PWM_PERIOD * 0.25)
#define TIM_PWM_DUTY_2     ROUND_TO(uint16_t , (double)TIM_PWM_PERIOD * 0.75)

/* Function prototype */
static void initTim3(void);

void initPwm(void)
{
    initTim3();
}

static void initTim3(void)
{
    TIM_TypeDef * const TIM_PWM = TIM3;

    // enable timer clock
    SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM3EN);

    TIM_PWM->PSC = TIM_PSC;       // prescale register
    // auto reload register value: 1000us / (1/system clock)
    TIM_PWM->ARR = TIM_PWM_PERIOD;

        /* Set CCRx to init value, which is minimum duty cycle value */
    TIM_PWM->CCR1 = TIM_PWM_DUTY_1;
    TIM_PWM->CCR2 = TIM_PWM_DUTY_2;

    /* Select PWM mode 1 on OC1 and OC2 (OC3M = 0b0110), enable preload register on OC3 (OC3PE = 1, reset value) */
    TIM_PWM->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 |TIM_CCMR1_OC1PE |
                      TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 |TIM_CCMR1_OC2PE;

    // auto-reload preload enable
    TIM_PWM->CR1 = TIM_CR1_ARPE;

    TIM_PWM->EGR = TIM_EGR_UG;

    // enable output on channel 1, 2
    SET_BIT(TIM_PWM->CCER, TIM_CCER_CC1E);
    SET_BIT(TIM_PWM->CCER, TIM_CCER_CC2E);

    SET_BIT(TIM_PWM->CR1, TIM_CR1_CEN);
}
