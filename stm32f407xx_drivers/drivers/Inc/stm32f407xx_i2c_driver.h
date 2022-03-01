/*
 * stm32f407xx_i2c_driver.h
 *
 *  Created on: Mar 5, 2021
 *      Author: class
 */

#ifndef INC_STM32F407XX_I2C_DRIVER_H_
#define INC_STM32F407XX_I2C_DRIVER_H_

#include "stm32f407xx.h"

/*
 * 		I2Cx Peripheral Configuration Macros
 */

/*
 *  I2C_SCLSPEED
*/
#define I2C_SCL_SPEED_SM	100000  // Standard Mode: 100 kHz
#define I2C_SCL_SPEED_FM_4k	400000	// Fast Mode: 	  400 kHz
#define I2C_SCL_SPEED_FM_2k	200000	// Fast Mode: 	  200 kHz
/*
 *  I2C_ACKControl
*/
#define I2C_ACK_ENABLE		1
#define I2C_ACK_DISABLE		0
/*
 *  I2C_FM_DUTY_CYCLE
*/
#define I2C_FM_DUTY_2		0
#define I2C_FM_DUTY_16_9	1

#define ABP1_CLK_SPEED		16

typedef struct{
	uint32_t	sclSpeed;
	uint8_t 	deviceAddr;
	uint8_t 	ackCtrl;
	uint8_t 	fmDutyCycle;
}I2C_Config_t;

typedef struct{
	I2C_Config_t 	I2Cconfig;
	I2C_RegDef_t*	pI2Cx;
}I2C_Handle_t;

/*
 * 	I2C SR1 Flags
 */
#define I2C_SB_FLAG			(1<<I2C_SR1_SB)
#define I2C_ADDR_FLAG		(1<<I2C_SR1_ADDR)
#define I2C_BTF_FLAG		(1<<I2C_SR1_BTF)
#define I2C_ADD10_FLAG		(1<<I2C_SR1_ADD10)
#define I2C_STOPF_FLAG		(1<<I2C_SR1_STOPF)
#define I2C_RXNE_FLAG		(1<<I2C_SR1_RXNE)
#define I2C_TXE_FLAG		(1<<I2C_SR1_TXE)
#define I2C_BERR_FLAG		(1<<I2C_SR1_BERR)
#define I2C_ARLO_FLAG		(1<<I2C_SR1_ARLO)
#define I2C_AF_FLAG			(1<<I2C_SR1_AF)
#define I2C_OVR_FLAG		(1<<I2C_SR1_OVR)
#define I2C_TIMEOUT_FLAG	(1<<I2C_SR1_TIMEOUT)


/******************************************************************************************
 *								APIs supported by this driver
 *		 For more information about the APIs check the function definitions
 ******************************************************************************************/
/*
 * Peripheral Clock setup
 */
void I2C_PeripheralClkCtrl(I2C_RegDef_t* pI2Cx, uint8_t EnableOrDisable);

/*
 * Init and De-init
 */
void I2C_Init(I2C_Handle_t *I2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

/*
 * 		Data Send and Receive ( blocking/non-interrupt APIs)
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxbuffer, uint8_t len, uint8_t SlaveAddr);

/*
 * 		Data Send and Receive ( interrupt APIs)
 */

/*
 * IRQ Configuration and ISR handling
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
/*
 *   Other peripheral control APIs
 */
void I2C_PeripheralControl( I2C_Handle_t *pI2CHandle, uint8_t EnOrDi );
uint8_t I2C_GetFlagStatusSR1(I2C_RegDef_t *pI2Cx, uint32_t Flag);
/*
 *  Application Callback
 */
void I2C_ApplicationEventCallback( I2C_Handle_t *pI2CHandle, uint8_t I2C_EVENT);

#endif /* INC_STM32F407XX_I2C_DRIVER_H_ */
