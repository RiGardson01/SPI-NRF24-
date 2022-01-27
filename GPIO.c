#include "GPIO.h"

void GPIO (void){
	//gpioc 13 out put

	
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

	GPIOC->BSRR |= GPIO_BSRR_BS13 ;
	GPIOC->CRH &= ~GPIO_CRH_CNF13;//general purpose output push-pull
	GPIOC->CRH |= GPIO_CRH_MODE13;
	GPIOC->ODR &= ~GPIO_ODR_ODR13;
	
	RCC->APB2ENR  |= RCC_APB2ENR_IOPBEN;

	GPIOB->BSRR |= GPIO_BSRR_BS14 ;
	GPIOB->CRH &= ~GPIO_CRH_CNF14;//general purpose output push-pull
	GPIOB->CRH |= GPIO_CRH_MODE14;
	GPIOB->ODR |= GPIO_ODR_ODR14;
	
	// MODE 10: Alternate function output Push-pull
//	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
//	GPIOA->CRH &= ~GPIO_CRH_CNF8;
//	GPIOA->CRH |= GPIO_CRH_CNF8_1;// Alternate function output Push-pull
//	GPIOA->CRH |= GPIO_CRH_MODE8_1;//output-2Hz
//	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	
}
