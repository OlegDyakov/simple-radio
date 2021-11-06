#include <stm32g431.h>


//Выход DAC1 на PA4
void DAC1_Init(void)
{

	SET_BIT(RCC_AHB2ENR, RCC_AHB2ENR_DAC1EN );    // Подать тактовый сигнал на DAC1
	SET_BIT(DAC1_CR, 1); 						  // Включить выход DAC1
	DAC1_DHR12R1 = 0;							  // Записать 0 в регистр данных DAC1

}
