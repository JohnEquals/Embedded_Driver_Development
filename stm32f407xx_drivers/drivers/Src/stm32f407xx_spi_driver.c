/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: Oct 29, 2020
 *      Author: class
 */

#include "stm32f407xx_spi_driver.h"

static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_ovr_interrupt_handle(SPI_Handle_t *pSPIHandle);
/*********************************************************************
 * @fn      		  - SPI_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given SPIx peripheral
 *
 * @param[in]         - base address of the SPIx peripheral
 * @param[in]         - ENABLE or DISABLE macros
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if( pSPIx == SPI1)
			SPI1_PCLK_EN();
		else if( pSPIx == SPI2)
			SPI2_PCLK_EN();
		else if( pSPIx == SPI3)
			SPI3_PCLK_EN();
	}
	else if(EnorDi == DISABLE)
	{
		if( pSPIx == SPI1)
			SPI1_PCLK_DI();
		else if( pSPIx == SPI2)
			SPI2_PCLK_DI();
		else if( pSPIx == SPI3)
			SPI3_PCLK_DI();
	}

}

/*********************************************************************
 * @fn      		  - SPI_Init
 *
 * @brief             - Initializes SPIx peripheral to configuration settings
 * 						specified in SPIx object
 *
 * @param[in]         - pointer to SPIx peripheral handle
 *
 * @return            -  none
 *
 * @Note              -  This API only initializes the SPIx peripheral.  To enable/disable SPIx, use SPI_PeripheralControl API.
 */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	// create a temp variable to store the bit configuration settings which we initialize to the default values of CR1
	uint32_t tempReg = 0;

	// enable clock for SPIx peripheral
	SPI_PeriClockControl(pSPIHandle -> pSPIx, ENABLE);

	// 1. configure device mode
	if(pSPIHandle->SPIConfig.SPI_deviceMode)
		tempReg |= ((pSPIHandle -> SPIConfig.SPI_deviceMode) << SPI_CR1_MSTR);

	// 2. configure bus configuration
	if(pSPIHandle->SPIConfig.SPI_busConfig == SPI_BUS_CONFIG_FD)  // full-duplex
	{
		// 2-unidirectional lines
		tempReg &= ~(1 << SPI_CR1_BIDIMODE);
		// full-duplex for Rx and Tx
		tempReg &= ~(1 << SPI_CR1_RXONLY);
	}
	else if( pSPIHandle -> SPIConfig.SPI_busConfig == SPI_BUS_CONFIG_HD_RXONLY)  // half-duplex
	{
		// 1 bidirectional line
		tempReg |= 1 << SPI_CR1_BIDIMODE;
	}
	else if( pSPIHandle -> SPIConfig.SPI_busConfig == SPI_BUS_CONFIG_SPLX_RXONLY) // Simplex:  Rx mode only
	{
		// start with full-duplex config which means enable 2 unidirectional data lines
		tempReg &= ~(1 << SPI_CR1_BIDIMODE);
		// set device in Rx mode
		tempReg |= 1 << SPI_CR1_RXONLY;
	}

	// 3. configure data frame format
	if(pSPIHandle -> SPIConfig.SPI_DFF)
		tempReg |= 1 << SPI_CR1_DFF;

	// 4. configure clock polarity
	if(pSPIHandle -> SPIConfig.SPI_CPOL)  // if CPOL is not default value ( default CPOL = 0)
		tempReg |= 1 << SPI_CR1_CPOL;  			  // set CPOL = 1

	// 5. configure clock phase
	if(pSPIHandle -> SPIConfig.SPI_CPHA)  // if CPHA is not default value
		tempReg |= 1 << SPI_CR1_CPHA; 				  // set CPHA = 1

	// 6. configure software slave management
	if(pSPIHandle -> SPIConfig.SPI_SSM)
		tempReg |= 1 << SPI_CR1_SSM;				  // enable SSM if we are using it

	// 7. Configure serial clock speed
	// by default, SCLK = f_PeripheralClock/2.  If we want lower clock speed, modify settings
	if( pSPIHandle -> SPIConfig.SPI_SclkSpeed)
	{
		tempReg |= (pSPIHandle -> SPIConfig.SPI_SclkSpeed) << SPI_CR1_BR;
	}

	// here, we store our register values into the actual control register
	pSPIHandle -> pSPIx-> CR1 = tempReg;
}

/*********************************************************************
 * @fn      		  - SPI_DeInit
 *
 * @brief             - De-initializes SPIx peripheral
 *
 * @param[in]         - base address of the SPIx peripheral
 *
 * @return            -  none
 *
 * @Note              -  Puts SPIx register to reset values
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	// Set bitfield in Reset Register
	if( pSPIx == SPI1)
		SPI1_REG_RESET();
	else if( pSPIx == SPI2)
		SPI2_REG_RESET();
	else if( pSPIx == SPI3)
		SPI3_REG_RESET();
}

/*********************************************************************
 * @fn      		  - SPI_GetFlagStatus
 *
 * @brief             - This function returns the given flag's status from SPIx status register
 *
 * @param[in]         - pointer to SPIx peripheral handle
 * @param[in]         - flag bitfield
 *
 * @return            -  returns status of flag
 *
 * @Note              -  none
 */
uint8_t SPI_GetFlagStatus ( SPI_RegDef_t *pSPIx, uint32_t Flag)
{
	if(pSPIx->SR & Flag)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*********************************************************************
 * @fn      		  - SPI_SSIConfig
 *
 * @brief             - Configures the value of the SSI bit in SPI_CR1
 *
 * @param[in]         - pointer to SPIx peripheral handle
 * @param[in]         - enable or disable
 *
 * @return            -  none
 *
 * @Note              -  When SSM bit is set, the value of SSI bit becomes the value of NSS pin
 */
void SPI_SSIConfig( SPI_Handle_t *pSPIHandle, uint8_t EnOrDi )
{
	// if enabling, then set SSI bit
	if ( EnOrDi == ENABLE)
		pSPIHandle -> pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	else
		pSPIHandle -> pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
}

/*********************************************************************
 * @fn      		  - SPI_SSEConfig
 *
 * @brief             - Configures the value of the SSOE bit in SPI_CR2.  Controls the value of NSS pin when SSOE is set.
 *
 * @param[in]         - pointer to SPIx peripheral handle
 * @param[in]         - enable or disable
 *
 * @return            -  none
 *
 * @Note              -  SSOE is Slave Select Output Enable.  When SSOE = 1 (meaning NSS is using hardware slave management),
 * 						 setting SPE will force NSS pin LOW and clearing SPE will force NSS pin HIGH.
 */
void SPI_SSOEConfig( SPI_Handle_t *pSPIHandle, uint8_t EnOrDi )
{
	// if enabling, then set SSOE bit
	if ( EnOrDi == ENABLE)
		pSPIHandle -> pSPIx->CR2 |= (1 << SPI_CR2_SSOE);
	else
		pSPIHandle -> pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);
}

/*********************************************************************
 * @fn      		  - SPI_Enable
 *
 * @brief             - Sets the SPE bitfield in SPI_CR1 register to enable SPIx peripheral
 *
 * @param[in]         - pointer to SPIx peripheral handle
 *
 * @return            -  none
 *
 * @Note              -  Only call SPI_Enable after configuring and initializing SPIx peripheral.
 * 						 If user wants to change the settings for SPIx peripheral, they must reset SPE bitfield (call SPI_Disable)
 * 						 before changing SPIx settings.  Then, call SPI_Enable again.
 */
void SPI_PeripheralControl ( SPI_Handle_t *pSPIHandle, uint8_t EnOrDi )
{
	if ( EnOrDi == ENABLE)
		pSPIHandle -> pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	else
		pSPIHandle -> pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
}

/*********************************************************************
 * @fn      		  - SPI_SendData
 *
 * @brief             - This function sends data over the given SPIx peripheral
 *
 * @param[in]         - base address of the SPIx peripheral
 * @param[in]         - ptr to Tx buffer which contains the data
 * @param[in]         - length of buffer
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void SPI_SendData(SPI_RegDef_t *pSPIx ,uint8_t *pTxbuffer, uint32_t len)
{
	while( len != 0)  // while we haven't sent all our data
	{
		// wait if the Tx buffer is not empty
		// ver.1
		// while(!(pSPIx->SR & (1 << SPI_SR_TXE));
		// ver.2
		while( SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);
		uint16_t data = 0;
		if( pSPIx -> CR1 & (1 << SPI_CR1_DFF) ) // if DFF is 16 bits
		{
			data |= *((uint16_t *)pTxbuffer);
			len = len - 2;
			pTxbuffer = pTxbuffer + 2;
		}else{
			data |= (uint16_t)*pTxbuffer;
			--len;
			pTxbuffer++;
		}
		pSPIx -> DR = data;
		/*
		// 2. Check DFF bit in CR
		if( pSPIx -> CR1 & (1 << SPI_CR1_DFF) ) // if DFF is 16 bits
		{
			// 16 bit DFF
			// 1. Load data into DR
			pSPIx->DR = *((uint16_t *)pTxbuffer);
			len = len - 2; // decrease length by 2 bytes
			pTxbuffer = pTxbuffer + 2; // increment buffer address by 2 bytes
		}else{
			pSPIx->DR = *pTxbuffer;
			--len;
			pTxbuffer++;
		}
		*/
	}
}

/*********************************************************************
 * @fn      		  - SPI_ReceiveData
 *
 * @brief             - This function receives data from MISO line over the given SPIx peripheral
 *
 * @param[in]         - base address of the SPIx peripheral
 * @param[in]         - ptr to Rx buffer which contains the data
 * @param[in]         - length of buffer
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxbuffer, uint32_t len)
{
	while( len != 0)  // while we haven't received all our data
	{
		// wait until Rx buffer is not empty (full)
		// ver.1
		// while(!(pSPIHandle->pSPIx->SR & (1 << SPI_SR_RXNE));
		// ver.2
		while( SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

		if(pSPIx -> CR1 & (1 << SPI_CR1_DFF) ) // if DFF is 16 bits
		{
			*((uint16_t*)pRxbuffer) = pSPIx -> DR;
			len = len - 2;
			pRxbuffer = pRxbuffer + 2;
		}else{   // else DFF is 8 bits
			*pRxbuffer = (uint8_t)(pSPIx -> DR);
			--len;
			pRxbuffer++;
		}
	}
}

/*********************************************************************
 * @fn      		  - SPI_SendData_IT
 *
 * @brief             - This function sends data over the given SPIx peripheral
 *
 * @param[in]         - base address of the SPIx peripheral
 * @param[in]         - ptr to Tx buffer which contains the data
 * @param[in]         - length of buffer
 *
 * @return            -  none
 *
 * @Note              -  none
 */
uint8_t I2C_SendData_IT(SPI_Handle_t *pSPIHandle ,uint8_t *pTxbuffer, uint32_t len)
{
	// only execute this code if the state of SPI Tx communication is free/ready
		if( pSPIHandle -> txState != SPI_BUSY_IN_TX)
		{
		// 1. Save Tx buffer address and length info in some global variables
		(pSPIHandle -> pTxBuffer) = pTxbuffer;
		pSPIHandle -> txLen = len;

		// 2.  Mark SPI state as busy in transmission so no other code can
		//	   take over SPIx peripheral until transmission is completed
		pSPIHandle -> txState = SPI_BUSY_IN_TX;

		// 3.  Enable TXEIE control bit to generate interrupt when TXE flag set in SR
		pSPIHandle -> pSPIx -> CR2 |= (1 << SPI_CR2_TXEIE);
		}
	// 4.  Data transmission is handled by ISR code (implement later)
	return pSPIHandle -> txState;
}
/*********************************************************************
 * @fn      		  - SPI_ReceiveData_IT
 *
 * @brief             - This function receives data from MISO line over the given SPIx peripheral
 *
 * @param[in]         - base address of the SPIx peripheral
 * @param[in]         - ptr to Rx buffer which contains the data
 * @param[in]         - length of buffer
 *
 * @return            -  none
 *
 * @Note              -  none
 */
uint8_t SPI_ReceiveData_IT(SPI_Handle_t *pSPIHandle, uint8_t *pRxbuffer, uint32_t len)
{
	// only execute this code if the state of SPI Rx communication is free/ready
		if( pSPIHandle -> rxState != SPI_BUSY_IN_RX)
		{
		// 1. Save Rx buffer address and length info in some global variables
		(pSPIHandle -> pRxBuffer) = pRxbuffer;
		pSPIHandle -> rxLen = len;

		// 2.  Mark SPI state as busy in transmission so no other code can
		//	   take over SPIx peripheral until transmission is completed
		pSPIHandle -> rxState = SPI_BUSY_IN_RX;

		// 3.  Enable RXEIE control bit to generate interrupt when TXE flag set in SR
		pSPIHandle -> pSPIx -> CR2 |= (1 << SPI_CR2_RXNEIE);
		}
	// 4.  Data reception is handled by ISR code (implement later)
	return pSPIHandle -> rxState;
}
/*********************************************************************
 * @fn      		  - GPIO_IRQConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{

	if(EnorDi == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			//program ISER0 register
			*NVIC_ISER0 |= ( 1 << IRQNumber );

		}else if(IRQNumber > 31 && IRQNumber < 64 ) //32 to 63
		{
			//program ISER1 register
			*NVIC_ISER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 64 && IRQNumber < 96 )
		{
			//program ISER2 register //64 to 95
			*NVIC_ISER2 |= ( 1 << (IRQNumber % 64) );
		}
	}else
	{
		if(IRQNumber <= 31)
		{
			//program ICER0 register
			*NVIC_ICER0 |= ( 1 << IRQNumber );
		}else if(IRQNumber > 31 && IRQNumber < 64 )
		{
			//program ICER1 register
			*NVIC_ICER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 64 && IRQNumber < 96 )
		{
			//program ICER2 register
			*NVIC_ICER2 |= ( 1 << (IRQNumber % 64) );
		}
	}

}



/*********************************************************************
 * @fn      		  - SPI_IRQPriorityConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -
 */
void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	//1. first lets find out the ipr register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section  = IRQNumber %4 ;
	uint8_t shift_amount = ( 8 * iprx_section) + ( 8 - NO_PR_BITS_IMPLEMENTED) ;
	*(  NVIC_PR_BASE_ADDR + iprx ) |=  ( IRQPriority << shift_amount );
}

void SPI_IRQHandler(SPI_Handle_t *pSPIHandle)
{
	uint8_t bFlag, bInterrupt;
	// first, check for TXE flag
	bFlag = pSPIHandle -> pSPIx -> SR & (1<<SPI_SR_TXE);
	bInterrupt = pSPIHandle -> pSPIx -> CR2 & (1<<SPI_CR2_TXEIE);
	// if TXE flag set and interrupt enabled, send data
	if(bFlag && bInterrupt)
	{
		// Handle TXE
		spi_txe_interrupt_handle(pSPIHandle);
	}
	// next, check for RXNE flag
	bFlag = pSPIHandle -> pSPIx -> SR & (1<<SPI_SR_RXNE);
	bInterrupt = pSPIHandle -> pSPIx -> CR2 & (1<<SPI_CR2_RXNEIE);
	// if RXNE flag set and interrupt enabled, read from rxbuffer
	if(bFlag && bInterrupt)
	{
		// Handle RXNE
		spi_rxne_interrupt_handle(pSPIHandle);
	}
	// next, check for OVR (error) flag
	bFlag = pSPIHandle -> pSPIx -> SR & (1<<SPI_SR_OVR);
	bInterrupt = pSPIHandle -> pSPIx -> CR2 & (1<<SPI_CR2_ERRIE);
	// if ovr error flag set and interrupt enabled, do stuff
	if(bFlag && bInterrupt)
	{
		// Handle OVR
		spi_ovr_interrupt_handle(pSPIHandle);
	}
}
/*
 * 		Helper Functions
 */
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	if( pSPIHandle ->pSPIx -> CR1 & (1 << SPI_CR1_DFF) ) // if DFF is 16 bits
	{
		// 1. Load data into DR
		pSPIHandle->pSPIx->DR = *((uint16_t *)pSPIHandle ->pTxBuffer);
		pSPIHandle -> txLen = pSPIHandle -> txLen - 2; // decrease length by 2 bytes
		pSPIHandle->pTxBuffer = pSPIHandle->pTxBuffer + 2; // increment buffer address by 2 bytes
	}else{
		pSPIHandle -> pSPIx->DR = *(pSPIHandle -> pTxBuffer);
		--(pSPIHandle -> txLen);
		(pSPIHandle -> pTxBuffer)++;
	}
	// if we have no more data, end SPI transmission and let application know
	if(pSPIHandle -> txLen == 0)
	{
		// disable TXE interrupt (prevents future interrupts from TXE flag)
		pSPIHandle -> pSPIx -> CR2 = ~(1<<SPI_CR2_TXEIE);
		// Txbuffer is empty so set it to NULL so we know
		pSPIHandle -> pTxBuffer = NULL;
		// make sure txLen is set to 0
		pSPIHandle -> txLen = 0;
		// lets app know SPI is now ready
		pSPIHandle -> txState = SPI_READY;
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);
	}
}
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	if(pSPIHandle->pSPIx -> CR1 & (1 << SPI_CR1_DFF) ) // if DFF is 16 bits
		{
			*((uint16_t*)pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx -> DR;
			pSPIHandle->rxLen = pSPIHandle->rxLen - 2;
			pSPIHandle->pRxBuffer = pSPIHandle->pRxBuffer + 2;
		}else{   // else DFF is 8 bits
			*(pSPIHandle->pRxBuffer) = (uint8_t)(pSPIHandle->pSPIx -> DR);
			--(pSPIHandle->rxLen);
			(pSPIHandle->pRxBuffer)++;
		}
	// if we have no more data to retrieve, end SPI reception and let application know
	if(pSPIHandle -> rxLen == 0)
	{
		// disable RXNE interrupt (prevents future interrupts from TXE flag)
		pSPIHandle -> pSPIx -> CR2 = ~(1<<SPI_CR2_RXNEIE);
		// Rxbuffer is empty so set it to NULL so we know
		pSPIHandle -> pRxBuffer = NULL;
		// make sure txLen is set to 0
		pSPIHandle -> rxLen = 0;
		// lets app know SPI is now ready
		pSPIHandle -> rxState = SPI_READY;
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);
	}
}

static void spi_ovr_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	// clear OVR flag
	if(pSPIHandle ->txState != SPI_BUSY_IN_TX)
	{
		uint8_t temp;
		temp = pSPIHandle -> pSPIx -> DR;
		temp = pSPIHandle -> pSPIx -> SR;
		(void)temp;
	}
	//	inform application
	SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);
}

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{
	uint8_t temp;
	temp = pSPIx -> DR;
	temp = pSPIx -> SR;
	(void)temp;
}

void SPI_CloseTransmission( SPI_Handle_t *pSPIHandle)
{
	// disable TXE interrupt (prevents future interrupts from TXE flag)
	pSPIHandle -> pSPIx -> CR2 = ~(1<<SPI_CR2_TXEIE);
	// Txbuffer is empty so set it to NULL so we know
	pSPIHandle -> pTxBuffer = NULL;
	// make sure txLen is set to 0
	pSPIHandle -> txLen = 0;
	// lets app know SPI is now ready
	pSPIHandle -> txState = SPI_READY;
}

void SPI_CloseReception( SPI_Handle_t *pSPIHandle)
{
	// disable RXNE interrupt (prevents future interrupts from TXE flag)
	pSPIHandle -> pSPIx -> CR2 = ~(1<<SPI_CR2_RXNEIE);
	// Rxbuffer is empty so set it to NULL so we know
	pSPIHandle -> pRxBuffer = NULL;
	// make sure txLen is set to 0
	pSPIHandle -> rxLen = 0;
	// lets app know SPI is now ready
	pSPIHandle -> rxState = SPI_READY;
}

__attribute__((weak)) void SPI_ApplicationEventCallback( SPI_Handle_t *pSPIHandle, uint8_t SPI_EVENT)
{
	// this is a weak implementation.  User application may override this function
}
