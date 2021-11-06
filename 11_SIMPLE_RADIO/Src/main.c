#include <stm32g431.h>

void GpioInit(void);
void OLED_Init(void);
void print(uint8_t x,uint8_t y,char *str);


void TIM2_Init(void);
void DAC1_Init(void);
void ADC1_init();
void Encoder_Init(void);

int main(void)
{

	GpioInit(); 	//Настройка GPIO

	OLED_Init();	//Настройка OLED

	print(0,1,"Freq :     KHz");

	Encoder_Init();	//Настройка энкодера
	TIM2_Init();	//Настройка таймера TIM2
	DAC1_Init();	//Настройка ЦАП
	ADC1_init();	//Настройка и запуск АЦП

	while(1);

}

