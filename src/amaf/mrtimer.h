#ifndef _MRTMR_H_
#define _MRTMR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LPC8xx.h"

#define TIMER_1 (0)
#define TIMER_2 (1)
#define TIMER_3 (2)
#define TIMER_4 (3)

void mrtmr_setupMrtDelay();

void mrtmr_enableMrtDelay(uint32_t channel, uint32_t delay);

void mrtmr_enableMrtRepeatingDelay(uint32_t channel, uint32_t delay);

void mrtmr_disableMrtDelay(uint32_t channel);

#ifdef __cplusplus
}
#endif

#endif
