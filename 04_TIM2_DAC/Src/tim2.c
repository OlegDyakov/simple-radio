#include <stm32g431.h>


void TIM2_Init(void)
{

  SET_BIT(RCC_APB1ENR1, RCC_APB1ENR1_TIM2EN); // Подаем на TIM2 тактовую частоту

  // Загружаем Auto-reload register  f = 170 000 000/(TIM2_ARR + 1) (1 MHz -> 170 - 1)
  TIM2_ARR = 170 - 1;

  SET_BIT(NVIC_ISER0, (1 << 28));		// Разрешить в NVIC прерывание #28 (TIM2)
  SET_BIT(TIM2_DIER, TIM_DIER_UIE);		// Разрешить прерывание по переполнению таймера
  SET_BIT(TIM2_CR1, TIM_CR1_CEN);		// Включить таймер
}

void TIM2_IRQHandler(void)
{
	// Переменная I увеличивается на 1 при каждом вызове обработчика прерывания
	static int	 I=0;

	CLEAR_BIT(TIM2_SR, TIM_SR_UIF);	      //Сброс флага переполнения
	DAC1_DHR12R1 = I++;					  // Запись в регистр данных DAC 12-ти младших разрядов переменной I

}
