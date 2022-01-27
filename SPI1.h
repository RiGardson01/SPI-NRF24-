#include "stm32f10x.h"

void SPI1_Init (void);
void SPI1_Deinit(void);
void SPI1_CE_OFF(void);
void SPI1_CE_ON(void);
void SPI1_CS_OFF (void);
void SPI1_CS_ON (void);
void SPI1_Transmit (uint8_t *Data, uint8_t size);
