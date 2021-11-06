#include <stm32g431.h>

void GpioInit(void);
void TIM2_Init(void);
void DAC1_Init(void);
void ADC1_init();

int main(void)
{

	GpioInit();	//Настройка GPIO
	TIM2_Init();//Настройка TIM2
	DAC1_Init();//Настройка ЦАП
	ADC1_init();//Настройка и запуск АЦП

	while(1);
}

