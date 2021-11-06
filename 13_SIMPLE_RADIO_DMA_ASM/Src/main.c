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

	GpioInit();
	OLED_Init();

	print(0,1,"Freq:      KHz");

	Encoder_Init();
	TIM2_Init();
	DAC1_Init();
	ADC1_init();


	while(1){}

}

