/*
 *  TODO: Test lcd_init API for 8-bit mode.   Write APIs for the commands needed
 */

#include "stm32f407xx.h"
#include <stdint.h>

#define MODE_8BIT 	8
#define MODE_4BIT 	4
#define HIGH		1
#define LOW			0

// time is in milliseconds
void delay( uint32_t time){
	for( uint8_t i = 0; i < (time*2000); i++);
}

void pulseEnablePin( GPIO_Handle_t *E ){
	GPIO_WriteToOutputPin(E -> pGPIOx, E -> GPIO_PinConfig.GPIO_PinNumber, HIGH);
	delay(2);
	GPIO_WriteToOutputPin(E ->pGPIOx, E -> GPIO_PinConfig.GPIO_PinNumber, LOW);
}

void LCD_init( GPIO_Handle_t *RS, GPIO_Handle_t *RW, GPIO_Handle_t *E, GPIO_Handle_t *DB0, GPIO_Handle_t *DB1, GPIO_Handle_t *DB2, GPIO_Handle_t *DB3, GPIO_Handle_t *DB4, GPIO_Handle_t *DB5, GPIO_Handle_t *DB6, GPIO_Handle_t *DB7,uint8_t mode){
	// initialization protocol for 8-bit mode
	if( mode == MODE_8BIT ){
		// wait at least 15 ms
		delay(20);
		/*
		GPIO_WriteToOutputPin(RS -> pGPIOx, RS ->GPIO_PinConfig.GPIO_PinNumber, LOW);
		GPIO_WriteToOutputPin(RW -> pGPIOx, RW ->GPIO_PinConfig.GPIO_PinNumber, LOW);
		GPIO_WriteToOutputPin(DB0 -> pGPIOx, DB0 ->GPIO_PinConfig.GPIO_PinNumber, LOW);
		GPIO_WriteToOutputPin(DB1 -> pGPIOx, DB1 ->GPIO_PinConfig.GPIO_PinNumber, LOW);
		GPIO_WriteToOutputPin(DB2 -> pGPIOx, DB2 ->GPIO_PinConfig.GPIO_PinNumber, LOW);
		GPIO_WriteToOutputPin(DB3 -> pGPIOx, DB3 ->GPIO_PinConfig.GPIO_PinNumber, LOW);
		*/
		GPIO_WriteToOutputPin(DB4 -> pGPIOx, DB4 ->GPIO_PinConfig.GPIO_PinNumber, HIGH);
		GPIO_WriteToOutputPin(DB5 -> pGPIOx, DB5 ->GPIO_PinConfig.GPIO_PinNumber, HIGH);
		pulseEnablePin(E);
		// wait at least 4.1 ms
		delay(5);
		pulseEnablePin(E);
		// wait at least 100 us
		pulseEnablePin(E);
		// function set
		GPIO_WriteToOutputPin(DB2 -> pGPIOx, DB2 ->GPIO_PinConfig.GPIO_PinNumber, HIGH);
		GPIO_WriteToOutputPin(DB3 -> pGPIOx, DB3 ->GPIO_PinConfig.GPIO_PinNumber, HIGH);
		pulseEnablePin(E);
		// display off
		GPIO_WriteToOutputPin(DB4 -> pGPIOx, DB4 ->GPIO_PinConfig.GPIO_PinNumber, LOW);
		GPIO_WriteToOutputPin(DB5 -> pGPIOx, DB5 ->GPIO_PinConfig.GPIO_PinNumber, LOW);
		GPIO_WriteToOutputPin(DB2 -> pGPIOx, DB2 ->GPIO_PinConfig.GPIO_PinNumber, LOW);
		pulseEnablePin(E);
		// display clear
		GPIO_WriteToOutputPin(DB3 -> pGPIOx, DB3 ->GPIO_PinConfig.GPIO_PinNumber, LOW);
		GPIO_WriteToOutputPin(DB0 -> pGPIOx, DB0 ->GPIO_PinConfig.GPIO_PinNumber, HIGH);
		// entry mode set
		GPIO_WriteToOutputPin(DB2 -> pGPIOx, DB2 ->GPIO_PinConfig.GPIO_PinNumber, HIGH);
		GPIO_WriteToOutputPin(DB1 -> pGPIOx, DB1 ->GPIO_PinConfig.GPIO_PinNumber, HIGH);
		GPIO_WriteToOutputPin(DB0 -> pGPIOx, DB0 ->GPIO_PinConfig.GPIO_PinNumber, LOW);
		pulseEnablePin(E);
	}
}

int main(void){

	GPIO_Handle_t RS, RW, E, DB0, DB1, DB2, DB3, DB4, DB5, DB6, DB7;
	// init RS pin
	RS.pGPIOx = GPIOC;
	RS.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	RS.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_1;
	RS.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	RS.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	RS.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&RS);

	// init RW pin
	RW.pGPIOx = GPIOC;
	RW.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	RW.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	RW.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	RW.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	RW.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&RW);

	// init E pin
	E.pGPIOx = GPIOC;
	E.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	E.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_3;
	E.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	E.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	E.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&E);

	// init DB0 pin
	DB0.pGPIOx = GPIOC;
	DB0.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	DB0.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_2;
	DB0.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	DB0.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	DB0.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&DB0);

	// init DB1 pin
	DB1.pGPIOx = GPIOA;
	DB1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	DB1.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_1;
	DB1.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	DB1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	DB1.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&DB1);

	// init DB2 pin
	DB2.pGPIOx = GPIOA;
	DB2.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	DB2.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_3;
	DB2.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	DB2.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	DB2.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&DB2);

	// init DB3 pin
	DB3.pGPIOx = GPIOA;
	DB3.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	DB3.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_2;
	DB3.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	DB3.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	DB3.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&DB3);

	// init DB4 pin
	DB4.pGPIOx = GPIOA;
	DB4.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	DB4.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	DB4.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	DB4.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	DB4.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&DB4);

	// init DB5 pin
	DB5.pGPIOx = GPIOA;
	DB5.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	DB5.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	DB5.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	DB5.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	DB5.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&DB5);

	// init DB6 pin
	DB6.pGPIOx = GPIOC;
	DB6.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	DB6.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	DB6.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	DB6.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	DB6.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&DB6);

	// init DB7 pin
	DB7.pGPIOx = GPIOC;
	DB7.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	DB7.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_4;
	DB7.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	DB7.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	DB7.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&DB7);

	GPIO_WriteToOutputPin(RS.pGPIOx, RS.GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(RW.pGPIOx, RW.GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(E.pGPIOx, E.GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB0.pGPIOx, DB0.GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB1.pGPIOx, DB1.GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB2.pGPIOx, DB2.GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB3.pGPIOx, DB3.GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB4.pGPIOx, DB4.GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB5.pGPIOx, DB5.GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB6.pGPIOx, DB6.GPIO_PinConfig.GPIO_PinNumber, LOW);
	GPIO_WriteToOutputPin(DB7.pGPIOx, DB7.GPIO_PinConfig.GPIO_PinNumber, LOW);

	//uint8_t LCD_init( uint8_t mode);

	return 0;
}
