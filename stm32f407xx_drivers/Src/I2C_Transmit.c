

#include "stm32f407xx.h"

/*
 *  Set PB8 - I2C1 SCL
 *      PB7 - I2C1 SDA
 */

void init_i2c1_pins(void);
void init_i2c1(void);

int main(void){

	init_i2c1_pins();
	init_i2c1();


	return 0;
}


void init_i2c1_pins(void){

	GPIO_Handle_t i2c1_scl_pin = {
			.pGPIOx = GPIOB,
			.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8,
			.GPIO_PinConfig.GPIO_PinMode   = GPIO_MODE_ALTFN,
			.GPIO_PinConfig.GPIO_PinSpeed  = GPIO_SPEED_FAST,
			.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU,
			.GPIO_PinConfig.GPIO_PinOPType		 = GPIO_OP_TYPE_OD,
			.GPIO_PinConfig.GPIO_PinAltFunMode  = 4
	};

	GPIO_Handle_t i2c1_sda_pin = {
			.pGPIOx = GPIOB,
			.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8,
			.GPIO_PinConfig.GPIO_PinMode   = GPIO_MODE_ALTFN,
			.GPIO_PinConfig.GPIO_PinSpeed  = GPIO_SPEED_FAST,
			.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU,
			.GPIO_PinConfig.GPIO_PinOPType		 = GPIO_OP_TYPE_OD,
			.GPIO_PinConfig.GPIO_PinAltFunMode  = 4
	};

	GPIO_Init(&i2c1_sda_pin);
	GPIO_Init(&i2c1_scl_pin);

}

void init_i2c1(void){

	I2C_Handle_t i2c1 = {

			.I2Cconfig.sclSpeed = I2C_SCL_SPEED_SM,
			.I2Cconfig.ackCtrl = I2C_ACK_ENABLE,
			.I2Cconfig.fmDutyCycle = I2C_FM_DUTY_2
			.pI2Cx = I2C1,
	};


	I2C_Init(&i2c1);
}
