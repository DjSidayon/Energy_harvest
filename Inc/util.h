#ifndef UTIL_H_
#define UTIL_H_

#include "stm32g4xx_hal.h"
#include "stm32g4xx.h"

#define SYSTEM_CLOCK_HZ     (170000000U)

#define ROUND_TO(type, val)                                                         \
    ((type) (((double)val < 0.0) ? ((double)(val) - 0.5) : (((double)val + 0.5))))

#define IS_BIT_SET(val, msk)  ((val & msk) != 0U)

#endif /* UTIL_H_ */
