#include <stm32g431.h>

void start_up(void);
void nmi_handler(void);
void hardfault_handler(void);
void ADC1_IRQHandler(void);
void TIM3_IRQHandler(void);
void EXTI9_5_IRQHandler(void);

extern uint32_t _estack;

/* Таблица векторов прерывания */

__attribute__ ((section(".vectors"),used))//Таблицу размещаем в секции .vectors
uint32_t *vectors[]  = {
	(uint32_t *) &_estack,               //Вершина стека
	(uint32_t *) start_up,				 //Обработчик reset (точка входа)
	(uint32_t *) nmi_handler,			 //Обработчик немаскируемого прерывания
	(uint32_t *) hardfault_handler,		 //Обработчик прерывания аппаратной ошибки
	0,0,0,0,0,0,0,0,0,0,0,0,		 	 //Окончание блока системных прерываний
	0,0,0,0,0,0,0,0,0,0,				 //Прерывания 0 - 9
	0,0,0,0,0,0,0,0,					 //Прерывания 10 -17
	(uint32_t *) ADC1_IRQHandler,		 //Прерывание 18
	0,				 					 //Прерывание 19
	0,0,0,								 //Прерывание 20 - 22
	(uint32_t *) EXTI9_5_IRQHandler,	 //23 Обработчик прерывания от EXTI9_5 (Encoder BUT)
	0,0,0,0,0,					 		 //Прерывания 24 -28
	(uint32_t *) TIM3_IRQHandler,		 //29 Обработчик прерывания от TIM3 (Encoder)
	0,0,0,0,0,0,0,0,0,0, 				 //Прерывания 30 -39
};

/* Обработчик немаскируемого прерывания  */
void nmi_handler(void)
{
	for (;;);
}

/* Обработчик прерывания аппаратной ошибки */
void hardfault_handler(void)
{
	for (;;);
}
