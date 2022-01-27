#include "NRF_SPI1.h"
#include "SPI1.h"
#include "TIM2.h"

#define  TX_ADDR_WIDTH 5
#define  TX_PLOAD_WIDTH 4


uint8_t SPI1_TX_ADDRESS[TX_ADDR_WIDTH] = {0x00,0xDD,0xCC};
uint8_t SPI1_ACK[1] = {123};

uint8_t SPI1_NRF24L01P_Read_Register(uint8_t RegAdd){
	uint8_t READ_REG = 0;
	SPI1->CR1 |= SPI_CR1_SPE;
 	SPI1_CS_ON();

	while(!(READ_BIT(SPI1->SR, SPI_SR_TXE) == (SPI_SR_TXE))) {}
	SPI1->DR = RegAdd;
	if(RegAdd == STATUS){
		while(!(READ_BIT(SPI1->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))) {}
		READ_REG = SPI1->DR;
	}
	else{
		while(!(READ_BIT(SPI1->SR, SPI_SR_TXE) == (SPI_SR_TXE))) {}
		while (SPI1->SR & SPI_SR_BSY);
		SPI1->CR1 |= SPI_CR1_RXONLY;

		while(!(READ_BIT(SPI1->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))) {}
		(void) SPI1->DR;

		while(!(READ_BIT(SPI1->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))) {}
		READ_REG = SPI1->DR;
		SPI1_CS_OFF();
		SPI1->CR1 &= ~SPI_CR1_RXONLY;
	}
	DelayMicro(1);
	return READ_REG;
}

void SPI1_NRF24L01P_MultiRead_Register(uint8_t RegAdd, uint8_t* Data, uint8_t size){
	uint8_t i = 0;
	SPI1->CR1 |= SPI_CR1_SPE;
	SPI1_CS_ON();
	while(!(READ_BIT(SPI1->SR, SPI_SR_TXE) == (SPI_SR_TXE))) {}
	SPI1->DR = RegAdd;

	while(!(READ_BIT(SPI1->SR, SPI_SR_TXE) == (SPI_SR_TXE))) {}
	while (SPI1->SR & SPI_SR_BSY);
	SPI1->CR1 |= SPI_CR1_RXONLY;

	while(!(READ_BIT(SPI1->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))) {}
	(void) SPI1->DR;

	for(i=0; i<size; i++){

		while(!(READ_BIT(SPI1->SR, SPI_SR_RXNE) == (SPI_SR_RXNE))) {}
		Data[i] = SPI1->DR;
	}

	SPI1_CS_OFF();
	SPI1->CR1 &= ~SPI_CR1_RXONLY;
}


void SPI1_NRF24_WriteReg(uint8_t addr, uint8_t dt)
{
	uint8_t TxData[1];
	addr |= W_REGISTER;
  SPI1_CS_ON();
	TxData[0]=addr;
	SPI1_Transmit(TxData,1);
	TxData[0]=dt;
	SPI1_Transmit(TxData,1);
  SPI1_CS_OFF();
}

void SPI1_NRF24_WriteReg_Mutil(uint8_t addr, uint8_t *dt, uint8_t size)
{
	uint8_t TxData[1];
	addr |= W_REGISTER;
  SPI1_CS_ON();
	TxData[0]=addr;
	SPI1_Transmit(TxData,1);
	SPI1_Transmit(dt,size);
  SPI1_CS_OFF();
}

void SPI1_NRF24_reset(uint8_t REG )
{
	uint8_t rx_addr_p0_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	uint8_t rx_addr_p1_def[5] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
	uint8_t tx_addr_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	SPI1_CE_OFF();
  DelayMicro(5000); 
	
	if (REG == STATUS) 
	{
		SPI1_NRF24_WriteReg(STATUS, 0x00);
	}
	
	else 	if (REG == FIFO_STATUS)
	{
		SPI1_NRF24_WriteReg(FIFO_STATUS,0x11);
	}				
	
	else 
	{
		SPI1_NRF24_WriteReg(CONFIG,0x08);
		SPI1_NRF24_WriteReg(EN_AA, 0x3F);
		SPI1_NRF24_WriteReg(EN_RXADDR, 0x03);
		SPI1_NRF24_WriteReg(SETUP_AW, 0x03);
		SPI1_NRF24_WriteReg(SETUP_RETR, 0x03);
		SPI1_NRF24_WriteReg(RF_CH, 0x02);
		SPI1_NRF24_WriteReg(RF_SETUP, 0x0E);
		SPI1_NRF24_WriteReg(STATUS, 0x00);
		SPI1_NRF24_WriteReg(OBSERVE_TX, 0x00);
		SPI1_NRF24_WriteReg_Mutil(RX_ADDR_P0,rx_addr_p0_def,5);
		SPI1_NRF24_WriteReg_Mutil(RX_ADDR_P1,rx_addr_p1_def,5);
		SPI1_NRF24_WriteReg(RX_ADDR_P2, 0xC3);
		SPI1_NRF24_WriteReg(RX_ADDR_P3, 0xC4);
		SPI1_NRF24_WriteReg(RX_ADDR_P4, 0xC5);
		SPI1_NRF24_WriteReg(RX_ADDR_P5, 0xC6);	
		SPI1_NRF24_WriteReg_Mutil(TX_ADDR,tx_addr_def,5);
		SPI1_NRF24_WriteReg(RX_PW_P0, 0);
		SPI1_NRF24_WriteReg(RX_PW_P1, 0);
		SPI1_NRF24_WriteReg(RX_PW_P2, 0);
		SPI1_NRF24_WriteReg(RX_PW_P3, 0);
		SPI1_NRF24_WriteReg(RX_PW_P4, 0);
		SPI1_NRF24_WriteReg(RX_PW_P5, 0);
		SPI1_NRF24_WriteReg(FIFO_STATUS, 0x11);
		SPI1_NRF24_WriteReg(DYNPD, 0);
		SPI1_NRF24_WriteReg(FEATURE, 0);			
	}
}

 void SPI1_NRF24_Rx_ini(void)
{
	uint8_t config = 0;
	SPI1_CE_OFF();
	//select mode PRX
	config = SPI1_NRF24L01P_Read_Register(CONFIG);
	config |= 0x01;
	SPI1_NRF24_WriteReg (CONFIG, config);
	//reset NRF
	SPI1_NRF24_reset(0);
	SPI1_NRF24_reset(STATUS);
	
	SPI1_NRF24_WriteReg(EN_RXADDR, 0x02);//enable data pipe 1
	SPI1_NRF24_WriteReg(EN_AA, 0x01);//enable auto-ack pipe 1
	SPI1_NRF24_WriteReg(DYNPD, 0x01);//ennable dynamic payload length data pipe 0
	
	SPI1_NRF24_WriteReg(RX_PW_P1, TX_PLOAD_WIDTH);//  set numbers bit payload size for pipe 1 = TX_PLOAD_WIDTH
	SPI1_NRF24_WriteReg_Mutil(RX_ADDR_P1, SPI1_TX_ADDRESS,TX_ADDR_WIDTH);// Write the Pipe1 ADDR
		
	SPI1_NRF24_WriteReg(SETUP_AW, 0x01);// 3 Bytes for the TX/RX address
	SPI1_NRF24_WriteReg(SETUP_RETR, 0x05);//retransmit 5 time + wait 250 us
	SPI1_NRF24_WriteReg(RF_CH, 76);// period 2476 MHz	
	SPI1_NRF24_WriteReg(RF_SETUP, 0x02 );//TX_PWR:0dBm, Datarate:1Mbps
	SPI1_NRF24_WriteReg(FEATURE, 0x06);//enable dynamic payload and Ack payload
	SPI1_NRF24_WriteReg(DYNPD, 0x3F);//ennable dynamic payload length data pipe 0
	//NRF24_WriteReg(ACTIVATE,0x73);
	//power up
	config = SPI1_NRF24L01P_Read_Register(CONFIG);
	config |= 0x02;
	SPI1_NRF24_WriteReg (CONFIG, config);
	SPI1->DR = 0x00;
	DelayMicro(5);
}
