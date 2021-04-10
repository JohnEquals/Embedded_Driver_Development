/*
 * SPI_CmdHandling_Arduino.c
 *
 *  Created on: Feb 26, 2021
 *      Author: John H.
 *
 *  Description:      This tests SPI communication Full Duplex mode in
 *  				  a 1 master (stm32f407xx Discovery Board) and 1 slave (Arduino UNO) configuration.
 *					  I test command handling in a "command and response" communication set-up.
 *					  Master device sends slave device a command and slave sends back an ACK or NACK response.
 *					  If master receives an ACK response, master sends arguments.  Once slave receives arguments,
 *					  slave will behave accordingly.  Depending on the command, slave may send additional packets
 *					  to master.
 */
#include "stm32f407xx.h"
#include <string.h>
#include <stdio.h>

// Commands
#define COMMAND_LED_CTRL		0x50
#define COMMAND_SENSOR_READ		0x51
#define COMMAND_LED_READ		0x52
#define COMMAND_PRINT			0x53
#define COMMAND_ID_READ			0x54
// LED toggle
#define LED_ON					1
#define LED_OFF					0
// Arduino Analog pins
#define ANALOG_PIN0				0
#define ANALOG_PIN1				1
#define ANALOG_PIN2				2
#define ANALOG_PIN3				3
// Arduino LED
#define LED_PIN					9
// Response from Arduino
#define ACK		0xF5
#define NACK	0xFA



// enable printf debugging via OpenOCD Semihosting.  Now you can use printf normally (include \n in string)
extern void initialise_monitor_handles(void);

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

	initialise_monitor_handles();

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

		// command arguments container
		uint8_t arg[2];
		uint8_t dummyWrite = 0xFF;
		uint8_t dummyRead;
		uint8_t pinState;

		// wait until button is pressed
		while ( !GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) );
		// avoiding button de-bouncing issues.   200 ms of delay
		delay(250);
		// Enable SPI2 Communications
		SPI_PeripheralControl(&spi2_peripheral, ENABLE);

		//  Send command to slave
		uint8_t cmd = COMMAND_LED_CTRL;
		SPI_SendData(spi2_peripheral.pSPIx, &cmd, sizeof(cmd));
		// clear RXNE flag by removing garbage data from Rxbuffer
		SPI_ReceiveData(spi2_peripheral.pSPIx, &dummyRead, sizeof(dummyRead));

		// send dummy data to Txbuffer receive ACK/NACK response
		SPI_SendData(spi2_peripheral.pSPIx, &dummyWrite, sizeof(dummyWrite));
		uint8_t response = 0;
		SPI_ReceiveData(spi2_peripheral.pSPIx, &response, sizeof(cmd));
		if(response == ACK){
			arg[0] = LED_PIN;
			arg[1] = LED_ON;
			// send command arguments
			SPI_SendData(spi2_peripheral.pSPIx, arg, sizeof(arg));
			SPI_ReceiveData(spi2_peripheral.pSPIx, arg, sizeof(arg));
		}else{
			// printf error message to terminal
			printf("Error! Slave did not recognize command: %x\n",cmd);
		}
		// end of LED_CTRL cmd

		// start of SENSOR_READ cmd
		// TODO: Check RXNE flag in register window to see if its cleared.
		// We will see if it is necessary to clear RXNE prior to transmitting more data
		// wait until button is pressed
		while ( !GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) );
		// avoiding button de-bouncing issues.   200 ms of delay
		delay(250);
		//  Send command to slave
		cmd = COMMAND_SENSOR_READ;
		SPI_SendData(spi2_peripheral.pSPIx, &cmd, sizeof(cmd));
		// clear RXNE flag by removing garbage data from Rxbuffer
		SPI_ReceiveData(spi2_peripheral.pSPIx, &dummyRead, sizeof(dummyRead));

		// send dummy data to Txbuffer receive ACK/NACK response
		SPI_SendData(spi2_peripheral.pSPIx, &dummyWrite, sizeof(dummyWrite));
		SPI_ReceiveData(spi2_peripheral.pSPIx, &response, sizeof(cmd));
		if(response == ACK){
			arg[0] = ANALOG_PIN0;
			// send LED pin number to slave
			SPI_SendData(spi2_peripheral.pSPIx, arg, 1);
			SPI_ReceiveData(spi2_peripheral.pSPIx, &dummyRead, sizeof(dummyRead));

			SPI_SendData(spi2_peripheral.pSPIx, &dummyWrite, sizeof(dummyWrite));
			// receive analog value from analog pin requested
			uint8_t analogVal;
			SPI_ReceiveData(spi2_peripheral.pSPIx, &analogVal, sizeof(&pinState));
			printf("Analog pin value read: %d \n",analogVal);
		}else{
				// printf error message to terminal
				printf("Error! Slave did not recognize command: %x\n",cmd);
		}
		// end of SENSOR_READ cmd


		// start of LED_READ cmd
		// TODO: Check RXNE flag in register window to see if its cleared.
		// We will see if it is necessary to clear RXNE prior to transmitting more data
		// wait until button is pressed
		while ( !GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) );
		// avoiding button de-bouncing issues.   200 ms of delay
		delay(250);
		//  Send command to slave
		cmd = COMMAND_LED_READ;
		SPI_SendData(spi2_peripheral.pSPIx, &cmd, sizeof(cmd));
		// clear RXNE flag by removing garbage data from Rxbuffer
		SPI_ReceiveData(spi2_peripheral.pSPIx, &dummyRead, sizeof(dummyRead));

		// send dummy data to Txbuffer receive ACK/NACK response
		SPI_SendData(spi2_peripheral.pSPIx, &dummyWrite, sizeof(dummyWrite));
		SPI_ReceiveData(spi2_peripheral.pSPIx, &response, sizeof(cmd));
		if(response == ACK){
			arg[0] = LED_PIN;
			// send LED pin number
			SPI_SendData(spi2_peripheral.pSPIx, arg, 1);
			SPI_ReceiveData(spi2_peripheral.pSPIx, &dummyRead, sizeof(dummyRead));

			SPI_SendData(spi2_peripheral.pSPIx, &dummyWrite, sizeof(dummyWrite));
			// receive LED pin state
			SPI_ReceiveData(spi2_peripheral.pSPIx, &response, sizeof(&pinState));
		}else{
			// printf error message to terminal
			printf("Error! Slave did not recognize command: %x\n",cmd);
		}
		// end of LED_READ cmd

		// start of PRINT cmd
		// TODO: Check RXNE flag in register window to see if its cleared.
		// We will see if it is necessary to clear RXNE prior to transmitting more data
		// wait until button is pressed
		while ( !GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) );
		// avoiding button de-bouncing issues.   200 ms of delay
		delay(250);
		//  Send command to slave
		cmd = COMMAND_PRINT;
		SPI_SendData(spi2_peripheral.pSPIx, &cmd, sizeof(cmd));
		// clear RXNE flag by removing garbage data from Rxbuffer
		SPI_ReceiveData(spi2_peripheral.pSPIx, &dummyRead, sizeof(dummyRead));

		// send dummy data to Txbuffer receive ACK/NACK response
		SPI_SendData(spi2_peripheral.pSPIx, &dummyWrite, sizeof(dummyWrite));
		SPI_ReceiveData(spi2_peripheral.pSPIx, &response, sizeof(cmd));
		if(response == ACK){
			uint8_t msg[] = "Let's go party like its 1980!!!";
			// length of msg
			arg[0] = strlen((char*)msg);
			// send length of message we want to print
			SPI_SendData(spi2_peripheral.pSPIx, arg,  sizeof(dummyRead));
			SPI_ReceiveData(spi2_peripheral.pSPIx, &dummyRead, sizeof(dummyRead));
			delay(250);
			// send msg contents
			for(uint8_t i = 0; i < arg[0]; i++){
				SPI_SendData(spi2_peripheral.pSPIx, msg, arg[0]);
				SPI_ReceiveData(spi2_peripheral.pSPIx, &dummyRead, sizeof(dummyRead));
			}
			printf("Message Sent.\n");
		}else{
			// printf error message to terminal
			printf("Error! Slave did not recognize command: %x\n",cmd);
		}
		// end of PRINT cmd

		// start of ID_READ cmd
		// TODO: Check RXNE flag in register window to see if its cleared.
		// We will see if it is necessary to clear RXNE prior to transmitting more data
		// wait until button is pressed
		while ( !GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) );
		// avoiding button de-bouncing issues.   200 ms of delay
		delay(250);
		//  Send command to slave
		cmd = COMMAND_ID_READ;
		SPI_SendData(spi2_peripheral.pSPIx, &cmd, sizeof(cmd));
		// clear RXNE flag by removing garbage data from Rxbuffer
		SPI_ReceiveData(spi2_peripheral.pSPIx, &dummyRead, sizeof(dummyRead));

		// send dummy data to Txbuffer receive ACK/NACK response
		SPI_SendData(spi2_peripheral.pSPIx, &dummyWrite, sizeof(dummyWrite));
		SPI_ReceiveData(spi2_peripheral.pSPIx, &response, sizeof(cmd));
		if(response == ACK){
			uint8_t slaveId[11] = {'\0'};
			// receive ID of slave device
			SPI_SendData(spi2_peripheral.pSPIx, &dummyWrite,  sizeof(dummyWrite));
			SPI_ReceiveData(spi2_peripheral.pSPIx, slaveId, sizeof(slaveId));
			slaveId[11] = '\0';
			printf("The message is: %s\n",slaveId);
		}else{
			// printf error message to terminal
			printf("Error! Slave did not recognize command: %x\n",cmd);
		}
		// end of ID_READ cmd


	}
	return 0;
}
