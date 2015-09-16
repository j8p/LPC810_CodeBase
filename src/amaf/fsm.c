#include "fsm.h"

fsm_state_type * FSM = 0;

void exitState(fsm_state_type * state)
{
	while (state) {
		if (state->onExit)
		{
			state->onExit();
		}

		state = state->parentState;
	}
}

typedef void (*onEnterFunction_type)();


void enterState(fsm_state_type * state)
{
	onEnterFunction_type onEnterFunctions[FSM_MAX_NESTED_STATE];
	uint32_t index = 0;

	while (state) {
		if (state->onEnter)
		{
			onEnterFunctions[index++] = state->onEnter;
		}

		state = state->parentState;
	}

	while (index) {
		(onEnterFunctions[--index])();
	}
}

void fsm_setInitialState(fsm_state_type * intialState)
{
	if (intialState == 0) {
		return;
	}

	FSM = intialState;
	enterState(FSM);
}


void fsm_setNextState(fsm_state_type * currentState, fsm_state_type * nextState)
{
	if (nextState == 0) {
		return;
	}

	if (currentState != FSM)
	{
		// Handle possible interrupt issues
		return;
	}

	exitState(FSM);
	FSM = nextState;
	enterState(FSM);
}

void fsm_sendEvent(evt_type event)
{
	fsm_state_type * state = FSM;

	while (state && state->onEvent) {
		if (state->onEvent(event) == HANDLED_EVENT) {
			break;
		}

		state = state->parentState;
	}
}
