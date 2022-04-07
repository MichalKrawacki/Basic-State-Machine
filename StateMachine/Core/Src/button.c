/*
 * button.c
 *
 *  Created on: May 9, 2021
 *      Author: micha
 */

#include "main.h"
#include "button.h"



// Button Init
void ButtonInitKey(TButton* Key, GPIO_TypeDef* GpioPort, uint16_t GpioPin, uint32_t TimerDebounce, uint32_t TimerLongPress, uint32_t TimerRepeat)
{
	Key->State = IDLE;

	Key->GpioPort = GpioPort;
	Key->GpioPin = GpioPin;

	Key->TimerDebounce = TimerDebounce;
	Key->TimerLongPress = TimerLongPress;
	Key->TimerRepeat = TimerRepeat;
}


// Time setting function
void ButtonSetDebounceTime(TButton* Key, uint32_t Miliseconds)
{
	Key->TimerDebounce = Miliseconds;
}
void ButtonSetLongPressTime(TButton* Key, uint32_t Miliseconds)
{
	Key->TimerLongPress = Miliseconds;
}
void ButtonSetRepeatTime(TButton* Key, uint32_t Miliseconds)
{
	Key->TimerRepeat = Miliseconds;
}


// Register callbacks
void ButtonRegisterPressCallback(TButton* Key, void *Callback)
{
	Key->ButtonPressed = Callback;
}

void ButtonRegisterLongPressCallback(TButton* Key, void *Callback)
{
	Key->ButtonLongPressed = Callback;
}

void ButtonRegisterRepeatCallback(TButton* Key, void *Callback)
{
	Key->ButtonRepeat = Callback;
}


// States routines
void ButtonIdleRoutine(TButton* Key)
{
	if(HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin) == GPIO_PIN_RESET)
	{
		Key->State = DEBOUNCE;
		Key->LastTick = HAL_GetTick();
	}
}

void ButtonDebounceRoutine(TButton* Key)
{

	if((HAL_GetTick() - Key->LastTick) > Key->TimerDebounce)
	{
		if(HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin) == GPIO_PIN_RESET)
		{
			Key -> State = PRESSED;
			Key -> LastTick = HAL_GetTick();

			if(Key->ButtonPressed != NULL)
			{
				Key->ButtonPressed();
			}

		}
		else
		{
			Key->State = IDLE;
		}
	}

}

void ButtonPressedRoutine(TButton* Key)
{
	if(HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin) == GPIO_PIN_SET)
	{
		Key->State = IDLE;
	}
	else
	{

		if( (HAL_GetTick() - (Key -> LastTick)) > Key->TimerLongPress)
		{
			Key->State = REPEAT;
			Key -> LastTick = HAL_GetTick();

			if(Key->ButtonLongPressed != NULL)
			{
				Key->ButtonLongPressed();
			}
		}
	}

}

void ButtonRepeatRoutine(TButton* Key)
{
	if(HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin) == GPIO_PIN_SET)
		{
			Key->State = IDLE;
		}
		else
		{

			if( (HAL_GetTick() - (Key -> LastTick)) > Key->TimerRepeat)
			{

				Key -> LastTick = HAL_GetTick();

				if(Key-> ButtonRepeat != NULL)
				{
					Key -> ButtonRepeat();
				}
			}
		}
}

// State Machine - funkcje przyjmujace obiekty
void ButtonTask(TButton* Key)
{
	switch(Key->State)
	{
	case IDLE:
		ButtonIdleRoutine(Key);
		break;

	case DEBOUNCE:
		ButtonDebounceRoutine(Key);
		break;

	case PRESSED:
		ButtonPressedRoutine(Key);
		break;

	case REPEAT:
		ButtonRepeatRoutine(Key);
		break;
	}
}



