#ifndef ADC_H_
#define ADC_H_

#include "util.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * AD Channels list
 * ------------------
 * IN6  AIN6_ADC_IDX, PC0,
 * IN16 AD_TEMP_MCU
 * IN18 AD_VREFINT
 */
typedef enum {
    //AIN6_ADC_IDX,
    TEMP_MCU_IDX,
    VREFINT_IDX,
    TEMP_MCU_IDX_2,
    VREFINT_IDX_2,
    ADC_CH_COUNT
} AdcIndex_t;

typedef struct {
    uint16_t average;
    uint16_t raw;
    uint32_t accum;
} AdcFilterStruct_t;
extern AdcFilterStruct_t g_adcdata[ADC_CH_COUNT];


#define ACCUM_WEIGHTING_SHIFT       (3U) // running average, 8 values accumulator

/* Function prototypes */
void initAdc(void);
void serviceAdc(void);


#endif /* ADC_H_ */
