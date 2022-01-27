#include "SPI1.h"


void SPI1_Init (void)
{
//SPI GPIO CONFIG
//	PB0->NSS general output pull-up
//	PB1 general output pull-up //CE => chip enable enable
//	PA5->SCK Master -> Alternate function push-pull
//	PA6->MISO Full duplex / master ->Input floating (/ Input pull-up)
//	PB7->MOSI Full duplex / master ->Alternate function push-pull
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	
	GPIOB->CRL &= ~GPIO_CRL_CNF0;
	GPIOB->CRL |= GPIO_CRL_MODE0;
	GPIOB->ODR |= GPIO_ODR_ODR0 ;
	
	GPIOB->CRL &= ~GPIO_CRL_CNF1;
	GPIOB->CRL |= GPIO_CRL_MODE1;
	GPIOB->ODR &= ~GPIO_ODR_ODR1;
	
	GPIOA->CRL |= GPIO_CRL_CNF5_1;
	GPIOA->CRL &= ~GPIO_CRL_CNF5_0;
	GPIOA->CRL |= GPIO_CRL_MODE5;
	
	GPIOA->CRL &= ~GPIO_CRL_CNF6_1;
	GPIOA->CRL |= GPIO_CRL_CNF6_0;
	GPIOA->CRL &= ~GPIO_CRL_MODE6;
	
	GPIOA->CRL |= GPIO_CRL_CNF7_1;
	GPIOA->CRL &= ~GPIO_CRL_CNF7_0;
	GPIOA->CRL |= GPIO_CRL_MODE7;
	
	SPI1_Deinit();
	
//Configuring the SPI in master mode
//	Procedure
//1. Select the BR[2:0] bits to define the serial clock baud rate (see SPI_CR1 register).
//2. Select the CPOL and CPHA bits to define one of the four relationships between the
//data transfer and the serial clock (see Figure 240).
//3. Set the DFF bit to define 8- or 16-bit data frame format
//4. Configure the LSBFIRST bit in the SPI_CR1 register to define the frame format.
//5. If the NSS pin is required in input mode, in hardware mode, connect the NSS pin to a
//high-level signal during the complete byte transmit sequence. In NSS software mode,
//set the SSM and SSI bits in the SPI_CR1 register. If the NSS pin is required in output
//mode, the SSOE bit only should be set.
//6. The MSTR and SPE bits must be set (they remain set only if the NSS pin is connected
//to a high-level signal).
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	while ((RCC->APB2ENR & RCC_APB2ENR_SPI1EN) != RCC_APB2ENR_SPI1EN);
	
	SPI1->CR1 |= SPI_CR1_MSTR;//Master configuration
	SPI1->CR1 &= ~SPI_CR1_BIDIMODE;//Full duplex (Transmit and receive)
	SPI1->CR1 &= ~SPI_CR1_RXONLY;//
	SPI1->CR1 &= ~SPI_CR1_DFF;//8-bit data frame format is selected for transmission/reception
	SPI1->CR1 |= (SPI_CR1_BR_0|SPI_CR1_BR_1);//Baud rate control  fPCLK/32 = 36/16
	SPI1->CR1 &= ~SPI_CR1_BR_2;
	SPI1->CR1 &= ~SPI_CR1_CPOL;// CK to 0 when idle, CPOL is reset
	SPI1->CR1 &= ~SPI_CR1_CPHA;// The first clock transition is the first data capture edge, CPHA is reset
	//SPI1->CR2 |= SPI_CR2_SSOE; //SS output is enabled in master mode
	SPI1->CR1 |= SPI_CR1_SSM;//Software slave management enabled
	SPI1->CR1 |= SPI_CR1_SSI;
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;//MSB transmitted first
	
	//SPI1->CR1 &= ~SPI_CR1_SPE;
	SPI1->CR1 &= ~SPI_CR1_CRCEN;// CRC calculation disabled
	//SPI1->CRCPR = 0X0A;// =10 the polynomial used for the CRC calculation
	SPI1->CR1 |= SPI_CR1_SPE;//Peripheral enabled
}

void SPI1_Deinit(void){
	
//	while (!(SPI1->SR & SPI_SR_TXE)){};
//	while (SPI1->SR & SPI_SR_BSY){};
	SPI1->CR1 = 0x0000;
	SPI1->CR2 = 0x0000;
	SPI1->DR = 0x0000;
}

//==============================================
void SPI1_CS_ON (void)
{
	GPIOB->ODR &= ~GPIO_ODR_ODR0;  
}


void SPI1_CS_OFF (void)
{
	GPIOB->ODR |= GPIO_ODR_ODR0;
}

// chip enable enable
void SPI1_CE_ON(void)
{
	GPIOB->ODR |= GPIO_ODR_ODR1;// PB1 set
}

// chip enable disable
 void SPI1_CE_OFF(void)
{
	GPIOB->ODR &= ~GPIO_ODR_ODR1;// PB1 reset
}

void SPI1_Transmit (uint8_t *Data, uint8_t size)
{	
	int8_t i=0;
//	/* Disable SPI Peripheral before set 1Line direction (BIDIOE bit) */
//	SPI1->CR1 &= ~SPI_CR1_SPE;//Peripheral disabled
//	SPI1->CR1 |= SPI_CR1_BIDIOE;
//	SPI1->CR1 &= ~SPI_CR1_RXONLY;
	//enable again
	SPI1->CR1 |= SPI_CR1_SPE;//Peripheral enabled	
	/* Check TXE flag */
	for(i=0;i<size;i++) 
	{
		if (i<(size-1))
		{
			while((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE);
			SPI1->DR = Data[i];
	  }else 
		{
			while((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE);
			SPI1->DR = Data[i];
			while((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE) {}
			while (SPI1->SR & SPI_SR_BSY);/* Control the BSY flag */
		}
	}
	/* Clear overrun flag in 2 Lines communication mode because received is not read */
	(void)SPI1->DR;
	(void)SPI1->SR;
	SPI1->CR1 &= ~SPI_CR1_SPE;
}

