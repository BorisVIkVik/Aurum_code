#include "project_config.h"	
#include "pins_setup.h"
#include "uart.h"
#include <adc.h>
#include <dma.h>
#include <multiplexer.h>
#define CHANNEL1 1
#define CHANNEL2 2
#define CHANNEL3 3
#define CHANNEL4 4

void delay(unsigned int nCount);
//GPIO_InitTypeDef GPIO_InitStruct;
 
int main (void)
{

  Pin tx(GPIO_Pin_10,
				 GPIOB,
				 GPIO_Mode_AF_PP,
				 TIM2,
				 CHANNEL4,
				 RCC_APB1Periph_TIM2,
				 GPIO_PinSource10,
         4096,
				 1);	
	tx.pinInit();
  Pin rx(GPIO_Pin_11,
				 GPIOB,
				 GPIO_Mode_AF_PP,
				 TIM2,
				 CHANNEL4,
				 RCC_APB1Periph_TIM2,
				 GPIO_PinSource10,
         4096,
         1);	
	Pin state1(GPIO_Pin_4,
				 GPIOA,
				 GPIO_Mode_Out_PP,
				 TIM2,
				 CHANNEL4,
				 RCC_APB1Periph_TIM2,
				 GPIO_PinSource10,
         4096,
         1);				 
  Pin state2(GPIO_Pin_5,
				 GPIOA,
				 GPIO_Mode_Out_PP,
				 TIM2,
				 CHANNEL4,
				 RCC_APB1Periph_TIM2,
				 GPIO_PinSource10,
         4096,
         1);
	Pin state3(GPIO_Pin_6,
				 GPIOA,
				 GPIO_Mode_Out_PP,
				 TIM2,
				 CHANNEL4,
				 RCC_APB1Periph_TIM2,
				 GPIO_PinSource10,
         4096,
         1);
  Pin state4(GPIO_Pin_7,
						 GPIOA,
						 GPIO_Mode_Out_PP,
						 TIM2,
						 CHANNEL4,
						 RCC_APB1Periph_TIM2,
						 GPIO_PinSource10,
						 4096,
						 1);
	Pin lightValue(GPIO_Pin_1,
								 GPIOA,
								 GPIO_Mode_AIN,
								 TIM2,
								 CHANNEL4,
								 RCC_APB1Periph_TIM2,
								 GPIO_PinSource10,
								 4096,
								 1);
	lightValue.pinInit();
	Adc mpAdc(ADC1, 2, 0, RCC_APB2Periph_ADC1, lightValue);
	mpAdc.startAdc();
	Dma mpDma(RCC_AHBPeriph_DMA1, mpAdc, DMA1_Channel1, 2);
	mpDma.adcInitInDma();
	rx.pinInit();
	Uart::uartInit();
	Pin state5(GPIO_Pin_8,
				 GPIOA,
				 GPIO_Mode_Out_PP,
				 TIM2,
				 CHANNEL4,
				 RCC_APB1Periph_TIM2,
				 GPIO_PinSource10,
         4096,
         1);				 
  Pin state6(GPIO_Pin_9,
				 GPIOA,
				 GPIO_Mode_Out_PP,
				 TIM2,
				 CHANNEL4,
				 RCC_APB1Periph_TIM2,
				 GPIO_PinSource10,
         4096,
         1);
	Pin state7(GPIO_Pin_10,
				 GPIOA,
				 GPIO_Mode_Out_PP,
				 TIM2,
				 CHANNEL4,
				 RCC_APB1Periph_TIM2,
				 GPIO_PinSource10,
         4096,
         1);
  Pin state8(GPIO_Pin_11,
						 GPIOA,
						 GPIO_Mode_Out_PP,
						 TIM2,
						 CHANNEL4,
						 RCC_APB1Periph_TIM2,
						 GPIO_PinSource10,
						 4096,
						 1);
	Pin lightValue_2(GPIO_Pin_2,
								 GPIOA,
								 GPIO_Mode_AIN,
								 TIM2,
								 CHANNEL4,
								 RCC_APB1Periph_TIM2,
								 GPIO_PinSource10,
								 4096,
								 1);
	lightValue_2.pinInit();
	Multiplexer mp(state1, state2, state3, state4, state5, state6, state7, state8, mpDma, 2, DMA1_Channel1);
  volatile int andrew = 1, andrey = 47;
	volatile int Andrew[16], Andrey[16];
//	delay(5000);
//	volatile int32_t sph1 = mp.getPh1Value(9);
//	volatile int32_t sph2 = mp.getPh1Value(5);
//	volatile int32_t sph3 = mp.getPh1Value(2);
//	volatile int32_t sph4 = mp.getPh1Value(14);
//	volatile int32_t sph5 = mp.getPh1Value(8);
//	volatile int32_t sph6 = mp.getPh2Value(0);
//	volatile int32_t sph7 = mp.getPh2Value(4);
//	volatile int32_t sph8 = mp.getPh2Value(14);
//	volatile int32_t sph9 = mp.getPh2Value(13);
//	volatile int32_t sph10 = mp.getPh2Value(15);
//	delay(5000);
//	sph1 += mp.getPh1Value(9);
//	delay(5000);
//	sph2 += mp.getPh1Value(5);
//	delay(5000);
//	sph3 += mp.getPh1Value(2);
//	delay(5000);
//	sph4 += mp.getPh1Value(14);
//	delay(5000);
//	sph5 += mp.getPh1Value(8);
//	delay(5000);
//	sph6 += mp.getPh2Value(0);
//	delay(5000);
//	sph7 += mp.getPh2Value(4);
//	delay(5000);
//	sph8 += mp.getPh2Value(14);
//	delay(5000);
//	sph9 += mp.getPh2Value(13);
//	delay(5000);
//	sph10 += mp.getPh2Value(15);
	
//	sph1 /= 2;
//	sph2 /= 2;
//	sph3 /= 2;
//	sph4 /= 2;
//	sph5 /= 2;
//	sph6 /= 2;
//	sph7 /= 2;
//	sph8 /= 2;
//	sph9 /= 2;
//	sph10 /= 2;
	
	while (1)
	{
		for(int i = 0; i < 16; i++)
		{

			Andrey[i] = mp.getPh1Value(i);
			Andrew[i] = mp.getPh2Value(i);
		}
		volatile int32_t ph1 = Andrey[9]; 
		volatile int32_t ph2 = Andrey[12];
		volatile int32_t ph4 = Andrey[14];
		volatile int32_t ph5 = Andrey[11];
		volatile int32_t ph6 = Andrey[15];
		volatile int32_t ph7 = Andrew[4]; 
		volatile int32_t ph3 = Andrew[2]; 
		volatile int32_t ph8 = Andrew[14];
		volatile int32_t ph9 = Andrew[13]; 
		volatile int32_t ph10 = Andrew[9]; 
		volatile int32_t ph11 = Andrew[15]; 
		volatile int32_t ph12 = Andrew[0]; 
		volatile int32_t ph13 = Andrew[5]; 
		volatile int32_t ph14 = Andrey[2]; 
		volatile int32_t ph15 = Andrey[10]; 
		volatile int32_t ph16 = Andrey[5]; 
		bool Line = ((ph1 > 3000) ? 1 : 0);	
		Line |= ((ph2 > 3000) ? 1 : 0);
		Line |= ((ph3 > 3000) ? 1 : 0);
		Line |= ((ph4 > 3000) ? 1 : 0);
		Line |= ((ph5 > 3000) ? 1 : 0);
		Line |= ((ph6 > 3000) ? 1 : 0);
		Line |= ((ph7 > 1500) ? 1 : 0);
		Line |= ((ph8 > 3000) ? 1 : 0);
		Line |= ((ph9 > 3000) ? 1 : 0);
		Line |= ((ph10 > 3000) ? 1 : 0);
		Line |= ((ph11 > 3000) ? 1 : 0);
		Line |= ((ph12 > 3000) ? 1 : 0);
		Line |= ((ph13 > 3000) ? 1 : 0);
		Line |= ((ph14 > 3000) ? 1 : 0);
		Line |= ((ph15 > 3000) ? 1 : 0);
		Line |= ((ph16 > 3000) ? 1 : 0);
		if(Line)
		{
			Uart::write(1);
		}
		else
		{
			Uart::write(0);
		}
	}
}
 
// Delay function
void delay(unsigned int nCount)
{
    unsigned int i, j;
 
    for (i = 0; i < nCount; i++)
        for (j = 0; j < 0x2AFF; j++);
}
