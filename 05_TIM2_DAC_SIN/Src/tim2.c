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


#define ds  0.000001      // Период дискретизации в секундах
#define PI  3.1415926	  // Число ПИ
#define PW  0.031415926   // PI*f*ds при f = 10000 Hz


void TIM2_IRQHandler(void)
{

	static float R = 4 * PW * PW; 	//R=(2*PW))^2
	static float V=0;
	static float X = 4096/3.3 * 0.5;//Начальное значение - амплитуда сигнала 0.5 V
	static float S=1500; 			//Смещение DAC

	CLEAR_BIT(TIM2_SR, TIM_SR_UIF);	//Сброс флага прерывания по переполнению таймера

	//Осциллятор
	V -= X*R;
	X += V;

	DAC1_DHR12R1 = X + S;

}

