#include <stm32g431.h>
#include <string.h>
#include <stdlib.h>

void print(uint8_t x,uint8_t y,char *str);
int IsDMA_Busy(void);
void GetAGCLevel(void);

extern float U2; //Уровень АРУ

void TIM4_Init(void)
{

  SET_BIT(RCC_APB1ENR1, RCC_APB1ENR1_TIM4EN); // Подаем на TIM4 тактовую частоту
  TIM4_PSC = 10000 - 1;						  // Загружаем TIMx prescaler        Fclk = 180 000 000 / (PSC[15:0] + 1)
  TIM4_ARR = 18000  - 1;     				  // Загружаем Auto-reload register  f = Fclk/(ARR[15:0]+1) (1Hz -> 17000 - 1)

  SET_BIT(NVIC_ISER0, (1 << 30));	// Разрешить в NVIC прерывание #30 (TIM4)
  NVIC_IPR7 = 0x00300000;           // Установка приоритета  3 для прерывания 30.

  SET_BIT(TIM4_DIER, TIM_DIER_UIE);		// Разрешить прерывание по переполнению таймера
  SET_BIT(TIM4_CR1,  TIM_CR1_CEN);		// Включить таймер
}


void TIM4_IRQHandler(void)
{
	CLEAR_BIT(TIM4_SR, TIM_SR_UIF);	      //Сброс флага переполнения
	if(IsDMA_Busy()) return;
	GetAGCLevel();
	int d = U2;
	d = d/10;
	static char	ds[10] = "     ";
	if( d > 9999 ) return;
	itoa(d, ds + 4 , 10);
	print(48,2,ds + strlen(ds + 4));
}
