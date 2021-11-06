#include <stm32g431.h>

void start_up(void);
void nmi_handler(void);
void hardfault_handler(void);

extern uint32_t _estack;

/* Таблица векторов прерывания */

__attribute__ ((section(".vectors"),used))//Таблицу размещаем в секции .vectors
uint32_t * vectors[]  = {
	(uint32_t *) &_estack,               //Вершина стека
	(uint32_t *) start_up,				 //Обработчик reset (точка входа)
	(uint32_t *) nmi_handler,			 //Обработчик немаскируемого прерывания
	(uint32_t *) hardfault_handler		 //Обработчик прерывания аппаратной ошибки
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
