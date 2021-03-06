/*
 * stm32f407xx_i2c_driver.c
 *
 *  Created on: Mar 5, 2021
 *      Author: class
 */

#include "stm32f407xx_i2c_driver.h"

uint16_t ahbDivisor[] = {2,4,8,16,32,64,128,256,512};
uint16_t apbDivisor[] = {2,4,8,16};

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddressPhase(I2C_RegDef_t *pI2Cx, uint8_t slaveAddr);
static void I2C_ClearAddressFlag(I2C_RegDef_t *pI2Cx);

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx){
	pI2Cx->CR1 |= (1<<I2C_CR1_START);
}

static void I2C_ExecuteAddressPhase(I2C_RegDef_t *pI2Cx, uint8_t slaveAddr){
	slaveAddr = (slaveAddr << 1);
	slaveAddr &= ~(0x1);  // slaveAddr is slave address (7 most signif. bits) + R/nW ( 1 least signif. bit).  R/nW bit = 0 for Write operation
	pI2Cx->DR = slaveAddr;
}

static void I2C_ClearAddressFlag(I2C_RegDef_t *pI2Cx){

	uint32_t dummyRead = pI2Cx->SR1;
	dummyRead = pI2Cx->SR1;
	(void)dummyRead;
}



void I2C_PeripheralClkCtrl(I2C_RegDef_t* pI2Cx, uint8_t EnableOrDisable){
	if(EnableOrDisable == ENABLE){
		if(pI2Cx == I2C1)
			I2C1_PCLK_EN();
		else if(pI2Cx == I2C2)
			I2C2_PCLK_EN();
		else if(pI2Cx == I2C3)
			I2C3_PCLK_EN();
	}else{
		if(pI2Cx == I2C1)
			I2C1_PCLK_DI();
		else if(pI2Cx == I2C2)
			I2C2_PCLK_DI();
		else if(pI2Cx == I2C3)
			I2C3_PCLK_DI();
	}
}

uint32_t RCC_GetPLLOutputClk(void){
	return 1;
}

uint32_t RCC_GetPCLK1Value(void){
	uint32_t pclk1;
	uint8_t clkSrc;
	uint32_t sysClk;
	clkSrc = (RCC->CFGR >>2)& 0x03;
	// find system clock source value
	if(clkSrc == 0)
		sysClk = 16000000;
	else if(clkSrc == 1)
		// stm32f407xx discovery board has an external 8 MHz crystal osc
		sysClk = 8000000;
	else if(clkSrc == 2)
		sysClk = RCC_GetPLLOutputClk();
	// find AHB prescale value
	uint8_t ahbPrescale = (RCC->CFGR >>4)& 0xF;
	uint16_t denominator = 1;
	if(ahbPrescale >= 8)
		denominator = ahbDivisor[ahbPrescale - 8];
	// calculate pclk1 after AHB prescaler
	pclk1 = sysClk/denominator;
	// find APB prescale value
	uint8_t apbPrescale = (RCC->CFGR >> 10)& 0x7;
	denominator = 1;
	if(apbPrescale >= 4)
		denominator = apbDivisor[apbPrescale - 4];
	// calculate pclk1 after APB prescaler
	pclk1 = pclk1/denominator;
	return pclk1;
}

void I2C_Init(I2C_Handle_t *pI2CHandle){
	uint32_t tempreg = 0;
	// enable clock
	I2C_PeripheralClkCtrl(pI2CHandle->pI2Cx, ENABLE);
	// configure ACK bit
	tempreg |= ((pI2CHandle->I2Cconfig.ackCtrl)<< I2C_CR1_ACK);
	pI2CHandle->pI2Cx->CR1 |= tempreg;
	// set peripheral speed
	tempreg = 0;
	uint32_t pclk1 = RCC_GetPCLK1Value();
	tempreg = pclk1 / 100000U;
	pI2CHandle->pI2Cx->CR2 |= ((tempreg & 0X3F) << I2C_CR2_FREQ);
	// program device's own address (slave mode only)
	tempreg = pI2CHandle->I2Cconfig.deviceAddr << 1;
	// set address mode to 7-bit
	tempreg |=  (1<<15);
	// required by device
	tempreg |=  (1<<14);
	pI2CHandle->pI2Cx->OAR1 |= tempreg;


	// CCR calculations
	uint16_t ccr;
	if(pI2CHandle->I2Cconfig.sclSpeed <= I2C_SCL_SPEED_SM){
		// device in standard mode
		pI2CHandle->pI2Cx->CCR &= ~(1<<I2C_CCR_FS);
		ccr = pclk1/(2 * pI2CHandle->I2Cconfig.sclSpeed);
	}else{
		// device in fast mode
		pI2CHandle->pI2Cx->CCR |= (1<<I2C_CCR_FS);
		// configure duty cycle
		if(pI2CHandle->I2Cconfig.fmDutyCycle == I2C_FM_DUTY_2){
			pI2CHandle->pI2Cx->CCR &= ~(1<<I2C_CCR_DUTY);
			ccr = pclk1/(3 * pI2CHandle->I2Cconfig.sclSpeed);
		}else if(pI2CHandle->I2Cconfig.fmDutyCycle == I2C_FM_DUTY_16_9){
			pI2CHandle->pI2Cx->CCR |= (1<<I2C_CCR_DUTY);
			ccr = pclk1/(25 * pI2CHandle->I2Cconfig.sclSpeed);
		}
	}
	// clear CCR bitfield
	tempreg = 0;
	tempreg = (ccr & 0xFFF);
	pI2CHandle->pI2Cx->CCR |= (ccr & 0xFFF);

	// TRISE calculation
	if(pI2CHandle->I2Cconfig.sclSpeed <= I2C_SCL_SPEED_SM){
		// device in standard mode so TRISE_MAX = 1 microsec
		tempreg = (pclk1/1000000U) + 1;  // tempreg stores TRISE value
	}else{
		// FM mode so TRISE_MAX = 300 ns
		tempreg = (pclk1 * 300/1000000000U) + 1;
	}
	// clear bits 5:0
	pI2CHandle->pI2Cx->TRISE &= 0x3F;
	// mask out all bits besides bits 5:0
	pI2CHandle->pI2Cx->TRISE |= (tempreg & 0x3F);
}

void I2C_DeInit(I2C_RegDef_t *pI2Cx){
	if(pI2Cx == I2C1){
		I2C1_REG_RESET();
	}else if(pI2Cx == I2C2){
		I2C2_REG_RESET();
	}else if(pI2Cx == I2C3){
		I2C3_REG_RESET();
	}
}

uint8_t I2C_GetFlagStatusSR1 ( I2C_RegDef_t *pI2Cx, uint32_t Flag)
{
	if(pI2Cx->SR1 & Flag)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxbuffer, uint8_t len, uint8_t slaveAddr)
{
	// 1. Generate START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	// 2. Confirm START generation is completed
	//	  Note: until SB is cleared, SCL is stretched (pulled LOW)
	while(!(I2C_GetFlagStatusSR1(pI2CHandle->pI2Cx, I2C_SB_FLAG) == FLAG_SET));

	// 3. Send address of slave with R/nW bit set to W mode(0).  (total: 8 bits)
	I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx, slaveAddr);
	// 4. Confirm address phase is completed by checking ADDR flag in SR1
	while(!(I2C_GetFlagStatusSR1(pI2CHandle->pI2Cx, I2C_ADDR_FLAG) == FLAG_SET));
	// 5. Clear ADDR Flag according to its software sequence
	//    Note: Until ADDR is cleared, SCL is stretched.
	I2C_ClearAddressFlag(pI2CHandle->pI2Cx);
	// 6.  Send data until len is 0.

	// 7.  When len becomes 0, wait for TxE=1 and BTF=1 before generating STOP condition
	// 	   Note:  TXE=1 and BTF=1 means both shift register and DR are empty and next transmission should begin.
	//     When BTF=1, SCL will be stretched.

	// 8.  Generate STOP condition.
	//	   Note:  Generating STOP will automatically clear BTF.  Master doesn't need to wait for STOP condition to complete.
}



void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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
 * @fn      		  - I2C_IRQPriorityConfig
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
void I2C_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	//1. first lets find out the ipr register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section  = IRQNumber %4 ;
	uint8_t shift_amount = ( 8 * iprx_section) + ( 8 - NO_PR_BITS_IMPLEMENTED) ;
	*(  NVIC_PR_BASE_ADDR + iprx ) |=  ( IRQPriority << shift_amount );
}

