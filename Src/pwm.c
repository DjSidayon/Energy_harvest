#include "pwm.h"


#define TIM_PSC             (1U - 1U)
#define TIM_FREQ            ((double) 10e3)

#define TIM_PWM_PERIOD      ((uint32_t)((double) SYSTEM_CLOCK_HZ/(TIM_FREQ * (TIM_PSC+1U))))

#define TIM_PWM_DUTY_1     ROUND_TO(uint16_t , (double)TIM_PWM_PERIOD * 0.5)
#define TIM_PWM_PERIOD_90_PHASE     (TIM_PWM_PERIOD >> 2)

/* Function prototype */
static void initTim3(void);
static void initTimers(void);

void initPwm(void)
{
    initTimers();
}

static void initTimers(void)
{
    TIM_TypeDef * const TIM_3 = TIM3;
    TIM_TypeDef * const TIM_4 = TIM4;
    SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM3EN);
    SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM4EN);

    //TIM3_CH1 acts as master to trigger TIM4_CH3 to generate a 90° phase shift offset.
    /* TIM3 configuration */
    TIM_3->PSC = TIM_PSC;   //Prescale register
    TIM_3->ARR = TIM_PWM_PERIOD;
    TIM_3->CCR1 = TIM_PWM_DUTY_1;
    TIM_3->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 |TIM_CCMR1_OC1PE; //PWM mode 1
    TIM_3->CR2 = TIM_CR2_MMS_1; // TIM3 als Master: TRGO = update event
    TIM_3->CR1 |= TIM_CR1_ARPE; // auto-reload preload enable
    TIM_3->EGR = TIM_EGR_UG;
    SET_BIT(TIM_3->CCER, TIM_CCER_CC1E); // enable output on channel 1

    TIM_4->PSC = TIM_PSC;
    TIM_4->ARR = TIM_PWM_PERIOD;
    TIM_4->CCR3 = TIM_PWM_DUTY_1;
    TIM_4->CCMR2 = TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 |TIM_CCMR2_OC3PE; //PWM mode 1
    TIM_4->SMCR |= TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1 | TIM_SMCR_TS_0; //Trigger mode and ITR1(TIM3)
    TIM_4->CR1 |= TIM_CR1_ARPE; // auto-reload preload enable
    TIM_4->EGR = TIM_EGR_UG;
    TIM4->CNT = TIM_PWM_PERIOD_90_PHASE; //90° phase shift
    SET_BIT(TIM_4->CCER, TIM_CCER_CC3E); // enable output on channel 3

    SET_BIT(TIM_3->CR1, TIM_CR1_CEN);
    SET_BIT(TIM_4->CR1, TIM_CR1_CEN);
}
