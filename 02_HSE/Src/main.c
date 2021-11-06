#include <stm32g431.h>

void GpioInit(void);

int main(void)
{
		
	GpioInit();	//Настройка линий PB7 и PB8 на вывод

	while(1)
	{
		uint32_t i=1000000;				      //Количество итераций в цикле задержки
		while(i)i--; 						  //Задержка
		INVERT_BIT(GPIOB_ODR, GPIO_ODR_OD8);  //Инвертировать LD2 (PB8)
		INVERT_BIT(GPIOB_ODR, GPIO_ODR_OD7);  //Инвертировать (PB7)
	}
}


