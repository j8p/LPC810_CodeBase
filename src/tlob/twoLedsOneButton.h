#ifndef _TLOB_H_
#define _TLOB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LPC8xx.h"
#include "../amaf/event.h"
#include "../amaf/fsm.h"

void tlob_setup();

void tlob_toggleLedsState(uint32_t toggleLed1, uint32_t toggleLed2);

void tlob_setLedsState(uint32_t enableLed1, uint32_t enableLed2);

void tlob_disableButtonInterrupt();

#define TLOB_BUTTON_INTERRUPT_PRESS   (0)
#define TLOB_BUTTON_INTERRUPT_RELEASE (1)

void tlob_enableButtonInterrupt(uint32_t release);

uint32_t tlob_isButtonInterruptReleased(uint32_t clear);

uint32_t tlob_isButtonInterruptPressed(uint32_t clear);

#ifdef __cplusplus
}
#endif

#endif
