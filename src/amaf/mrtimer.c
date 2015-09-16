#include "mrtimer.h"
#include "event.h"
#include "fsm.h"

#define MRT_FORCE_LOAD_IMMEDIATELY (0x1UL<<31)
#define MRT_INTERRUPT_ON  (1)
#define MRT_INTERRUPT_OFF (0)
#define MRT_INTTERUPT_MODE   (1)
#define MRT_INTTERUPT_MODE_REPEAT (0)
#define MRT_INTTERUPT_MODE_ONCE   (1)
#define MRT_TIMER_IDLE (0xFFFFFF)

uint32_t TimerEvents[] = {TIMER1_EVENT, TIMER2_EVENT, TIMER3_EVENT, TIMER4_EVENT};


void MRT_IRQHandler(void)
{
	uint32_t i;
	for (i = 0; i < 4; i++)
	{
	  if ((LPC_MRT->Channel[i].STAT & 1) && (LPC_MRT->Channel[i].CTRL & 1)) {
		  LPC_MRT->Channel[i].STAT = 1;
		  fsm_sendEvent(TimerEvents[i]);
		  return;
	  }
  }

  return;
}

void mrtmr_setupMrtDelay()
{
	  LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 10);
	  LPC_SYSCON->PRESETCTRL &= ~(1 << 7);
	  LPC_SYSCON->PRESETCTRL |= (1 << 7);
}

void mrtmr_enableMrtDelay(uint32_t channel, uint32_t delay)
{
	if (channel > 3)
	{
		// reset...
		return;
	}
	  LPC_MRT->Channel[channel].INTVAL = delay;
	  LPC_MRT->Channel[channel].INTVAL |= MRT_FORCE_LOAD_IMMEDIATELY;
	  LPC_MRT->Channel[channel].CTRL |= (MRT_INTTERUPT_MODE_ONCE << MRT_INTTERUPT_MODE) | MRT_INTERRUPT_ON;
}

void mrtmr_enableMrtRepeatingDelay(uint32_t channel, uint32_t delay)
{
	if (channel > 3)
	{
		// reset...
		return;
	}
	  LPC_MRT->Channel[channel].INTVAL = delay;
	  LPC_MRT->Channel[channel].INTVAL |= MRT_FORCE_LOAD_IMMEDIATELY;
	  LPC_MRT->Channel[channel].CTRL |= (MRT_INTTERUPT_MODE_REPEAT << MRT_INTTERUPT_MODE) | MRT_INTERRUPT_ON;
}

uint32_t mrtmr_getCurrentValue(uint32_t channel)
{
	uint32_t timer;

	if (channel > 3)
	{
		// reset...
		return 0;
	}

	timer = LPC_MRT->Channel[channel].TIMER;
	return (timer == MRT_TIMER_IDLE) ? 0 : timer;
}


void mrtmr_disableMrtDelay(uint32_t channel)
{
	if (channel > 3)
	{
		// reset...
	}

	LPC_MRT->Channel[channel].CTRL &= ~(MRT_INTERRUPT_ON);
	LPC_MRT->Channel[channel].INTVAL = MRT_FORCE_LOAD_IMMEDIATELY;
}

