/*
 * dht11.c
 *
 *  Created on: Dec 1, 2021
 *      Author: John
 *
 *      Hardware requirements:
 *      Note: DHT11 Temperature and Humidity Sensor
 */


#include "dht11.h"
#include<stdint.h>
#include "stm32f407xx.h"
#include<stdbool.h>

#define LOW		(0)
#define HIGH	(1)

static void delay( uint32_t time)
{
	for( uint32_t i = 0; i < time; i++);
}

uint16_t humidity = 0;
uint16_t temperature = 0;
uint8_t checksum = 0;
bool data_valid = false;


void dht11_get_data( GPIO_Handle_t *dht_signal_pin)
{
	/*
	 * data format (40 bits):
	 * 				   - 8 bit integral RH data
	 * 				   - 8 bit decimal RH data
	 * 				   - 8 bit integral T data
	 *                 - 8 bit decimal T data
	 *                 - 8 bit checksum (should be last 8 bit of the above data sequence.
	 *                   So should be equal to 8bit decimal T data.)
	 *
	 *  Note:  MSB is sent first so must bitshift to appropriate bit positon in data variable
	 *
	 *  comms procesS:
	 *  - GPIO pin is HIGH initially. (set GPIO to open-drain, no pull-up/pull-down resistors enabled)
	 *  - GPIO sends start signal (LOW) to DHT11 signal pin.  Hold this signal LOW for at least 18 ms.
	 *  - GPIO pulls signal HIGH and wait for DHT response (20-40 us)
	 *  - DHT sends response signal for 80 us
	 *  - DHT pulls up voltage for 80 us.
	 *  - DHT begins to send data.
	 *     - It sends data 1 bit by 1 bit.  Each data bit starts with LOW signal for 50us
	 *     and then the data bit signal.  If data bit (a HIGH signal) is 26-28us long, its 0.
	 *     If it is 70us long, its 1.  Then DHT sends LOW signal for 50us and the next data bit.
	 *
	 */

	humidity = 0;
	temperature = 0;
	checksum = 0;
	data_valid = false;



	// GPIO pin must be HIGH initially
	//GPIO_WriteToOutputPin(dht_signal_pin -> pGPIOx, dht_signal_pin -> GPIO_PinConfig.GPIO_PinNumber, HIGH);
	// send start signal
	GPIO_WriteToOutputPin(dht_signal_pin -> pGPIOx, dht_signal_pin -> GPIO_PinConfig.GPIO_PinNumber, LOW);
	// wait 18 ms
	delay(20000);
	// pull GPIO signal high and wait for DHT response (turn GPIO into input pin mode)
	GPIO_WriteToOutputPin(dht_signal_pin -> pGPIOx, dht_signal_pin -> GPIO_PinConfig.GPIO_PinNumber, HIGH);
	dht_signal_pin -> GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIO_Init(dht_signal_pin);
	// wait 40 us
	delay(40);
	// DHT will send a LOW signal to respond
	while(GPIO_ReadFromInputPin(dht_signal_pin -> pGPIOx, dht_signal_pin -> GPIO_PinConfig.GPIO_PinNumber) != LOW);
	// wait 80 us
	delay(80);
	// DHT sends HIGH signal
	while(GPIO_ReadFromInputPin(dht_signal_pin -> pGPIOx, dht_signal_pin -> GPIO_PinConfig.GPIO_PinNumber) != HIGH);
	// wait 80 us
	delay(80);
	while(GPIO_ReadFromInputPin(dht_signal_pin -> pGPIOx, dht_signal_pin -> GPIO_PinConfig.GPIO_PinNumber) != LOW);
	delay(50);
	for(uint8_t i=15; i >= 0; i--)
	{
		delay(70);
		if(GPIO_ReadFromInputPin(dht_signal_pin -> pGPIOx, dht_signal_pin -> GPIO_PinConfig.GPIO_PinNumber) == HIGH)
		{
			humidity |= (1<<i);
		}
		delay(50);
	}
	for(uint8_t i=15; i >= 0; i--)
	{
		delay(70);
		if(GPIO_ReadFromInputPin(dht_signal_pin -> pGPIOx, dht_signal_pin -> GPIO_PinConfig.GPIO_PinNumber) == HIGH)
		{
			temperature |= (1<<i);
		}
		delay(50);
	}
	for(uint8_t i=7; i>=0; i--)
	{
		delay(70);
		if(GPIO_ReadFromInputPin(dht_signal_pin -> pGPIOx, dht_signal_pin -> GPIO_PinConfig.GPIO_PinNumber) == HIGH)
		{
			checksum |= (1<<i);
		}
		delay(50);
	}
	if(checksum == ((uint8_t)humidity))
	{
		data_valid = true;
	}
}
