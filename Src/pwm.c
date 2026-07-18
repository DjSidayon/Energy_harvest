#include "pwm.h"


#define TIM_PSC             (1U - 1U)
#define TIM_FREQ            ((double) 10e3)

//#define TIM_PWM_PERIOD      ROUND_TO(uint32_t, ((double) SYSTEM_CLOCK_HZ/(2*TIM_FREQ*(double)(TIM_PSC+1U)) ))
#define TIM_PWM_PERIOD      ((uint32_t)((double) SYSTEM_CLOCK_HZ/(TIM_FREQ * (TIM_PSC+1U))))

#define TIM_PWM_DUTY_1     ROUND_TO(uint16_t , (double)TIM_PWM_PERIOD * 0.5)
#define TIM_PWM_PERIOD_90_PHASE     (TIM_PWM_PERIOD >> 2)
#define TIM_PWM_DUTY_2     ROUND_TO(uint16_t, TIM_PWM_DUTY_1 + TIM_PWM_PERIOD_90_PHASE) //90° Verschiebung

/* Function prototype */
static void initTim3(void);
static void initTimers(void);

void initPwm(void)
{
    //initTim3();
    initTimers();
}

static void initTimers(void)
{
    TIM_TypeDef * const TIM_3 = TIM3;
    TIM_TypeDef * const TIM_4 = TIM4;
    SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM3EN);
    SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM4EN);

    TIM_3->PSC = TIM_PSC;   //Prescale register
    TIM_4->PSC = TIM_PSC;

    TIM_3->ARR = TIM_PWM_PERIOD;
    TIM_4->ARR = TIM_PWM_PERIOD;

    TIM_3->CCR1 = TIM_PWM_DUTY_1;
    TIM_4->CCR3 = TIM_PWM_DUTY_1;

    //PWM mode 1
    TIM_3->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 |TIM_CCMR1_OC1PE;
    TIM_4->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 |TIM_CCMR2_OC3PE;

    SET_BIT(TIM_3->CR2, TIM_CR2_MMS_1); // TIM3 als Master: TRGO = update event
    TIM_4->SMCR |= TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1 | TIM_SMCR_TS_0; //Trigger mode and ITR1(TIM3)


    // auto-reload preload enable
    TIM_3->CR1 |= TIM_CR1_ARPE;
    TIM_3->EGR = TIM_EGR_UG;
    TIM_4->CR1 |= TIM_CR1_ARPE;
    TIM_4->EGR = TIM_EGR_UG;

    TIM4->CNT = TIM_PWM_PERIOD_90_PHASE; //90° phase shift
    // enable output on channel 1, 2
    SET_BIT(TIM_3->CCER, TIM_CCER_CC1E);
    SET_BIT(TIM_3->CR1, TIM_CR1_CEN);

    SET_BIT(TIM_4->CCER, TIM_CCER_CC3E);
    SET_BIT(TIM_4->CR1, TIM_CR1_CEN);
}

static void initTim3(void)
{
    //TIM_TypeDef * const TIM_PWM = TIM3;
    TIM_TypeDef * const TIM_PWM = TIM8;

    // enable timer clock
    //SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM3EN);
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM8EN);


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
    TIM_PWM->CR1 |= TIM_CR1_ARPE;

    // Center-aligned mode 1 (CMS = 01) --> interrupt flags of output compare set when counting down.
    TIM_PWM->CR1 |= TIM_CR1_CMS_0;

    TIM_PWM->EGR = TIM_EGR_UG;

    /* Enable main output (MOE) — required for TIM1/TIM8 */
    TIM_PWM->BDTR |= TIM_BDTR_MOE;

    // enable output on channel 1, 2
    SET_BIT(TIM_PWM->CCER, TIM_CCER_CC1E);
    SET_BIT(TIM_PWM->CCER, TIM_CCER_CC2E);

    SET_BIT(TIM_PWM->CR1, TIM_CR1_CEN);
}
