#include "led.h"

#define LED_MAX_TIMING (13)
uint32_t ledTimings[] = {0, 0, 1, 6, 15, 25, 48, 64, 128, 255, 512, 768, 1024, 1024 };
int32_t ledDirection = 0;
uint32_t  ledCounter = 0;

uint32_t led_resetTiming(uint32_t on)
{
	ledDirection = 0;
	ledCounter = 0;

	return on ? LED_MAX_TIMING : 0;
}

uint32_t led_getNextTiming(uint32_t currentTiming)
{
    if (ledCounter == 4)
    {
    	ledCounter = 0;

    	if (currentTiming == 0)
    	{
    		ledDirection = +1;
    	}
    	else if (currentTiming == LED_MAX_TIMING)
    	{
    		ledDirection = -1;
    	}

    	currentTiming += ledDirection;
    }
    ledCounter++;

    return currentTiming;
}

uint32_t led_hasTiming(uint32_t currentTiming)
{
	return ledTimings[currentTiming] != 0;
}

void led_waitTiming(uint32_t currentTiming)
{
	volatile uint32_t i = 0;
	while (i<ledTimings[currentTiming])
	{
		i++;
	}
}
