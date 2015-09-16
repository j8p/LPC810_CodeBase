#include "fsm_TwoLedsOneButton_Iron.h"

// Define MRT delays
#define MRT_CONTINUE_DELAY (__SYSTEM_CLOCK / 0x2)
#define MRT_START_KEEP_ON_DELAY (__SYSTEM_CLOCK / 0x2)
#define MRT_START_KEEP_ON_TIMEOUT (15 * __SYSTEM_CLOCK)

// Declare states and on event functions
fsm_state_type state_00_idle;
evt_handled_type onEvent_00_idle(evt_type event);

fsm_state_type state_01_firstOn;
evt_handled_type onEvent_01_firstOn(evt_type event);

fsm_state_type state_02_on;
evt_handled_type onEvent_02_on(evt_type event);

fsm_state_type state_03_firstOff;
evt_handled_type onEvent_03_firstOff(evt_type event);

fsm_state_type state_04_secondOn;
evt_handled_type onEvent_04_secondOn(evt_type event);

fsm_state_type state_05_secondOff;
evt_handled_type onEvent_05_secondOff(evt_type event);

fsm_state_type state_06_keepOn;
evt_handled_type onEvent_06_keepOn(evt_type event);

// Declare current LED timing variable used for PWM
uint32_t currentLedTiming = 0;


// onEvent and onExit common functions

void onEnter_disableLedsAndSetButtonPressedInterrupt()
{
	tlob_setLedsState(0, 0);
	tlob_enableButtonInterrupt(TLOB_BUTTON_INTERRUPT_PRESS);
}

void onExit_resetButtonInterrupt()
{
	tlob_disableButtonInterrupt();
}

void onEnter_setLed2AndSetButtonReleasedInterrupt()
{
	tlob_setLedsState(0, 1);
	tlob_enableButtonInterrupt(TLOB_BUTTON_INTERRUPT_RELEASE);
}

void onEnter_setLedsContinueTimer1AndButtonReleaseInterrupt()
{
	tlob_setLedsState(1, 1);
	mrtmr_enableMrtDelay(TIMER_1, MRT_CONTINUE_DELAY);
	tlob_enableButtonInterrupt(TLOB_BUTTON_INTERRUPT_RELEASE);
}

void onExit_resetTimer1AndButtonInterrupt()
{
	mrtmr_disableMrtDelay(TIMER_1);
	tlob_disableButtonInterrupt();
}

void onExit_resetTimer1And2AndButtonInterrupt()
{
	mrtmr_disableMrtDelay(TIMER_1);
	mrtmr_disableMrtDelay(TIMER_2);
	tlob_disableButtonInterrupt();
}

void onEnter_setLed1ContinueTimer1AndButtonPressInterrupt()
{
	tlob_setLedsState(1, 0);
	mrtmr_enableMrtDelay(TIMER_1, MRT_CONTINUE_DELAY);
	tlob_enableButtonInterrupt(TLOB_BUTTON_INTERRUPT_PRESS);
}

void onEnter_setLed1KeepOnTimer1AndButtonPressInterrupt()
{
	tlob_setLedsState(1, 0);
	mrtmr_enableMrtDelay(TIMER_1, MRT_START_KEEP_ON_DELAY);
	tlob_enableButtonInterrupt(TLOB_BUTTON_INTERRUPT_PRESS);
}

void onEnter_setLed1KonOnTimeoutTimer1RepeatTimer2AndButtonPressInterrupt()
{
	tlob_setLedsState(0, 0);
	mrtmr_enableMrtDelay(TIMER_1, MRT_START_KEEP_ON_TIMEOUT);
	currentLedTiming = led_resetTiming(0);
	mrtmr_enableMrtRepeatingDelay(TIMER_2, LED_TIMING_FREQENCY);
	tlob_enableButtonInterrupt(TLOB_BUTTON_INTERRUPT_PRESS);
}


// ---------------------------------------
// States and corresponding event handling
// ---------------------------------------

// state_00_idle
// output initialization: disable LEDs
// input: button pressed
fsm_state_type state_00_idle = (fsm_state_type) {
	.parentState = 0,
	.onEnter     = onEnter_disableLedsAndSetButtonPressedInterrupt,
	.onExit      = onExit_resetButtonInterrupt,
	.onEvent     = onEvent_00_idle
};

// onEvent_00_idle
// - on button pressed -> set state to state_01_firstOn
evt_handled_type onEvent_00_idle(evt_type event)
{
	if (event == PIN_PRESSED_EVENT) {
		fsm_setNextState(&state_00_idle, &state_01_firstOn);
		return HANDLED_EVENT;
	}

	return NOT_HANDLED_EVENT;
}


// state_01_firstOn
// output initialization: enable LEDs
// input: button released, timer1
fsm_state_type state_01_firstOn = (fsm_state_type) {
	.parentState = 0,
	.onEnter     = onEnter_setLedsContinueTimer1AndButtonReleaseInterrupt,
	.onExit      = onExit_resetTimer1AndButtonInterrupt,
	.onEvent     = onEvent_01_firstOn
};

// onEvent_01_firstOn
// - on button release -> set state to state_03_firstOff
// - on timer1 -> set state to state_02_on
evt_handled_type onEvent_01_firstOn(evt_type event)
{
	switch (event) {
		case TIMER1_EVENT:
			fsm_setNextState(&state_01_firstOn, &state_02_on);
			return HANDLED_EVENT;
		case PIN_RELEASED_EVENT:
			fsm_setNextState(&state_01_firstOn, &state_03_firstOff);
			return HANDLED_EVENT;
		default:
			return NOT_HANDLED_EVENT;
	}
}


// state_02_on
// output initialization: enable LED 2 only
// input: button released
fsm_state_type state_02_on = (fsm_state_type) {
	.parentState = 0,
	.onEnter     = onEnter_setLed2AndSetButtonReleasedInterrupt,
	.onExit      = onExit_resetButtonInterrupt,
	.onEvent     = onEvent_02_on
};

// onEvent_02_on
// - on button release -> set state to state_00_idle
evt_handled_type onEvent_02_on(evt_type event)
{
	if (event == PIN_RELEASED_EVENT) {
		fsm_setNextState(&state_02_on, &state_00_idle);
		return HANDLED_EVENT;
	}

	return NOT_HANDLED_EVENT;
}


// state_03_firstOff
// output initialization: enable LED 1 and disable LED 2
// input: button pressed, timer1
fsm_state_type state_03_firstOff = (fsm_state_type) {
	.parentState = 0,
	.onEnter     = onEnter_setLed1ContinueTimer1AndButtonPressInterrupt,
	.onExit      = onExit_resetTimer1AndButtonInterrupt,
	.onEvent     = onEvent_03_firstOff
};

// onEvent_03_firstOff
// - on button pressed -> set state to state_04_secondOn
// - on timer1 -> set state to state_00_idle
evt_handled_type onEvent_03_firstOff(evt_type event)
{
	switch (event) {
		case TIMER1_EVENT:
			fsm_setNextState(&state_03_firstOff, &state_00_idle);
			return HANDLED_EVENT;
		case PIN_PRESSED_EVENT:
			fsm_setNextState(&state_03_firstOff, &state_04_secondOn);
			return HANDLED_EVENT;
		default:
			return NOT_HANDLED_EVENT;
	}
}


// state_04_secondOn
// output initialization: enable LEDs
// input: button released, timer1
fsm_state_type state_04_secondOn = (fsm_state_type) {
	.parentState = 0,
	.onEnter     = onEnter_setLedsContinueTimer1AndButtonReleaseInterrupt,
	.onExit      = onExit_resetTimer1AndButtonInterrupt,
	.onEvent     = onEvent_04_secondOn
};

// onEvent_04_secondOn
// - on button release -> set state to state_05_secondOff
// - on timer1 -> set state to state_02_on
evt_handled_type onEvent_04_secondOn(evt_type event)
{
	switch (event) {
		case TIMER1_EVENT:
			fsm_setNextState(&state_04_secondOn, &state_02_on);
			return HANDLED_EVENT;
		case PIN_RELEASED_EVENT:
			fsm_setNextState(&state_04_secondOn, &state_05_secondOff);
			return HANDLED_EVENT;
		default:
			return NOT_HANDLED_EVENT;
	}
}


// state_05_secondOff
// output initialization: enable LED 1 and disable LED 2
// input: button pressed, timer1
fsm_state_type state_05_secondOff = (fsm_state_type) {
	.parentState = 0,
	.onEnter     = onEnter_setLed1KeepOnTimer1AndButtonPressInterrupt,
	.onExit      = onExit_resetTimer1AndButtonInterrupt,
	.onEvent     = onEvent_05_secondOff
};

// onEvent_05_secondOff
// - on button pressed -> set state to state_02_on
// - on timer1 -> set state to state_06_keepOn
evt_handled_type onEvent_05_secondOff(evt_type event)
{
	switch (event) {
		case TIMER1_EVENT:
			fsm_setNextState(&state_05_secondOff, &state_06_keepOn);
			return HANDLED_EVENT;
		case PIN_PRESSED_EVENT:
			fsm_setNextState(&state_05_secondOff, &state_02_on);
			return HANDLED_EVENT;
		default:
			return NOT_HANDLED_EVENT;
	}
}


// state_06_keepOn
// output initialization: enable LED 1 and disable LED 2
// input: button pressed, timer1 for timeout, timer2 for PWM
fsm_state_type state_06_keepOn = (fsm_state_type) {
	.parentState = 0,
	.onEnter     = onEnter_setLed1KonOnTimeoutTimer1RepeatTimer2AndButtonPressInterrupt,
	.onExit      = onExit_resetTimer1And2AndButtonInterrupt,
	.onEvent     = onEvent_06_keepOn
};

// onEvent_06_keepOn
// - on button pressed -> set state to state_02_on
// - on timer1 -> set state to state_00_idle (timeout)
// - on timer2 -> LED PWM
evt_handled_type onEvent_06_keepOn(evt_type event)
{
	switch (event) {
		case TIMER1_EVENT:
			fsm_setNextState(&state_06_keepOn, &state_00_idle);
			return HANDLED_EVENT;
		case TIMER2_EVENT:
			currentLedTiming = led_getNextTiming(currentLedTiming);
			if (led_hasTiming(currentLedTiming))
			{
				tlob_setLedsState(0, 1);
				led_waitTiming(currentLedTiming);
				tlob_setLedsState(0, 0);
			}
			return HANDLED_EVENT;
		case PIN_PRESSED_EVENT:
			fsm_setNextState(&state_06_keepOn, &state_02_on);
			return HANDLED_EVENT;
		default:
			return NOT_HANDLED_EVENT;
	}
}


// Setup and run
void fsm_tlob_iron_setup()
{
	mrtmr_setupMrtDelay();

	NVIC_EnableIRQ(PININT0_IRQn);
    NVIC_EnableIRQ(MRT_IRQn);

    fsm_setInitialState(&state_00_idle);
}
