/*
 * button.h
 *
 *  Created on: May 9, 2021
 *      Author: micha
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_


// States for State Machine
typedef enum
{
	IDLE = 0,
	DEBOUNCE,
	PRESSED,
	REPEAT

} BUTTON_STATE;


// Struct for button
typedef struct
{
	BUTTON_STATE 	State;
	GPIO_TypeDef* 	GpioPort;
	uint16_t 		GpioPin;

// Timers
	uint32_t		LastTick;
	uint32_t 		TimerDebounce; //Fixed
	uint32_t 		TimerLongPress;
	uint32_t		TimerRepeat;

// Callbacks
	void(*ButtonPressed)(void);
	void(*ButtonLongPressed)(void);
	void(*ButtonRepeat)(void);


}TButton;


// public functions

void ButtonTask(TButton* Key);


void ButtonSetDebounceTime(TButton* Key, uint32_t Miliseconds);
void ButtonSetLongPressTime(TButton* Key, uint32_t Miliseconds);
void ButtonSetRepeatTime(TButton* Key, uint32_t Miliseconds);

void ButtonRegisterPressCallback(TButton* Key, void *Callback);
void ButtonRegisterLongPressCallback(TButton* Key, void *Callback);
void ButtonRegisterRepeatCallback(TButton* Key, void *Callback);

void ButtonInitKey(TButton* Key, GPIO_TypeDef* GpioPort, uint16_t GpioPin, uint32_t TimerDebounce, uint32_t TimerLongPress, uint32_t TimerRepeat);


#endif /* INC_BUTTON_H_ */
