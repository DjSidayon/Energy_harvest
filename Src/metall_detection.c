#include "metall_detection.h"

#define RAD_TO_DEG  (57.29577951f)
#define BASELINE    (100.0f) //Needs to be calibrated


#define METAL_IRON_UPPER_RANGE      (20.0f)
#define METAL_IRON_LOWER_RANGE      (0.0f)

#define METAL_STEEL_UPPER_RANGE     (40.0f)
#define METAL_STEEL_LOWER_RANGE     (METAL_STEEL_UPPER_RANGE - 15.0f)

#define METAL_COPPER_UPPER_RANGE    (70.0f)
#define METAL_COPPER_LOWER_RANGE    (METAL_COPPER_UPPER_RANGE - 25.0f)

#define METAL_ALU_UPPER_RANGE    (80.0f)


typedef enum
{
    METAL_NONE = 0,
    METAL_IRON,
    METAL_STEEL,
    METAL_COPPER,
    METAL_ALUMINUM
}MetalType_t;
MetalType_t g_material = METAL_NONE;



void evaluateDetection(void)
{
    float in_phase = (float)getAdcAvg(ADC_IDX_IN_PHASE);
    float deg_90_phase = (float)getAdcAvg(ADC_IDX_90_DEG_PHASE);

    float magnitude = sqrt((in_phase * in_phase) + (deg_90_phase * deg_90_phase));
    float phase = atan2f(deg_90_phase, in_phase) * RAD_TO_DEG;

    if(magnitude > BASELINE + 150.0f)
    {
        // Material bestimmen
        if(phase < METAL_IRON_UPPER_RANGE){
            setGpioOutputValue(GPIOB, 3, true);
            setGpioOutputValue(GPIOB, 4, false);
            setGpioOutputValue(GPIOB, 5, false);
            setGpioOutputValue(GPIOB, 10, false);
            //g_material = METAL_IRON;
        }
        else if(phase < METAL_STEEL_UPPER_RANGE){
            setGpioOutputValue(GPIOB, 3, false);
            setGpioOutputValue(GPIOB, 4, false);
            setGpioOutputValue(GPIOB, 5, true);
            setGpioOutputValue(GPIOB, 10, false);
            //g_material = METAL_STEEL;
        }
        else if(phase < METAL_COPPER_UPPER_RANGE){
            setGpioOutputValue(GPIOB, 3, false);
            setGpioOutputValue(GPIOB, 4, true);
            setGpioOutputValue(GPIOB, 5, false);
            setGpioOutputValue(GPIOB, 10, false);
            //g_material = METAL_COPPER;
        }
        else{
            setGpioOutputValue(GPIOB, 3, false);
            setGpioOutputValue(GPIOB, 4, false);
            setGpioOutputValue(GPIOB, 5, false);
            setGpioOutputValue(GPIOB, 10, true);
            //g_material = METAL_ALUMINUM;
        }
    }
}
