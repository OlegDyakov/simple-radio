#include <stm32g431.h>


void TIM2_Init(void)
{

  SET_BIT(RCC_APB1ENR1, RCC_APB1ENR1_TIM2EN); // Подаем на TIM2 тактовую частоту
  TIM2_ARR = 85000000 - 1;     				  // Загружаем Auto-reload register  f = 170 000 000/(K+1) (2Hz -> 85000000 - 1)

  SET_BIT(NVIC_ISER0, (1 << 28));		// Разрешить в NVIC прерывание #28 (TIM2)
  SET_BIT(TIM2_DIER, TIM_DIER_UIE);		// Разрешить прерывание по переполнению таймера
  SET_BIT(TIM2_CR1, TIM_CR1_CEN);		// Включить таймер
}


void TIM2_IRQHandler(void)
{
	CLEAR_BIT(TIM2_SR, TIM_SR_UIF);	      //Сброс флага переполнения
	INVERT_BIT(GPIOB_ODR, GPIO_ODR_OD7);  //Инвертировать (PB7)
	INVERT_BIT(GPIOB_ODR, GPIO_ODR_OD8);  //Инвертировать LD2 (PB8)
}
