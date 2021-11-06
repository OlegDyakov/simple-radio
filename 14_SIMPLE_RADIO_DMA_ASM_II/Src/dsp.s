.syntax unified		@ синтаксис исходного кода
.thumb				@ тип используемых инструкций Thumb
.cpu cortex-m4		@ процессор
.fpu fpv4-sp-d16	@ сопроцессор

.globl   dsp_init
.globl   KHZ_to_R
.globl   DMA1_CH4_IRQHandler
.global  CopyRA_toReg
.global  GetAGCLevel

//Общие константы
.equ  ADC1_ISR          ,0x50000000
.equ  ADC1_DR           ,0x50000040
.equ  DAC1_DHR12R1      ,0x50000808
.equ  GPIOB_ODR       	,0x48000414
.equ  DMA1_ISR			,0x40020000
.equ  DMA1_IFCR	    	,0x40020004
.equ  DMA_ISR_HTIF4     ,0x00004000
.equ  DMA_IFCR_CTCIF4   ,0x00002000
.equ  DMA_IFCR_CHTIF4   ,0x00004000
.equ  WAVEFORM_SAMPLES_SIZE 	,64

.section .text.

//Блок констант для быстрой загрузки в r1-r6
const:
		.word Waveform_ADC
		.word WAVEFORM_SAMPLES_SIZE
		.word DMA1_ISR
		.word 0x6000
		.word DMA1_IFCR
		.word DAC1_DHR12R1
PI2:
		.float 6.2831852 //2*PI

//Размещение переменных в регистрах
//s16 = X1, s17 = V1, s18 = R, s19 = L
//s20 = X2, s21 = V2, s24 = M
//s25 = U1, s26 = K1
//s27 = U2, s28 = K2
//s29 = S_ADC, s30 = S_DAC, s31 = AGCT


//Инициализация регистров FPU
.type    dsp_init, %function
dsp_init:
 	push    {r0,lr}
	vldr 	s0,  =0		//s0 = 0
	vmov	s16, s0		//X1 = 0
	vmov 	s17, s0		//V1 = 0
	vmov	s20, s0		//X2 = 0
	vmov 	s21, s0		//V2 = 0
	vmov 	s25, s0		//U1 = 0
	vmov 	s27, s0		//U2 = 0

	ldr	    r0, =L
	vldr 	s19,[r0]    //s19 = L
	ldr		r0, =K1
	vldr 	s26,[r0]    //s26 = K1
	ldr 	r0, =K2
	vldr 	s28,[r0]    //s28 = K2
	ldr		r0, =S_ADC
	vldr 	s29,[r0]    //s29 = S_ADC
	ldr		r0, =S_DAC
	vldr 	s30,[r0]    //s30 = S_DAC
	ldr		r0, =AGCT
	vldr 	s31,[r0]    //s31 = AGCT
	ldr		r0, =M
	vldr 	s24,[r0]    //s24 = M

 	pop 	{r0,pc}


//Расчет параметров фильтра
//Frc=A[Fr/100]+B[Fr/100]
//R = (2*PI*Frc/Fd)^2

.type    KHZ_to_R, %function
KHZ_to_R:
	push    {r0,r1,lr}

	ldr 	r0,	=frequency
	ldr 	r1, [r0]     //r1 = Fr
	vldr   	s0, [r0]
	vcvt.f32.s32 s0, s0 //(float) s0 = (int), s0 = Fr

	mov		r0,	#100
	udiv	r1,r0
	lsl 	r1,2       //r1 = index в массиве A[] и B[]

	ldr		r0,	=fd
	vldr   	s1, [r0]   //s1 = Fd

	ldr		r0, =A
	add		r0, r1
	vldr   	s2, [r0]   //s2 = A[F]

	ldr		r0,=B
	add		r0,r1
	vldr   	s3,[r0] 	//s3 = B[F]

	vfma.f32 s3,s0,s2	  //s3 = s3 + s0*s2  -> Frc = B + A*Fr

    vdiv.f32    s4, s3,s1	// W = Frc/Fd для использования скоректированной частоты
//  vdiv.f32    s4, s0,s1	// W = Fr/Fd для калибровки шкалы

// Расчет R = (2*PI*Frc/Fd)^2
    ldr 	r0,	=PI2
    vldr   	s5, [r0]     //2*PI
    vmul.f32 	s5,s5,s4 //2*PI*W
    vmul.f32 	s18,s5,s5 //s18 = (2*PI*W)^2

//Коррекция M

    ldr 	r0, =AA
	add		r0, r1
	vldr   	s1, [r0]   //AA[i]

	ldr 	r0, =BA
	add		r0, r1
	vldr   	s2, [r0]   //BA[i]
    vfma.f32 s2,s1,s0  //s2 = s2 + s1*s0  -> Mc = BA + AA[i]*M

	ldr 	r0, = M
	vldr 	s24,[r0]        //s24 = M

	vdiv.f32    s24, s24,s2	// M = M/K

 	pop 	{r0,r1,pc}


//Получение уровня сигнала
//Запись значения S27 в переменную U2
.type   GetAGCLevel, %function
GetAGCLevel:

	push    {r0,lr}
	ldr		r0, =U2
	vstr 	s27,[r0]    //U2 = s27
	pop 	{r0,pc}


// Основной блок обработки массива отсчетов от ADC1
// r1 = pointer на буфер Waveform_ADC
// r2 = количество элементов во входном массиве
// r3 = адрес регистра DMA1_ISR
// r4 = константа для анализа заполнения буфера
// r5 = адрес регистра DMA1_IFCR
// r6 = адрес регистра DAC1_DHR12R1
// s16 = X1, s17 = V1, s18 = R, s19 = L
// s20 = X2, s21 = V2, s24 = M
// s25 = U1, s26 = K1
// s27 = U2, s28 = K2
// s29 = S_ADC, s30 = S_DAC, s31 = AGCT

.type    DMA1_CH4_IRQHandler, %function
DMA1_CH4_IRQHandler:

	push 	{r0-r6,lr}

//  Раскомментировать для измерения скорости обработки
/*	mov		r0,	0x80	//On   PB7
	ldr     r1,	=GPIOB_ODR
	str		r0,	[r1] */

   ldr		r0, =const
   ldmia    r0, {r1,r2,r3,r4,r5,r6} //Загрузка констант из массива const в регистры r1-r6

// Анализ какая часть буфера Waveform_ADC заполнена
	ldr	   r0,[r3] 			//[DMA1_ISR]
	ands   r0,DMA_ISR_HTIF4
	beq    1f
	add	   r1,WAVEFORM_SAMPLES_SIZE * 2	//Заполнена вторая часть буфера.
										//pointer += WAVEFORM_SAMPLES_SIZE * 2
1:
	str	   r4,[r5] // [DMA1_IFCR] Сброс прерывания

2:	/************ Цикл обработки буфера Waveform_ADC *******************/

    /************ Чтение одного отсчета *************/
 	ldrh   	r0, [r1]	//r0 = [pointer]
	vmov   	s0, r0  	//s0 = r0
  	vcvt.f32.s32 s0, s0 //(float) s0 = (int) s0
    vsub.f32 s0, s29	 //s0 = s0 - S_ADC

  	/************ Фильтр 1 *******************/

	vadd.f32 s16, s0	 //X1 += s0

	//V -= X * R;
   	vfms.f32 s17, s16, s18  //s17 = s17 - (s16 x s18) -> V1 -= X1 * R;

	//X1 += V1 - X1 * L;
	vadd.f32 s16,s17      //X1 = X1 + V1
	vfms.f32 s16,s16,s19  //s16 = s16 - (s16 x s19) -> X1 -= X1 * L;

  	/************ Фильтр 2 *******************/

	vadd.f32 s20, s16	 //X2 += X1

	//V2 -= X2 * R;
   	vfms.f32 s21, s20, s18  //s21 = s21 - (s20 x s18) -> V2 -= X2 * R;

	//X2 += V2 - X2 * L;
	vadd.f32 s20,s21      //X2 = X2 + V2
	vfms.f32 s20,s20,s19  //s20 = s20 - (s20 x s19) -> X2 -= X2 * L;

	//X2 += V2 - X2 * L;
//	vmul.f32 s0,s20,s19  //s0 = X2*L
//	vsub.f32 s0,s21,s0   //s0 = V2 - s0
//	vadd.f32 s20,s0      //X2 = X2 + s0


	//Детектор и интегрирующая цепочка
    vabs.f32 s0, s20     //s0 = abs(X2)
    vadd.f32 s1, s0		 //s1 sum of samples

	/************ Окончание цикла ************/
	add		r1, #2			//pointer += 2;
    subs    r2, #1			//length --
    bne     2b


	/************ Обработка НЧ части *****************/

	vmul.f32 s1,s1,s24  //s1 = sum * M Компенсация коэффициента передачи фильтра

    /************ ФНЧ ********************************/
    //U1+=(s1 - U1) * K1;
    vsub.f32 s0, s1, s25  //s0 = (abs-U1);
    vfma.f32 s25,s0,s26	  //s25 = s25 + s0*s26 -> U1+=(ABS-U1)*K1;
	vmov   	s1, s25		  //s1 = U1

	/************ АРУ  *******************/
    //U2 += (s0 - U2) * K2;
    vsub.f32 s0, s1, s27  //s0 = (abs-U2);
    vfma.f32 s27,s0,s28	  //s27 = s27 + s0*s28 -> U2+=(ABS-U2)*K2;

	//if( U2 > AGCT) out = AGCT * s0/U2;
	vcmp.f32	s27,s31 	//U2 < AGCT
    vmrs APSR_nzcv,FPSCR
    ble 3f
    vdiv.f32    s1, s1,s27	// s1 = U1/U2
    vmul.f32	s1,	s1,s31	// s1 = s1 * AGCT

3: /************ ЦАП  *******************/

    vadd.f32 s1,s1,s30  // s1 = s1 + S_DAC
    vcvt.s32.f32 s1, s1 //(int) s1 = (float) s1
    vmov   	r0, s1		//r0 = (int) s1

    //if( r4 > 4095) r4 = 4095
	usat    r0, #12, r0	//Saturation

	str		r0,	[r6] //DAC1_DHR12R1

//  Раскомментировать для измерения скорости обработки
/*	mov		r0,	0x0	//On   PB7
	ldr     r1,	=GPIOB_ODR
	str		r0,	[r1] */

	pop {r0-r6,pc}

