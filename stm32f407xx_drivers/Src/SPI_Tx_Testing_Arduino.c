/*
 * SPI_Tx_Testing_Arduino.c
 *
 *  Created on: Feb 22, 2021
 *      Author: John H.
 *
 *  Description:      This tests SPI communication "Simplex" mode (Full Duplex with MISO line d/c) in
 *  				  a 1 master (stm32f407xx Discovery Board) and 1 slave (Arduino UNO) configuration.
 *
 */
#include "stm32f407xx.h"
#include <string.h>

// time is in milliseconds
void delay( uint32_t time){
	for( uint32_t i = 0; i < (time*2000); i++);
}


void SPI2_GPIOPinsInit(void){

	/*
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);
	*/

	GPIO_Handle_t pin_MOSI, pin_NSS, pin_SCK;
	// configure PC3 as SPI2 MOSI pin
	pin_MOSI.pGPIOx = GPIOC;
	pin_MOSI.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	pin_MOSI.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	pin_MOSI.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_3;
	pin_MOSI.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	pin_MOSI.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	pin_MOSI.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&pin_MOSI);

	// configure PB10 as SPI2 SCK pin
	pin_SCK.pGPIOx = GPIOB;
	pin_SCK.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	pin_SCK.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	pin_SCK.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
	pin_SCK.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	pin_SCK.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	pin_SCK.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&pin_SCK);
	// configure PB12 as SPI2 NSS pin
	pin_NSS.pGPIOx = GPIOB;
	pin_NSS.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	pin_NSS.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	pin_NSS.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	pin_NSS.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	pin_NSS.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	pin_NSS.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&pin_NSS);
}


int main(void)
{

	char user_data[] = "An Arduino Uno board is best suited for beginners who have just started using microcontrollers, on the other hand, Arduino Mega board is for enthusiasts who require a lot of I/O pins for their projects";

	SPI2_GPIOPinsInit();

	// configure user button pin
	GPIO_Handle_t GpioBtn;
	GpioBtn.pGPIOx = GPIOA;
	GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GpioBtn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&GpioBtn);

	// Set configuration settings for SPI2 Peripheral
	SPI_Handle_t spi2_peripheral;
	spi2_peripheral.pSPIx = SPI2;
	spi2_peripheral.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	spi2_peripheral.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	spi2_peripheral.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	spi2_peripheral.SPIConfig.SPI_SSM = SPI_SSM_DI;
	spi2_peripheral.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;
	spi2_peripheral.SPIConfig.SPI_busConfig = SPI_BUS_CONFIG_FD;
	spi2_peripheral.SPIConfig.SPI_deviceMode = SPI_DEVICE_MODE_MASTER;

	// initialize SPI2 Peripheral
	SPI_Init(&spi2_peripheral);

	/*
	 *   Setting SSOE will enable NSS output
	 *   The NSS pin is automatically managed by the hardware.
	 *   i.e. when SPE = 1, NSS is pulled LOW.
	 *   and when SPE = 0, NSS pin pulled HIGH
	 */
	SPI_SSOEConfig( &spi2_peripheral, ENABLE );

	while(1){

		// wait until button is pressed
		while ( !GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) );
		// avoiding button de-bouncing issues.   200 ms of delay
		delay(250);
		// Enable SPI2 Communications
		SPI_PeripheralControl(&spi2_peripheral, ENABLE);

		// send information about the data over SPI2 to slave
		uint8_t dataLen = strlen(user_data);
		SPI_SendData(spi2_peripheral.pSPIx ,&dataLen, 1);

		// send data to transmit over SPI2
		SPI_SendData(spi2_peripheral.pSPIx ,(uint8_t *) user_data, strlen(user_data));

		// wait for SPI to not be busy
		// while( spi2_peripheral.pSPIx->SR & (1 << SPI_SR_BSY));
		while(SPI_GetFlagStatus( spi2_peripheral.pSPIx, SPI_BSY_FLAG));

		// Disable SPI2 Communications once SPI is no longer busy
		SPI_PeripheralControl(&spi2_peripheral, DISABLE);
	}
	return 0;
}
