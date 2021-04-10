/*
 * 1edToggle.c
 *
 *      Author: John H
 */


#include "stm32f407xx.h"

void delay(void)
{
	for(uint32_t i = 0 ; i < 800000 ; i ++);
}

int main(void)
{
	uint8_t ledIndex = 0;
	GPIO_Handle_t GpioLed[4]
	// PD12, green led
	GpioLed[0].pGPIOx = GPIOD;
	GpioLed[0].GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioLed[0].GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed[0].GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed[0].GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed[0].GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	// PD13, orange led
	GpioLed[1].pGPIOx = GPIOD;
	GpioLed[1].GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GpioLed[1].GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed[1].GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed[1].GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed[1].GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	// PD14, red led
	GpioLed[2].pGPIOx = GPIOD;
	GpioLed[2].GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GpioLed[2].GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed[2].GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed[2].GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed[2].GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	// PD15, blue led
	GpioLed[3].pGPIOx = GPIOD;
	GpioLed[3].GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GpioLed[3].GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed[3].GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed[3].GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed[3].GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	for(uint8_t i= 0;i< sizeof(GpioLed)/sizeof(GPIO_Handle_t);i++){
		GPIO_Init(&GpioLed[i]);}

	while(1)
	{
		// toggle all 4 leds in a cycle
		for(uint8_t k = 0;  k < 4; k++){
			GPIO_ToggleOutputPin(GPIOD, GpioLed[ledIndex].GPIO_PinConfig.GPIO_PinNumber);
			// allows us to see which led is being toggled
			delay();
		}
		ledIndex = (ledIndex+1)%4;  // sets up the index for next led toggle
	}
	return 0;
}
