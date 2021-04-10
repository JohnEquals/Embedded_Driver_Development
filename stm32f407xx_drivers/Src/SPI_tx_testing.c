/*
 * SPI_tx_testing.c
 *
 *  Created on: Feb 17, 2021
 *      Author: class
 *
 *  Note:  Verify that the SPI pins are sending the right signals by using a logic analyzer
 *
 *
 */


#include "stm32f407xx.h"
#include <string.h>

int main(void)
{

	uint8_t user_data[] = "Hello World";


	/*
	 * 	Configure 4 GPIO pins for SPI2 peripheral
	 */
	GPIO_Handle_t pin_MOSI, pin_MISO, pin_NSS, pin_SCK;
	// configure PC3 as SPI2 MOSI pin
	pin_MOSI.pGPIOx = GPIOC;
	pin_MOSI.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	pin_MOSI.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	pin_MOSI.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_3;
	pin_MOSI.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	pin_MOSI.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	pin_MOSI.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&pin_MOSI);
	// configure PC2 as SPI2 MISO pin
	pin_MISO.pGPIOx = GPIOC;
	pin_MISO.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	pin_MISO.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	pin_MISO.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_2;
	pin_MISO.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	pin_MISO.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	pin_MISO.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&pin_MISO);
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
	pin_NSS.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	pin_NSS.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	pin_NSS.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&pin_NSS);

	// Set configuration settings for SPI2 Peripheral
	SPI_Handle_t spi2_peripheral;
	spi2_peripheral.pSPIx = SPI2;
	spi2_peripheral.SPIConfig.SPI_CPHA = 0;
	spi2_peripheral.SPIConfig.SPI_CPOL = 1;
	spi2_peripheral.SPIConfig.SPI_DFF = 0;
	spi2_peripheral.SPIConfig.SPI_SSM = 0;
	spi2_peripheral.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;
	spi2_peripheral.SPIConfig.SPI_busConfig = SPI_BUS_CONFIG_FD;
	spi2_peripheral.SPIConfig.SPI_deviceMode = SPI_DEVICE_MODE_MASTER;

	// initialize SPI2 Peripheral
	SPI_Init(&spi2_peripheral);

	// Enable SPI2 Communications
	SPI_PeripheralControl(&spi2_peripheral, ENABLE);
	// send data to transmit over SPI2
	SPI_SendData(spi2_peripheral.pSPIx ,user_data, strlen((const char*)user_data));
	// Disable SPI2 Communications
	SPI_PeripheralControl(&spi2_peripheral, DISABLE);

	while(1){
		;
	}

	return 0;
}
