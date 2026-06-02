#include "adc.h"

#define ADC_TIMER_PSC       (1U - 1U)
#define ADC_SAMPLING_FREQ   ((double) 2000.0)

typedef struct {
    uint32_t isr_tcif;
    uint32_t ifcr_ctcif;
} dma_reg_t;

static dma_reg_t g_dma_reg_handle;
static uint32_t g_adc_dma_data[ADC_CH_COUNT];
AdcFilterStruct_t g_adcdata[ADC_CH_COUNT];

/* Function prototypes */
static void initAdcTimer(void);
static void initAdcMetering(void);

void initAdc(void)
{
    initAdcMetering();
    initAdcTimer();

}

void serviceAdc(void)
{
    DMA_TypeDef * const DMA_UNIT = DMA1; /* parasoft-suppress BD-PB-VOVR "It is intended to only write to HW register (never read) ->OK" */

    if(IS_BIT_SET(DMA_UNIT->ISR, g_dma_reg_handle.isr_tcif))
    {
        updateAdcFilter(); // add new adc data to accumulator
        SET_BIT(DMA_UNIT->IFCR, g_dma_reg_handle.ifcr_ctcif); // clear TCIF by setting CTCIF
    }
}

static inline void calibrateAdc(ADC_TypeDef * const adc_module)
{
    // Starting the ADC voltage regulator
    SET_BIT(adc_module->CR, ADC_CR_ADVREGEN);
    // tADCVREG_SETUP ADC voltage regulator start-up time - managed by software
    // 20us is required according data sheet - 100us for some margin
    HAL_Delay(1);
    // Initiating calibration
    SET_BIT(adc_module->CR, ADC_CR_ADCAL);
    // Waiting until calibration is complete
    while (IS_BIT_SET(adc_module->CR, ADC_CR_ADCAL))
    {
        //do nothing
    }
}

static void initAdcTimer(void)
{
    // pointer to apply only change of timer in the next 2 commands
    TIM_TypeDef * const ADC_HW_TIMER = TIM1;
    // Enable clock for TIM1
    setBit(RCC->APB2ENR, RCC_APB2ENR_TIM1EN);

    ADC_HW_TIMER->PSC = ADC_TIMER_PSC; // prescale register
    // auto reload register value: 1000us / (1/system clock)
    ADC_HW_TIMER->ARR = ROUND_TO(uint32_t, ((double)SYSTEM_CLOCK_HZ/(ADC_SAMPLING_FREQ*(double)(ADC_TIMER_PSC + 1U)) )); /* parasoft-suppress MISRAC2012-DIR_4_6-b "Calculation is carried out int double, but will be replaced by compiler with const." */
    ADC_HW_TIMER->CR2 = TIM_CR2_MMS2_1;  // master mode "Update" trigger output (TRGO2)
    /* Force update generation (UG = 1) */
    ADC_HW_TIMER->EGR = TIM_EGR_UG;
    ADC_HW_TIMER->CR1 = TIM_CR1_DIR | TIM_CR1_CEN;  // used as down counte
}

static void initAdcMetering(void)
{
    ADC_TypeDef * const ADC_REG = ADC1;
    ADC_Common_TypeDef * const ADC_COMMON_REG = ADC12_COMMON;
    DMA_Channel_TypeDef * const DMA_CH = DMA1_Channel1;
    DMAMUX_Channel_TypeDef * const DMAMUX_CH = DMAMUX1_Channel0;

    // Enable clock for ADC
    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_ADC12EN);

    // select system clock for peripheral
    modifyBits(RCC->CCIPR, RCC_CCIPR_ADC12SEL, RCC_CCIPR_ADC12SEL_1);

    // assign TC registers of specified dma
    assignDmaTransComplBits((uintptr_t)DMA_CH, &g_dma_reg_handle.isr_tcif, &g_dma_reg_handle.ifcr_ctcif);

    ADC_COMMON_REG->CCR =
       _VAL2FLD(ADC_CCR_PRESC, 0U) |    // prescale
       _VAL2FLD(ADC_CCR_CKMODE, 3U); // PCLK/4 (Synchronous clock mode)

    ADC_REG->CR = 0U;   // disable ADC

    calibrateAdc(ADC_REG);

    ADC_REG->CFGR  =
            _VAL2FLD(ADC_CFGR_AWD1CH, 0U)  |         // ADC analog input channel 0 monitored by AWD1 (available on ADC1 only)
            _VAL2FLD(ADC_CFGR_DISCEN, 0U)      |     // Discontinuous mode for regular channels disabled
            _VAL2FLD(ADC_CFGR_ALIGN, 0U)       |     // Right alignment
            _VAL2FLD(ADC_CFGR_AUTDLY, 0U)      |     // Auto-delayed conversion mode off
            _VAL2FLD(ADC_CFGR_CONT, 0U)        |     // Single conversion mode
            _VAL2FLD(ADC_CFGR_OVRMOD, 1U)      |     // ADC_DR register is preserved with the old data when an overrun is detected.
            _VAL2FLD(ADC_CFGR_EXTEN, 1U)       |     // Hardware trigger detection on the rising edge
            _VAL2FLD(ADC_CFGR_EXTSEL, 10U)     |     // External trigger selection for regular group: adc_ext_trg10, TIM1_TRGO2, 01010
            _VAL2FLD(ADC_CFGR_RES, 0U)         |     // 12-bit data resolution
            _VAL2FLD(ADC_CFGR_DMACFG, 1U)      |     // DMA circular mode
            _VAL2FLD(ADC_CFGR_DMAEN, 0U);            // DMA enabled

    // Regular sequence configuration for control

        ADC_REG->SQR1 =
            _VAL2FLD(ADC_SQR1_L, (ADC_CH_COUNT-1))   |    // number of channels
            _VAL2FLD(ADC_SQR1_SQ1, 16U)  |                 // AD_VREF_int
            _VAL2FLD(ADC_SQR1_SQ2, 18U)  |                 // AD_VTS_int
            _VAL2FLD(ADC_SQR1_SQ3, 16U) |                  // AD_VREF_int
            _VAL2FLD(ADC_SQR1_SQ4, 18U);                   // AD_VTS_int

         /* sample time configuration */
        ADC_REG->SMPR1 =
            _VAL2FLD(ADC_SMPR1_SMP6, 7U);

        ADC_REG->SMPR2 =
            _VAL2FLD(ADC_SMPR2_SMP16, 7U)|
            _VAL2FLD(ADC_SMPR2_SMP18, 7U);

        // Enable internal reference voltage.
        SET_BIT(ADC_COMMON_REG->CCR, ADC_CCR_VREFEN);

        DMA_CH->CPAR = (uintptr_t)(&(ADC_REG->DR));
        DMA_CH->CMAR = (uintptr_t)(&g_adc_dma_data[0]);
        DMA_CH->CNDTR = (uint32_t)(ADC_CH_COUNT);

        // Set up DMA transfer from ADC to program data memory.
        DMA_CH->CCR =
            _VAL2FLD(DMA_CCR_MEM2MEM, 0U) |
            _VAL2FLD(DMA_CCR_PL, 0U) |       // low priority
            _VAL2FLD(DMA_CCR_MSIZE, 2U) |    // 32 bit memory size
            _VAL2FLD(DMA_CCR_PSIZE, 2U) |    // 32 bit peripheral size
            _VAL2FLD(DMA_CCR_MINC, 1U) |     // memory increment enabled
            _VAL2FLD(DMA_CCR_PINC, 0U) |     // peripheral increment disabled
            _VAL2FLD(DMA_CCR_CIRC, 1U) |     // circular mode enabled
            _VAL2FLD(DMA_CCR_DIR, 0U);       // from peripheral to memory

        // assign ADC as multiplexer input to DMA-Channel1 acc. to Table 91
        modifyBits(DMAMUX_CH->CCR, DMAMUX_CxCR_DMAREQ_ID, DMAMUX_REQ_INPUT_ADC1);

        SET_BIT(ADC_REG->CFGR, ADC_CFGR_DMAEN);

        SET_BIT(ADC_REG->CR, ADC_CR_ADEN);      // Enable and start ADC after configuration

        while(!isBitSet(ADC_REG->ISR, ADC_ISR_ADRDY)) {}

        enableDma(DMA_CH);                 // activating dma-channel 1
        setBit(ADC_REG->CR, ADC_CR_ADSTART);
}

static inline void updateAdcFilter(void)
{
    for(uint8_t i = 0U; i < (uint8_t)ADC_CH_COUNT; i++)
    {
        AdcIndex_t adc_idx = (AdcIndex_t)i;
        g_adcdata[adc_idx].raw = (uint16_t)g_adc_dma_data[adc_idx];
        weightedAccumulation(g_adcdata[adc_idx].accum, g_adcdata[adc_idx].raw, ACCUM_WEIGHTING_SHIFT);
        g_adcdata[adc_idx].average = (uint16_t)roundedRightShift(uint32_t, g_adcdata[adc_idx].accum, ACCUM_WEIGHTING_SHIFT);
    }
}


