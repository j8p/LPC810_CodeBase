#include "twoLedsOneButton.h"

// GREEN LED
#define LED1_LOCATION    (2)
// RED LED
#define LED2_LOCATION    (4)
 // PUSH BUTTON
#define BTN1_LOCATION    (3)
// INTERRUPT PIN
#define INT_PIN          (0)

void PININT0_IRQHandler(void)
{
	if (tlob_isButtonInterruptReleased(1))
	{
		fsm_sendEvent(PIN_RELEASED_EVENT);
	    return;
	}

	if (tlob_isButtonInterruptPressed(1))
	{
		fsm_sendEvent(PIN_PRESSED_EVENT);
		return;
	}
}

void tlob_setup()
{
	// Setup GPIO port
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);
	LPC_SYSCON->PRESETCTRL &= ~(1 << 10);
	LPC_SYSCON->PRESETCTRL |= (1 << 10);

	// Enable clock divider for glitch filter
	LPC_SYSCON->IOCONCLKDIV[0] |= 1;

	// Configure PINs
    LPC_SWM->PINASSIGN0 = 0xffff000CUL;
    LPC_SWM->PINENABLE0 = 0xffffffbfUL;

    // Set PIN direction
    LPC_GPIO_PORT->DIR0 |= (1 << LED1_LOCATION);
    LPC_GPIO_PORT->DIR0 |= (1 << LED2_LOCATION);
    LPC_GPIO_PORT->DIR0 &= ~(1 << BTN1_LOCATION);

    // Enable pull-up resistor
    LPC_IOCON->PIO0_3 &= ~(3 << 3);
    LPC_IOCON->PIO0_3 |= (1 <<4);

    // Enable glitch filter
    LPC_IOCON->PIO0_3 |= 0x1 << 11;

    // Select glitch filter clock divider
    LPC_IOCON->PIO0_3 |= 0x6 << 13;

    // Enable interrupt for Button
    LPC_SYSCON->PINTSEL[INT_PIN] = BTN1_LOCATION;
    LPC_PIN_INT->ISEL &= ~(1 << INT_PIN);

    // Previous test values
    //LPC_PIN_INT->ISEL |= (1 << INT_PIN);
    //LPC_PIN_INT->IENR |= (1 << INT_PIN);
    //LPC_PIN_INT->SIENR |= (1 << INT_PIN);
    //LPC_PIN_INT->IENF |= (1 << INT_PIN);
    //LPC_PIN_INT->SIENF |= (1 << INT_PIN);
}

void tlob_setLedsState(uint32_t enableLed1, uint32_t enableLed2)
{
	if (enableLed1 == enableLed2) {
		if (enableLed1) {
			LPC_GPIO_PORT->SET0 = (1 << LED1_LOCATION) | (1 << LED2_LOCATION);
			return;
		}

		LPC_GPIO_PORT->CLR0 = (1 << LED1_LOCATION) | (1 << LED2_LOCATION);
		return;
	}

	LPC_GPIO_PORT->SET0 = (enableLed1) ? 1 << LED1_LOCATION : 1 << LED2_LOCATION;
	LPC_GPIO_PORT->CLR0 = (!enableLed1) ? 1 << LED1_LOCATION : 1 << LED2_LOCATION;
}

void tlob_toggleLedsState(uint32_t toggleLed1, uint32_t toggleLed2)
{
	if (toggleLed1 || toggleLed2)
	{
		uint32_t enabledLed1 = LPC_GPIO_PORT->PIN0 & (1 << LED1_LOCATION);
		uint32_t enabledLed2 = LPC_GPIO_PORT->PIN0 & (1 << LED2_LOCATION);

		enabledLed1 = (toggleLed1) ? !enabledLed1 : enabledLed1;
		enabledLed2 = (toggleLed2) ? !enabledLed2 : enabledLed2;

		tlob_setLedsState(enabledLed1, enabledLed2);
	}
}
void tlob_disableButtonInterrupt()
{
	LPC_PIN_INT->IENF &= ~(1 << INT_PIN);
	LPC_PIN_INT->IENR &= ~(1 << INT_PIN);
}

void tlob_enableButtonInterrupt(uint32_t release)
{
	LPC_PIN_INT->RISE |= (1 << INT_PIN);
	LPC_PIN_INT->FALL |= (1 << INT_PIN);
	if (release)
    {
		LPC_PIN_INT->IENF &= ~(1 << INT_PIN);
    	LPC_PIN_INT->IENR |= (1 << INT_PIN);
    }
	else
	{
		LPC_PIN_INT->IENR &= ~(1 << INT_PIN);
		LPC_PIN_INT->IENF |= (1 << INT_PIN);
	}
}

uint32_t tlob_isButtonInterruptReleased(uint32_t clear)
{
	uint32_t result = LPC_PIN_INT->RISE &= (1 << INT_PIN);

	if (result && clear)
	{
		LPC_PIN_INT->RISE |= (1 << INT_PIN);
	}

	return result;
}

uint32_t tlob_isButtonInterruptPressed(uint32_t clear)
{
	uint32_t result = LPC_PIN_INT->FALL &= (1 << INT_PIN);

	if (result && clear)
	{
		LPC_PIN_INT->FALL |= (1 << INT_PIN);
	}

	return result;
}
