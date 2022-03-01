/*
 * stm32f407xx_spi_driver.h
 *
 *  Created on: Oct 29, 2020
 *      Author: class
 */

#ifndef INC_STM32F407XX_SPI_DRIVER_H_
#define INC_STM32F407XX_SPI_DRIVER_H_

#include "stm32f407xx.h"
/*
 *		Configuration Structure for SPIx Peripheral
 */
typedef struct
{
	uint8_t SPI_deviceMode;
	uint8_t	SPI_busConfig;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t	SPI_CPHA;
	uint8_t SPI_SSM;
	uint8_t SPI_SclkSpeed;
}SPI_Config_t;

/*
 * 		Handle Structure for SPIx Peripheral
 */
typedef struct
{
	SPI_RegDef_t *pSPIx;		// Holds base address of SPIx(x: 1,2,3) Peripheral
	SPI_Config_t SPIConfig;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t txLen;
	uint32_t rxLen;
	uint8_t  txState;
	uint8_t  rxState;
}SPI_Handle_t;

/*
 * SPI_DeviceMode
 */
#define SPI_DEVICE_MODE_MASTER	1
#define SPI_DEVICE_MODE_SLAVE	0

/*
 * SPI_BusConfig
 */
#define SPI_BUS_CONFIG_FD			1
#define SPI_BUS_CONFIG_HD_TXONLY	2
#define SPI_BUS_CONFIG_HD_RXONLY	3
#define SPI_BUS_CONFIG_SPLX_RXONLY	4

/*
 * SPI_SClkSpeed (supplied by baud rate generator)
 */
#define SPI_SCLK_SPEED_DIV2			0
#define SPI_SCLK_SPEED_DIV4			1
#define SPI_SCLK_SPEED_DIV8			2
#define SPI_SCLK_SPEED_DIV16		3
#define SPI_SCLK_SPEED_DIV32		4
#define SPI_SCLK_SPEED_DIV64		5
#define SPI_SCLK_SPEED_DIV128		6
#define SPI_SCLK_SPEED_DIV256		7

/*
 * SPI_DFF
 */
#define SPI_DFF_8BITS	0
#define SPI_DFF_16BITS	1

/*
 * SPI_CPOL
 */
#define SPI_CPOL_HIGH	1
#define SPI_CPOL_LOW	0

/*
 * SPI_CPHA
 */
#define	SPI_CPHA_HIGH	1
#define SPI_CPHA_LOW	0

/*
 * SPI_SSM
 */
#define SPI_SSM_EN		1
#define SPI_SSM_DI		0	// default

/*
 *  SPI_ApplicationState
 */
#define SPI_READY			0
#define SPI_BUSY_IN_RX		1
#define SPI_BUSY_IN_TX		2

/*
 *  Possible SPI Application Events
 */
#define SPI_EVENT_TX_CMPLT	1
#define SPI_EVENT_RX_CMPLT	2
#define SPI_EVENT_OVR_ERR	3
#define SPI_EVENT_CRC_ERR	4
/******************************************************************************************
 *								APIs supported by this driver
 *		 For more information about the APIs check the function definitions
 ******************************************************************************************/

/*
 * Peripheral Clock setup
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

/*
 * 		Data Send and Receive ( blocking/non-interrupt APIs)
 */
void SPI_SendData(SPI_RegDef_t *pSPIx ,uint8_t *pTxbuffer, uint32_t len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxbuffer, uint32_t len);

/*
 * 		Data Send and Receive ( interrupt APIs)
 */
uint8_t I2C_SendData_IT(SPI_Handle_t *pSPIHandle, uint8_t *pTxbuffer, uint32_t len);
uint8_t SPI_ReceiveData_IT(SPI_Handle_t *pSPIHandle, uint8_t *pRxbuffer, uint32_t len);

/*
 * IRQ Configuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pSPIHandle);

/*
 *   Other peripheral control APIs
 */
void SPI_PeripheralControl( SPI_Handle_t *pSPIHandle, uint8_t EnOrDi );
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t Flag);
void SPI_SSIConfig( SPI_Handle_t *pSPIHandle, uint8_t EnOrDi );
void SPI_SSOEConfig( SPI_Handle_t *pSPIHandle, uint8_t EnOrDi );
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx);
void SPI_CloseTransmission( SPI_Handle_t *pSPIHandle);
void SPI_CloseReception( SPI_Handle_t *pSPIHandle);

/*
 *  Application Callback
 */
void SPI_ApplicationEventCallback( SPI_Handle_t *pSPIHandle, uint8_t SPI_EVENT);

#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */