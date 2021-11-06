#include <stm32g431.h>

void GpioInit(void);
void OLED_Init(void);
void print(uint8_t x,uint8_t y,char *str);

void TIM2_Init(void);
void DAC1_Init(void);
void ADC1_init();
void Encoder_Init(void);
void TIM4_Init(void);

int main(void)
{

	GpioInit();
	TIM4_Init();
	OLED_Init();

	print(0,0,"Freq :     KHz");
	print(0,2,"Level:   1");

	Encoder_Init();

	TIM2_Init();
	DAC1_Init();
	ADC1_init();


	while(1){}

}

