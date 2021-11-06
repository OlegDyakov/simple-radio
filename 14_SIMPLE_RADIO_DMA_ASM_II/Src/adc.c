#include <stm32g431.h>


#define PI 3.1415926

#define WAVEFORM_SAMPLES_SIZE 64 //Длина буфера ADC (не забудь поменять в dsp.s)
uint16_t Waveform_ADC[WAVEFORM_SAMPLES_SIZE*2]; //Два буфера

void dsp_init();

/*************** Блок глобальных переменных обработки цифрового сигнала ***********************/

#define FD   3500  //Частота дискретизации KHz
#define FLPF 2      //Частота перегиба ФНЧ KHz
#define FAGC 0.001  //Частота перегиба АРУ KHz

//Частота дискретизации
float fd = FD;

//Начальные значения переменных фильтров

float L = 0.01; 	//L  ≈  5.6 * dF/FD
float M  = 0.0005;  //Коэффициент компенсации передачи фильтра

//Параметр интегрирующей цепочки детектора
float K1 = WAVEFORM_SAMPLES_SIZE*(2*PI*FLPF/FD); //K = 2PI*f/fd (Увеличивается в  WAVEFORM_SAMPLES_SIZE раз)

//Параметр интегрирующей цепочки АРУ
float U2 = 0;
float K2 = WAVEFORM_SAMPLES_SIZE*(2*PI*FAGC/FD); //K = 2PI*f/fd
float AGCT  = 1000; 							 //Порог АРУ

//Смещение для ADC и DAC
float S_ADC = 1910; // Uдел/3.3 * 4096  -> 1.54/3.3 *4096 = 1910
float S_DAC = 800;

//Массив коэффициентов линейной аппроксимации частоты фильтра 0 - 1600 KHz, шаг 100 KHz
float A[17] = {
		0.990,0.990,0.970,0.940,0.920,
		0.880,0.830,0.780,0.720,0.660,
		0.590,0.520,0.440,0.360,0.270,
		0.190,0.150
};

float B[17] = {
		0.00,0.00,4.00,13.00,21.00,
		41.00,71.00,106.00,154.00,208.00,
		278.00,355.00,451.00,555.00,681.00,
		801.00,865.00
};

//Массив коэффициентов линейной аппроксимации коэф. передачи фильтра 0 - 1600 KHz, шаг 100 KHz
float AA[17] = {
		0.000,0.000,0.000,0.001,0.001,
		0.001,0.002,0.002,0.003,0.005,
		0.007,0.010,0.017,0.024,0.105,
		0.282,0.504
};

float BA[17] = {
		1.00,0.95,0.98,0.91,0.68,
		0.80,0.17,-0.17,-0.89,-2.20,
		-4.48,-8.15,-16.24,-24.70,-138.86,
		-404.23,-759.35
};

//Таймер, задающий частоту дискретизации ADC1
void TIM2_Init(void)
{

  SET_BIT(RCC_APB1ENR1, RCC_APB1ENR1_TIM2EN); // Подаем на TIM2 тактовую частоту
  TIM2_ARR = 180000/FD - 1;         		  // Загружаем Auto-reload register  f = 170 000 000/(TIM2_ARR+1), 1 MHz -> 170 -1
  TIM2_CR2 = TIM_CR2_MMS_1;   				  // Выбор мастер режима таймера для возможности тактирования ADC

}


//Вход ADC1 на PA0
void ADC1_init()
{

    //Настройка тактирования ADC
	SET_BIT(RCC_CCIPR,   1 << 28 );       // Выбрать системный выход PLL_P  в качестве тактового сигнала для ADC
    SET_BIT(RCC_AHB2ENR, 1 << 13 );       // Подать тактовый сигнал на модуль ADC12
    SET_BIT(ADC12_COMMON_CCR, 2 << 16 );  // Выбрать в качестве тактового сигнала   adc_hclk/2 (60 MHz)

    //Настройка режима ADC
    ADC1_CR = 0;             			  // Сброс всех бит в ADC1_CR (ADEN = 0 для обеспечения конфигурирования)
    SET_BIT(ADC1_CR, 1 << 28);            // Включить  опорное напряжение ADC

    // Start calibration sequence
    ADC1_CR |= (1 << 31);
    while(ADC1_CR & (1 << 31)); 		  // wait for calibration to finish

    // Запуск преобразования от  TIM2_TRG0 (канал 11)(биты 5- 11), перезапись разрешена (бит 12)
    ADC1_CFGR = (1 << 12) + (1 << 10) + (11 << 5); //12 бит

	ADC1_SQR1 = (1 << 6);  				  // Выбрать входной канал 1 (PA0)

	/* ADC1 DMA Init */

    SET_BIT(RCC_AHB1ENR, RCC_AHB1ENR_DMAMUX1EN); //Подача тактового сигнала на DMAMUX1
    SET_BIT(RCC_AHB1ENR, RCC_AHB1ENR_DMA1EN);	 //Подача тактового сигнала на DMA1

	MODIFY_REG(DMAMUX1_Channel3_CCR, DMAMUX_CxCR_DMAREQ_ID, 5); 		//DMAMUX ADC1  request
	MODIFY_REG(DMA1_Channel4_CCR, DMA_CCR_DIR | DMA_CCR_MEM2MEM, 0); 	//DMA_DIRECTION_PERIPH_TO_MEMORY
	MODIFY_REG(DMA1_Channel4_CCR, DMA_CCR_PL, DMA_CCR_PL_1); 			//DMA_SetChannelPriorityLevel
	MODIFY_REG(DMA1_Channel4_CCR, DMA_CCR_CIRC, DMA_CCR_CIRC); 			//DMA_SetMode  CIRC
	MODIFY_REG(DMA1_Channel4_CCR, DMA_CCR_PINC, 0); 					//DMA_SetPeriphIncMode Peripheral increment mode Disable
	MODIFY_REG(DMA1_Channel4_CCR, DMA_CCR_MINC, DMA_CCR_MINC); 			//DMA_SetMemoryIncMode
	MODIFY_REG(DMA1_Channel4_CCR, DMA_CCR_PSIZE, DMA_CCR_PSIZE_1); 		//ACD_SetMemorySize - 32 bit
	MODIFY_REG(DMA1_Channel4_CCR, DMA_CCR_MSIZE, DMA_CCR_MSIZE_0); 		//DMA_SetMemorySize - 16 bit

	//Приоритеты DMA1_CH4 = 0 (ADC1, наивысший), DMA1_CH3 = 1 (print),
	//TIM3 = 2 (encoder), TIM4 = 3 (level)
	SET_BIT(NVIC_ISER0, (1 << 14));		// Разрешить в NVIC прерывание #14 (DMA1_CH4)
	NVIC_IPR3 = 0x00000000; 			// Приоритет 0 для DMA1_CH4


	/* Configure the ACD functional parameters transmission */
	WRITE_REG(DMA1_Channel4_CMAR, (uint32_t)Waveform_ADC); //Destination
	WRITE_REG(DMA1_Channel4_CPAR, (uint32_t)&(ADC1_DR));   //Source
	MODIFY_REG(DMA1_Channel4_CNDTR, DMA_CNDTR_NDT, WAVEFORM_SAMPLES_SIZE*2); //DMA_SetDataLength

	SET_BIT(DMA1_Channel4_CCR, DMA_CCR_TEIE | DMA_CCR_TCIE | DMA_CCR_HTIE ); //Enable DMA transfer interruption: transfer error, transfer complete, half transfer complete

	SET_BIT(DMA1_Channel4_CCR, DMA_CCR_EN); //Enable the DMA transfer

	SET_BIT(ADC1_CFGR, ADC_CFGR_DMAEN);     //Enable ADC channel DMA request
	SET_BIT(ADC1_CFGR, ADC_CFGR_DMACFG);    //DMA Circular mode selected

	SET_BIT(ADC1_CR, (1 << 0) | (1 << 2));  // Включить ADC1 и начать преобразование
	SET_BIT(TIM2_CR1, TIM_CR1_CEN);         //Запуск TIM2 - cтарт генерации сигнала дискретизации для ADC1

	dsp_init();		//Инициализация регистров FPU
}


