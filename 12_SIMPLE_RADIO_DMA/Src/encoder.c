#include <stm32g431.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define FREQ_MAX    750  // Максимальное значение частоты KHz
#define FREQ_MIN    150  // Минимальное значение частоты KHz
#define FREQ_START  150  // Начальное значение частоты KHz

void print(uint8_t x,uint8_t y,char *str);

uint32_t frequency = FREQ_START; //Центральная частота полосы пропускания фильтра

void KHZ_to_R(int fr);

void FreqPrint()
{
	static char	ds[10] = "     ";
	if( frequency > 9999 ) return;
	itoa(frequency, ds + 4 , 10);
	print(48,1,ds + strlen(ds + 4));
}

void Encoder_Init(void)
{

/* Подключение выхода энкодера A (PA6) и B (PA7) к таймеру TIM3 */

	 // Включить  PA6 и PA7 на альтерналивную функцию
	 MODIFY_REG(GPIOA_MODER,  GPIO_MODER_MODE7 | GPIO_MODER_MODE6, 2<<GPIO_MODER_MODE7_Pos | 2<<GPIO_MODER_MODE6_Pos );
     // Включить  у PA6 и PA7 режим Pull UP
	 MODIFY_REG(GPIOA_PUPDR,  GPIO_MODER_MODE7 | GPIO_MODER_MODE6, 1<<GPIO_MODER_MODE7_Pos | 1<<GPIO_MODER_MODE6_Pos);
	 //Выбрать альтернативную функцию 2 (TIM3_CH1 и TIM3_CH2)
	 SET_BIT(GPIOA_AFRL, (2UL << GPIO_AFRL_AFSEL6_Pos) | (2UL << GPIO_AFRL_AFSEL7_Pos)) ;

 /*  Настройка таймера TIM3 на работу с энкодером */

	 SET_BIT(RCC_APB1ENR1, RCC_APB1ENR1_TIM3EN); //Подать тактовую частоту на TIM3
	 // CC1 конфигурируется как вход, IC1 подключается к TI1
	 // CC2 конфигурируется как вход, IC2 подключается к TI2
	 SET_BIT(TIM3_CCMR1, TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0);
	 CLEAR_BIT(TIM3_CCMR1, TIM_CCMR1_CC1S_1 | TIM_CCMR1_CC2S_1);
	 // неинвертируемый/передний фронт
	 CLEAR_BIT(TIM3_CCER, TIM_CCER_CC1P | TIM_CCER_CC2P);
	 CLEAR_BIT(TIM3_CCER, TIM_CCER_CC2NP | TIM_CCER_CC2NP);
	 // Режим энкодера 1 - счетчик  увеличивается/уменьшается по TI2FP1 фронту
	 // в зависимости от уровня TI1FP2
	 SET_BIT(TIM3_SMCR, TIM_SMCR_SMS_0);
	 CLEAR_BIT(TIM3_SMCR, TIM_SMCR_SMS_1);
	 CLEAR_BIT(TIM3_SMCR, TIM_SMCR_SMS_2);
	 // 1111: fSAMPLING = fDTS / 32, N = 8
	 SET_BIT(TIM3_CCMR1, TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_2 | TIM_CCMR1_IC1F_3);
	 SET_BIT(TIM3_CCMR1, TIM_CCMR1_IC2F_0 | TIM_CCMR1_IC2F_1 | TIM_CCMR1_IC2F_2 | TIM_CCMR1_IC2F_3);

	 // Установка начального значения счетчика энкодера
	 // В выбраном режиме работы значение счетчика TIM3_CNT в 2 раза
	 // больше количества шагов энкодера
	 TIM3_CNT = (frequency - FREQ_MIN) *2;
	 KHZ_to_R(frequency);
	 FreqPrint();

	 // Максимальное значение счетчика энкодера
	 TIM3_ARR = 2*(FREQ_MAX - FREQ_MIN) + 1;

	 // Trigger Edge Detector
	 // 100: TI1 Edge Detector (TI1F_ED)
	 CLEAR_BIT(TIM3_SMCR,TIM_SMCR_TS_0 | TIM_SMCR_TS_1);
	 SET_BIT(TIM3_SMCR, TIM_SMCR_TS_2);
	 // Разрешить прерывание по триггеру
	 SET_BIT(TIM3_DIER, TIM_DIER_TIE);
	 // Разрешить в NVIC прерывание #29 (TIM3)
	 SET_BIT(NVIC_ISER0, 1 << 29);
	 NVIC_IPR7 = 0x00002000;  // Установка приоритета 2 для прерывания 29.

 /*  Настройка  кнопки BUT энкодера */

	 // Включить  PA5 на логический вход
	 CLEAR_BIT(GPIOA_MODER,  GPIO_MODER_MODE5);
	 // Включить  у PA5 режим Pull UP
	 MODIFY_REG(GPIOA_PUPDR,  GPIO_MODER_MODE5, 1<<GPIO_MODER_MODE5_Pos);

	 //Конфигурация прерывания от PA5 (EXTI5)
	 SET_BIT(EXTI_IMR1,   1 << 5);    // Interrupt request EXTI5 not mask
	 SET_BIT(EXTI_FTSR1,  1 << 5);    // Falling trigger enabled  EXTI5
	 SET_BIT(NVIC_ISER0,  1 << 23);   // Разрешить в NVIC прерывание #23 (EXTI9_5)
	 // Установка приоритета  1 для прерывания 23.
	 NVIC_IPR6 = 0x10000000;

	 SET_BIT(TIM3_CR1,TIM_CR1_CEN);   // Разрешить работу счетчика TIM3
}

//Функция-обработчик прерывания по изменению счетчика TIM3
void TIM3_IRQHandler(void){

    uint32_t c;
    static uint32_t f=0;
	if(TIM3_SR & TIM_SR_TIF){
		CLEAR_BIT(TIM3_SR,TIM_SR_TIF); //Сброс запроса на прерывания от TIM3
		c = TIM3_CNT;
		if((c & 1) == 0) {
			frequency = c/2 + FREQ_MIN;
			if(f != frequency){		  //Защита от дублирования значений частоты
				f = frequency;
				KHZ_to_R(frequency); //Расчет значения R для фильтра
				FreqPrint();
			}

		}
	}
}


//Функция-обработчик прерывания по нажатию кнопки энкодера
void EXTI9_5_IRQHandler(void)
{
	EXTI_PR1 = EXTI_PR1_PIF5 ; //Сброс запроса на прерывания от EXTI5
	frequency = (frequency/100 +1)*100;
	if(frequency > FREQ_MAX) frequency = FREQ_MIN;
	TIM3_CNT = (frequency - FREQ_MIN) *2;
	KHZ_to_R(frequency); 	  //Расчет значения R для фильтра
	FreqPrint();
}
