#include <stm32g431.h>

void GpioInit(void);
void DAC1_Init(void);
void TIM2_Init(void);

int main(void)
{

	GpioInit(); //Настройка линий PB7 и PB8 на вывод
	DAC1_Init();//Настройка ЦАП
	TIM2_Init();//Настройка и запуск таймера TIM2

	while(1);
}


