#ifndef _FSM_H_
#define _FSM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LPC8xx.h"
#include "event.h"

#define FSM_MAX_NESTED_STATE (10)

typedef struct fsm_state
{
	  struct fsm_state * parentState;
	  void (* onEnter)();
	  void (* onExit)();
	  evt_handled_type (* onEvent)(evt_type event);

} fsm_state_type;

void fsm_setInitialState(fsm_state_type * intialState);

void fsm_setNextState(fsm_state_type * currentState, fsm_state_type * nextState);

void fsm_sendEvent(evt_type event);

#ifdef __cplusplus
}
#endif

#endif
