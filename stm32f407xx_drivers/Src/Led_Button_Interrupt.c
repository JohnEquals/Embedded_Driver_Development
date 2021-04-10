/*
 * 004Led_Button_Interrupt.c
 *
 *  Created on: Oct 23, 2020
 *      Author: class
 */


#include "stm32f407xx_gpio_driver.h"
#define HIGH 1
#define LOW 0
#define BTN_PRESSED LOW
void delay(void)
{
	// this will introduce ~200ms delay when system clock is 16MHz
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}
int main(void){
	GPIO_Handle_t GpioBtn, GpioLed;
	// Settings for PD14 (button)
	GpioBtn.pGPIOx = GPIOD;
	GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_RT;
	GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PD;
	// Configure clock and GPIO peripheral registers for PD14
	GPIO_Init(&GpioBtn);
	// Settings for PD12 (LED)
	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
	// Configure clock and GPIO peripheral registers for PD12
	GPIO_Init(&GpioLed);
	// IRQ Configuration
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI15_10, NVIC_IRQ_PRI15);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI15_10, ENABLE);
	while(1);
	return 0;
}
// ISR for button interrupt
void EXTI15_10_IRQHandler(void){
	delay();
	GPIO_IRQHandling( GPIO_PIN_NO_14 );
	GPIO_ToggleOutputPin(GPIOD,  GPIO_PIN_NO_12);
}
