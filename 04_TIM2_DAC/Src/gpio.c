#include <stm32g431.h>


void GpioInit(void)
{
	SET_BIT(RCC_AHB2ENR, RCC_AHB2ENR_GPIOBEN);    // Подать тактовый сигнал на модуль GPIOB
	MODIFY_REG(GPIOB_MODER,  GPIO_MODER_MODE8, 1<<GPIO_MODER_MODE8_Pos); //Включить PIN8 на вывод (светодиод LD2)
	MODIFY_REG(GPIOB_MODER,  GPIO_MODER_MODE7, 1<<GPIO_MODER_MODE7_Pos); //Включить PIN7 на вывод
}


