# LPC810 TLOB - Iron

Based on LPC810 CodeBase, it is a simple test to explore LPC810 possibilities. This first test reproduces my steam iron behavior: if you double click on the button, it will stay on. I use the DIP8 package version.

# How to setup the hardware

This code assume you have the micro controller connected with two LEDs (with the corresponding resistors) and one push button. The first LED indicates when the iron is on. The second one indicates if the timing is still short enough to trigger the “stay on” state.

- LED1 is connected to PIO0_2 on one side and to the resistor and the ground on the other side.
- LED2 is connected to PIO0_4 on one side and to the resistor and the ground on the other side.
- Push button is connected to PIO0_3 on one side and to the ground on the other side.

# What is used

This test uses multi rate timers an pin interrupts. The behavior is configured using a little (could be hierarchical) state machine.

# File description

Here is a short description of the files that are not from the original repositiory.

src/amaf
src/amaf/event.h - Simple enumeration of the available events for the state machine.
src/amaf/fsm.h and fsm.c - Naive state machine functions and state definitions
src/amaf/led.h and led.c - Naive PWM functions for LEDs
src/amaf/mrtimer.h and mrtimer.c - Simple multi rate timer handling

src/main.c - Minimal main

src/tlbo/twoLedsOneButton.h and twoLedsOneButton.h - Setup for the two LEDs and on button configuration
src/tlob/iron/fsm_TwoLedsOneButton_Iron.h and fsm_TwoLedsOneButton_Iron.c - Simple state machine that reproduces the steam iron behavior. It only has 7 states.

# Uploading the firmware using lpc2lisp

./lpc21isp_197/lpc21isp -NXPARM -verify -debug3 -hex ./Release/LPC810.hex /dev/tty.usbserial 115200 12


