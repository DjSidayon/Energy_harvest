#ifndef ADC_H_
#define ADC_H_

#include "util.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * AD Channels list
 * ------------------
 * IN1 ADC_CH1
 * IN2 ADC_CH2
 */
typedef enum {
    ADC_IDX_IN_PHASE,
    ADC_IDX_90_DEG_PHASE,
    ADC_CH_COUNT
} AdcIndex_t;

typedef struct {
    uint16_t average;
    uint16_t raw;
    uint32_t accum;
} AdcFilterStruct_t;
extern AdcFilterStruct_t g_adcdata[ADC_CH_COUNT];


#define ACCUM_WEIGHTING_SHIFT       (3U) // running average, 8 values accumulator

#define getAdcAvg(idx)   (g_adcdata[(idx)].average)

/* Function prototypes */
void initAdc(void);
void serviceAdc(void);


#endif /* ADC_H_ */
