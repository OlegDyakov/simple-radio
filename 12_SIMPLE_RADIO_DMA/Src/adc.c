#include <stm32g431.h>
#include <math.h>

#define WAVEFORM_SAMPLES_SIZE 64 //Длина буфера ADC (не забудь поменять в dsp.s)
uint16_t Waveform_ADC[WAVEFORM_SAMPLES_SIZE*2]; //Два буфера

#define FD   1600   //Частота дискретизации KHz
#define FLPF 2      //Частота перегиба ФНЧ KHz
#define FAGC 0.001  //Частота перегиба АРУ KHz
float 	R;	  	    //Параметр фильтра, зависящий от frequency


//Таймер, задающий частоту дискретизации ADC1
void TIM2_Init(void)
{

  SET_BIT(RCC_APB1ENR1, RCC_APB1ENR1_TIM2EN); // Подаем на TIM2 тактовую частоту
  TIM2_ARR = 170000/FD - 1;         		  // Загружаем Auto-reload register  f = 170 000 000/(TIM2_ARR+1), 1 MHz -> 170 -1
  TIM2_CR2 = TIM_CR2_MMS_1;   				  // Выбор мастер режима таймера для возможности тактирования ADC

}

//Вход ADC1 на PA0
void ADC1_init()
{

    //Настройка тактирования ADC
	SET_BIT(RCC_CCIPR,   1 << 29 );       // Выбрать системный clock в качестве тактового сигнала для ADC
    SET_BIT(RCC_AHB2ENR, 1 << 13 );       // Подать тактовый сигнал на модуль ADC12
    SET_BIT(ADC12_COMMON_CCR, 2 << 16 );  // Выбрать в качестве тактового сигнала   adc_hclk/2

    //Настройка режима ADC
    ADC1_CR = 0;             			  // Сброс всех бит в ADC1_CR (ADEN = 0 для обеспечения конфигурирования)
    SET_BIT(ADC1_CR, 1 << 28);            // Включить  опорное напряжение ADC

    // Start calibration sequence
    ADC1_CR |= (1 << 31);
    while(ADC1_CR & (1 << 31)); // wait for calibration to finish

    // Запуск преобразования от  TIM2_TRG0 (канал 11)(биты 5- 11), перезапись разрешена (бит 12)
    ADC1_CFGR = (1 << 12)+(1 << 10)+ (11 << 5);
	ADC1_SQR1 = (1 << 6);  				  // Выбрать входной канал 1 (PA0)

	/* ADC1 DMA Init */

    SET_BIT(RCC_AHB1ENR, RCC_AHB1ENR_DMAMUX1EN); //Подача тактового сигнала на DMAMUX1
    SET_BIT(RCC_AHB1ENR, RCC_AHB1ENR_DMA1EN);	 //Подача тактового сигнала на DMA1

	MODIFY_REG(DMAMUX1_Channel3_CCR, DMAMUX_CxCR_DMAREQ_ID, 5); //DMAMUX request from line 5 (ADC1)
	MODIFY_REG(DMA1_Channel4_CCR, DMA_CCR_DIR | DMA_CCR_MEM2MEM, 0); ////DMA_DIRECTION_PERIPH_TO_MEMORY
	MODIFY_REG(DMA1_Channel4_CCR, DMA_CCR_PL, DMA_CCR_PL_1); //DMA_SetChannelPriorityLevel
	MODIFY_REG(DMA1_Channel4_CCR, DMA_CCR_CIRC, DMA_CCR_CIRC); //DMA_SetMode  CIRC
	MODIFY_REG(DMA1_Channel4_CCR, DMA_CCR_PINC, 0); //DMA_SetPeriphIncMode Peripheral increment mode Disable
	MODIFY_REG(DMA1_Channel4_CCR, DMA_CCR_MINC, DMA_CCR_MINC); //DMA_SetMemoryIncMode
	MODIFY_REG(DMA1_Channel4_CCR, DMA_CCR_PSIZE, DMA_CCR_PSIZE_1); //ACD_SetMemorySize - 32 bit
	MODIFY_REG(DMA1_Channel4_CCR, DMA_CCR_MSIZE, DMA_CCR_MSIZE_0); //DMA_SetMemorySize - 16 bit


	SET_BIT(NVIC_ISER0, (1 << 14));		// Разрешить в NVIC прерывание #14 (DMA1_CH4)
	NVIC_IPR3 = 0x00000000;				//Приоритет 0

	/* Configure the ACD functional parameters transmission */
	WRITE_REG(DMA1_Channel4_CMAR, (uint32_t)Waveform_ADC); //Destination
	WRITE_REG(DMA1_Channel4_CPAR, (uint32_t)&(ADC1_DR)); //Source
	MODIFY_REG(DMA1_Channel4_CNDTR, DMA_CNDTR_NDT, WAVEFORM_SAMPLES_SIZE*2); //DMA_SetDataLength

	SET_BIT(DMA1_Channel4_CCR, DMA_CCR_TEIE | DMA_CCR_TCIE | DMA_CCR_HTIE ); //Enable DMA transfer interruption: transfer error, transfer complete, half transfer complete

	SET_BIT(DMA1_Channel4_CCR, DMA_CCR_EN); //Enable the DMA transfer

	SET_BIT(ADC1_CFGR, ADC_CFGR_DMAEN);  //Enable ADC channel DMA request
	SET_BIT(ADC1_CFGR, ADC_CFGR_DMACFG); //DMA Circular mode selected

	SET_BIT(ADC1_CR, (1 << 0) | (1 << 2));// Включить ADC1 и начать преобразование
	SET_BIT(TIM2_CR1, TIM_CR1_CEN);       //Запуск TIM2 - cтарт генерации сигнала дискретизации для ADC1

}

// Расчет параметра фильтра зависящего от частоты R=(2*Sin(PI*W))^2
void KHZ_to_R(int fr)
{
	R = (2*sin(M_PI*(0.998*fr)/FD));
	R *= R;
}


// Обработчик прерывания от DMA.
// Основной блок обработки массива отсчетов от ADC1.

void DMA1_CH4_IRQHandler()
{

	//Параметр, определяющий полосу пропускания фильтра dF
	//L  ≈  5.6 * Fd/FD
	static float L = 0.005;

	//Начальные значения переменных фильтра
	static float X = 0;
	static float V = 0;

	//Параметр интегрирующей цепочки
	static float K1 = WAVEFORM_SAMPLES_SIZE*(2*M_PI*FLPF/FD); //ФНЧ K = 2PI*f/fd
	static float K2 = WAVEFORM_SAMPLES_SIZE*(2*M_PI*FAGC/FD); //АРУ

	//Начальные значения переменных детектора
	static float U1 = 0;
	static float U2 = 0;
	static float AGCT  = 1000;

	float M  = 0.02; //Коэффициент компенсации передачи фильтра

	//Смещение для ADC и DAC
	static int	 S_ADC = 1975; // Uдел/3.3 * 4096  -> 1.586/3.29 *4096 = 1910
	static int	 S_DAC = 800;

	float D =0;
	int   OUT,i;
	int16_t *Pointer;

	//Выбор буфера: первая половина Waveform_ADC/вторая половина Waveform_ADC
	if((DMA1_ISR &  DMA_ISR_HTIF4)){
		Pointer = Waveform_ADC;
	} else {
		Pointer = Waveform_ADC + WAVEFORM_SAMPLES_SIZE;
	}
	WRITE_REG(DMA1_IFCR, DMA_IFCR_CTCIF4 | DMA_IFCR_CHTIF4);

	//Обработка буфера
	for(i=0;i<WAVEFORM_SAMPLES_SIZE;i++){

		//Фильтр
		X += *Pointer- S_ADC;
		V -= X * R;
		X += V - X * L;
		Pointer++;

		//Детектор
		D += fabs(X);
	}

	//Обработка НЧ

	D *= M; //Нормализация

	U1  += (D - U1) * K1; // Интегрирующая цепь ФНЧ
	OUT = U1;

	//АРУ
	U2  += (D - U2) * K2; // Интегрирующая цепь АРУ
	if( U2 > AGCT)
		OUT = U1*(AGCT/U2);

	OUT +=  S_DAC;

	// Ограничитель, предотвращает переполнение DAC при сильном сигнале
	if( OUT > 4000) OUT = 4000;

	//Запись в DAC
	DAC1_DHR12R1 = OUT;

}
