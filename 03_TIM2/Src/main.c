#include <stm32g431.h>

void GpioInit(void);
void TIM2_Init(void);

int main(void)
{

	GpioInit(); //Настройка линий PB7 и PB8 на вывод
	TIM2_Init();//Настройка и запуск таймера TIM2

	while(1);
}


