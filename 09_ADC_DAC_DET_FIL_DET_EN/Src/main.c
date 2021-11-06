#include <stm32g431.h>

void GpioInit(void);
void TIM2_Init(void);
void DAC1_Init(void);
void ADC1_init();
void Encoder_Init(void);

int main(void)
{

	GpioInit();		//Настройка GPIO
	Encoder_Init(); //Настройка энкодера
	TIM2_Init();	//Настройка таймера TIM2
	DAC1_Init();	//Настройка ЦАП
	ADC1_init();	//Настройка и запуск АЦП

	while(1);
}

