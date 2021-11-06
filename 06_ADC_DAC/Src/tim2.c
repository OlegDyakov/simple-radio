#include <stm32g431.h>

void TIM2_Init(void)
{

  SET_BIT(RCC_APB1ENR1, RCC_APB1ENR1_TIM2EN); // Подаем на TIM2 тактовую частоту
  TIM2_ARR = 170-1;         				  // Загружаем Auto-reload register  f = 170 000 000/(TIM2_ARR+1), 1 MHz -> (170 - 1)
  TIM2_CR2 = TIM_CR2_MMS_1;   				  // Выбор мастер режима таймера для возможности тактирования ADC

}

