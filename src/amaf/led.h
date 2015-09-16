#ifndef _LED_H_
#define _LED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LPC8xx.h"

#define LED_TIMING_FREQENCY (__SYSTEM_CLOCK / 60)

uint32_t led_resetTiming(uint32_t on);

uint32_t led_getNextTiming(uint32_t currentTiming);

uint32_t led_hasTiming(uint32_t currentTiming);

void led_waitTiming(uint32_t currentTiming);

#ifdef __cplusplus
}
#endif

#endif
