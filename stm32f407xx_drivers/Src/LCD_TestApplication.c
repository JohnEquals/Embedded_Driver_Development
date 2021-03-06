/*
 *  TODO: Test lcd_init API for 8-bit mode.   Write APIs for the commands needed
 */

#include "stm32f407xx.h"
#include "1602a_lcd.h"
#include "dht11.h"
#include <stdint.h>
#include <stdio.h>


#define HIGH		1
#define LOW			0


// time is in milliseconds
void delay( uint32_t time)
{
	for( uint32_t i = 0; i < (time*1000); i++);
}

extern void initialise_monitor_handles(void);

extern uint16_t humidity;
extern uint16_t temperature;

int main(void)
{

	initialise_monitor_handles();
	//printf("Hello world1\n");
	//GPIO_Handle_t RS, RW, E, DB0, DB1, DB2, DB3, DB4, DB5, DB6, DB7;

	GPIO_Handle_t dht11_sensor; //use GPIO PD9

	// init dht11 pin (PD9)
	dht11_sensor.pGPIOx = GPIOD;
	dht11_sensor.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	dht11_sensor.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
	dht11_sensor.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	dht11_sensor.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	dht11_sensor.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&dht11_sensor);

	GPIO_WriteToOutputPin(dht11_sensor.pGPIOx, dht11_sensor.GPIO_PinConfig.GPIO_PinNumber, HIGH);
	//	delay(10);

	// send start signal
	GPIO_WriteToOutputPin(dht11_sensor.pGPIOx, dht11_sensor.GPIO_PinConfig.GPIO_PinNumber, LOW);
	// wait 18 ms
	delay(18);
	GPIO_WriteToOutputPin(dht11_sensor.pGPIOx, dht11_sensor.GPIO_PinConfig.GPIO_PinNumber, HIGH);
	dht11_sensor.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIO_Init(&dht11_sensor);
	// wait 40 ms
	delay(40);
	printf("Humidity = %i.%i\nTemp = %i.%i\n", humidity >> 4, humidity & 0x0f, temperature >> 4, temperature & 0x0f);

	printf("Hello world2\n");
	//dht11_get_data(&dht11_sensor);
	printf("Hello world3\n");
	printf("Humidity = %i.%i\nTemp = %i.%i\n", humidity >> 4, humidity & 0x0f, temperature >> 4, temperature & 0x0f);

#if 0
	// init RS pin (PC1)
	RS.pGPIOx = GPIOC;
	RS.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	RS.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_1;
	RS.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	RS.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	RS.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&RS);

	// init RW pin (PC0)
	RW.pGPIOx = GPIOC;
	RW.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	RW.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	RW.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	RW.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	RW.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&RW);

	// init E pin (PC3)
	E.pGPIOx = GPIOC;
	E.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	E.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_3;
	E.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	E.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	E.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&E);

	// init D0 pin (PC2)
	DB0.pGPIOx = GPIOC;
	DB0.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	DB0.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_2;
	DB0.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	DB0.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	DB0.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&DB0);

	// init D1 pin (PA1)
	DB1.pGPIOx = GPIOA;
	DB1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	DB1.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_1;
	DB1.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	DB1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	DB1.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&DB1);

	// init D2 pin (PA3)
	DB2.pGPIOx = GPIOA;
	DB2.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	DB2.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_3;
	DB2.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	DB2.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	DB2.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&DB2);

	// init D3 pin (PA2)
	DB3.pGPIOx = GPIOA;
	DB3.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	DB3.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_2;
	DB3.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	DB3.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	DB3.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&DB3);

	// init D4 pin (PA7)
	DB4.pGPIOx = GPIOA;
	DB4.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	DB4.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	DB4.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	DB4.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	DB4.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&DB4);

	// init D5 pin (PA6)
	DB5.pGPIOx = GPIOA;
	DB5.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	DB5.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	DB5.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	DB5.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	DB5.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&DB5);

	// init D6 pin (PC5)
	DB6.pGPIOx = GPIOC;
	DB6.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	DB6.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	DB6.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	DB6.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	DB6.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&DB6);

	// init D7 pin (PC4)
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
	double fnum = 14.0;
	char buff[50] = {0};
	sprintf(buff,"%f",fnum);
	//printf("Float val: %f\n", fnum);
	printf("test and %s", buff);
	//delay(50);
	//uint8_t LCD_init( uint8_t mode);
	lcd_clear_display(&RS, &RW, &E, &DB0, &DB1, &DB2, &DB3, &DB4, &DB5, &DB6, &DB7);
	lcd_return_home(&RS, &RW, &E, &DB0, &DB1, &DB2, &DB3, &DB4, &DB5, &DB6, &DB7);
	lcd_function_set(&RS, &RW, &E, &DB0, &DB1, &DB2, &DB3, &DB4, &DB5, &DB6, &DB7, (uint8_t)'A');



	//lcd_write_character_to_lcd(&RS, &RW, &E, &DB0, &DB1, &DB2, &DB3, &DB4, &DB5, &DB6, &DB7, (uint8_t)'A');
	uint8_t buffer[] = "Hello World\n";
	uint16_t data_siz = sizeof(buffer)/sizeof(uint8_t);
	//lcd_write_string_to_lcd( &RS, &RW, &E, &DB0, &DB1, &DB2, &DB3, &DB4, &DB5, &DB6, &DB7,  (uint8_t *)"Hello World\n", 12);


	//lcd_display_status_set(&RS, &RW, &E, &DB0, &DB1, &DB2, &DB3, &DB4, &DB5, &DB6, &DB7, 1);
	//lcd_display_status_set(&RS, &RW, &E, &DB0, &DB1, &DB2, &DB3, &DB4, &DB5, &DB6, &DB7, 1);
	//delay(1000);
	//for(uint8_t count = 0; count < 32; count++)
	//{
	//	lcd_write_character_to_lcd(&RS, &RW, &E, &DB0, &DB1, &DB2, &DB3, &DB4, &DB5, &DB6, &DB7, (uint8_t)'A');
	//}
	//lcd_return_home(&RS, &RW, &E, &DB0, &DB1, &DB2, &DB3, &DB4, &DB5, &DB6, &DB7);

#endif

	while(1)
	{
		//lcd_write_character_to_lcd(&RS, &RW, &E, &DB0, &DB1, &DB2, &DB3, &DB4, &DB5, &DB6, &DB7, (uint8_t)'A');
#if 0
		lcd_set_cgram_address(&RS, &RW, &E, &DB0, &DB1, &DB2, &DB3, &DB4, &DB5, &DB6, &DB7, 0x00);
		lcd_write_character_to_lcd(&RS, &RW, &E, &DB0, &DB1, &DB2, &DB3, &DB4, &DB5, &DB6, &DB7, (uint8_t)'L');
		delay(1000);
		lcd_set_cgram_address(&RS, &RW, &E, &DB0, &DB1, &DB2, &DB3, &DB4, &DB5, &DB6, &DB7, 0x0F);
		lcd_write_character_to_lcd(&RS, &RW, &E, &DB0, &DB1, &DB2, &DB3, &DB4, &DB5, &DB6, &DB7, (uint8_t)'R');
		delay(1000);
		lcd_set_cgram_address(&RS, &RW, &E, &DB0, &DB1, &DB2, &DB3, &DB4, &DB5, &DB6, &DB7, 0x3F);
		lcd_write_character_to_lcd(&RS, &RW, &E, &DB0, &DB1, &DB2, &DB3, &DB4, &DB5, &DB6, &DB7, (uint8_t)'L');
		delay(1000);
		lcd_set_cgram_address(&RS, &RW, &E, &DB0, &DB1, &DB2, &DB3, &DB4, &DB5, &DB6, &DB7, 0x4F);
		lcd_write_character_to_lcd(&RS, &RW, &E, &DB0, &DB1, &DB2, &DB3, &DB4, &DB5, &DB6, &DB7, (uint8_t)'R');
#endif
		//delay(10);
		;
	}

	return 0;
}


