#ifndef UTIL_H_
#define UTIL_H_

#include "stm32g4xx_hal.h"
#include "stm32g4xx.h"

#define SYSTEM_CLOCK_HZ     (170000000U)

#define DMAMUX_REQ_INPUT_ADC1             5U


#define roundedRightShift(type, value, shift)                                   \
            ((((shift) < (4U * (uint8_t)8)) && ((shift) > 0U)) ?                  \
            (((value) + ((type)1U << ((shift) - 1U))) >> (shift)) : (value))




#define ROUND_TO(type, val)                                                         \
    ((type) (((double)val < 0.0) ? ((double)(val) - 0.5) : (((double)val + 0.5))))


#define IS_BIT_SET(val, msk)  ((val & msk) != 0U)


#define modReg(v, clr_msk, set_msk)       \
    do {                                      \
        (v) = ((v) & ~(clr_msk)) | (set_msk); \
    } while (0)


static inline void assignDmaTransComplBits(uintptr_t dma_chan, uint32_t *isr_tcif_msk, uint32_t *ifcr_ctcif_msk)
{
    if(dma_chan != (uintptr_t)NULL)
    {
        switch(dma_chan)
        {
#if defined(DMA1_Channel1_BASE)
            case DMA1_Channel1_BASE:
                *isr_tcif_msk = DMA_ISR_TCIF1;
                *ifcr_ctcif_msk = DMA_IFCR_CTCIF1;
                break;
#endif
#if defined(DMA1_Channel2_BASE)
            case DMA1_Channel2_BASE:
                *isr_tcif_msk = DMA_ISR_TCIF2;
                *ifcr_ctcif_msk = DMA_IFCR_CTCIF2;
                break;
#endif
#if defined(DMA1_Channel3_BASE)
            case DMA1_Channel3_BASE:
                *isr_tcif_msk = DMA_ISR_TCIF3;
                *ifcr_ctcif_msk = DMA_IFCR_CTCIF3;
                break;
#endif
#if defined(DMA1_Channel4_BASE)
            case DMA1_Channel4_BASE:
                *isr_tcif_msk = DMA_ISR_TCIF4;
                *ifcr_ctcif_msk = DMA_IFCR_CTCIF4;
                break;
#endif
#if defined(DMA1_Channel5_BASE)
            case DMA1_Channel5_BASE:
                *isr_tcif_msk = DMA_ISR_TCIF5;
                *ifcr_ctcif_msk = DMA_IFCR_CTCIF5;
                break;
#endif
#if defined(DMA1_Channel6_BASE)
            case DMA1_Channel6_BASE:
                *isr_tcif_msk = DMA_ISR_TCIF6;
                *ifcr_ctcif_msk = DMA_IFCR_CTCIF6;
                break;
#endif
#if defined(DMA1_Channel7_BASE)
            case DMA1_Channel7_BASE:
                *isr_tcif_msk = DMA_ISR_TCIF7;
                *ifcr_ctcif_msk = DMA_IFCR_CTCIF7;
                break;
#endif
            default:
                break;
        }
    }
}
#endif /* UTIL_H_ */
