#ifndef _EVNT_H_
#define _EVNT_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum event_type {
   	TIMER1_EVENT,
	TIMER2_EVENT,
	TIMER3_EVENT,
	TIMER4_EVENT,
   	PIN_PRESSED_EVENT,
   	PIN_RELEASED_EVENT
} evt_type;

typedef enum event_handled_type {
   	HANDLED_EVENT,
	NOT_HANDLED_EVENT
} evt_handled_type;

#ifdef __cplusplus
}
#endif

#endif
