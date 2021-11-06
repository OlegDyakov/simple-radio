#include <stm32g431.h>
#include "oledfont.h"

//Блок переменных, обеспечивающий вывод строки символов через DMA

static uint8_t  I2C_buffer[10];        //Буфер передачи данных в I2C
static uint8_t  TransferComplete  = 1; //Флаг - "Завершение DMA посылки по каналу  I2C
static uint8_t  OutCharState =  0;     //Состояние вывода символа (0 - 5)
static char *CharCurrentPosition;	   //Текущее положение указателя в выводимой строке
static uint8_t oled_x;				   //Позиция x (0 - 127) вывода байта
static uint8_t oled_y;				   //Позиция y (0 -4) вывода байта

//Параметры интерфейса I2C
#define SLAVE_OWN_ADDRESS  0x78  //Адрес OLED индикатора
#define I2C_ADDRSLAVE_7BIT  0    //Формат адреса OLED индикатора
#define I2C_MODE_AUTOEND  1 << 25
#define I2C_GENERATE_START_WRITE  1 << 13

void print_process(void);

//Запуск передачи данных по кналу I2C
static inline  void I2C2_HandleTransfer(uint32_t SlaveAddr, uint32_t SlaveAddrSize,
                                           uint32_t TransferSize, uint32_t EndMode, uint32_t Request)
{
  MODIFY_REG(I2C2_CR2, I2C_CR2_SADD | I2C_CR2_ADD10 |
             (I2C_CR2_RD_WRN & (uint32_t)(Request >> (31U - I2C_CR2_RD_WRN_Pos))) |
             I2C_CR2_START | I2C_CR2_STOP | I2C_CR2_RELOAD |
             I2C_CR2_NBYTES | I2C_CR2_AUTOEND | I2C_CR2_HEAD10R,
             SlaveAddr | SlaveAddrSize | (TransferSize << I2C_CR2_NBYTES_Pos) | EndMode | Request);
}

//Передача n байт из буфера I2C_buffer[10]
//по каналу I2C с использованием DMA

#define WAIT_DMA     0
#define NO_WAIT_DMA  1

void OLED_Send_Data(uint32_t n , uint8_t mode)
{
	TransferComplete = 0;

	CLEAR_BIT(DMA1_Channel3_CCR, DMA_CCR_EN);  			//Отключение канала DMA
	MODIFY_REG(DMA1_Channel3_CNDTR, DMA_CNDTR_NDT, n);  //Запись количесва передаваемых байт n
	SET_BIT(DMA1_Channel3_CCR, DMA_CCR_EN);  			//Включение канала DMA

	//Запуск передачи данных по кналу I2C
	I2C2_HandleTransfer(SLAVE_OWN_ADDRESS, I2C_ADDRSLAVE_7BIT, n, I2C_MODE_AUTOEND,  I2C_GENERATE_START_WRITE);

    //Ожидание окончания пенредачи
	if( mode == WAIT_DMA){
		while (TransferComplete == 0);
	}
}


//Передача однобайтной комманды в OLED индикатор
void OLED_SEND_CMD(uint8_t cmd)
{
	I2C_buffer[0] = 0;
	I2C_buffer[1] = cmd;
	OLED_Send_Data(2 , WAIT_DMA);

}

//Передача комманды SetPosition в OLED индикатор
void OLED_Set_Pos(unsigned char x, unsigned char y, uint8_t mode)
{

	I2C_buffer[0] = 0;
	I2C_buffer[1] = 0xb0+y;
	I2C_buffer[2] = ((x&0xf0)>>4)|0x10;
	I2C_buffer[3] = (x&0x0f);

    OLED_Send_Data(4 , mode);
}

//Очистка экрана OLED индикатора
void OLED_Clear(void)
{
	uint8_t y, i, n;
	for( y = 0; y<4; y++)
	{

		OLED_Set_Pos(0, y, WAIT_DMA);
		for(i = 0; i< 16; i++)
		{
			I2C_buffer[0]   = 0x40;
			for(n=1;n<9;n++)I2C_buffer[n] = 0x0;
			OLED_Send_Data(9,WAIT_DMA);
		}
	}
}


//Настройка канала I2C2
//PA8 -> I2C2_SDA
//PA9 -> I2C2_SCL

void I2C2_init(void){

	// Подать на модуль I2C2 тактовый сигнал
	SET_BIT(RCC_APB1ENR1, 1<<22);

	// Включить  PA8 и PA9 на альтерналивную функцию
	 MODIFY_REG(GPIOA_MODER,  GPIO_MODER_MODE9 | GPIO_MODER_MODE8, 2<<GPIO_MODER_MODE9_Pos | 2<<GPIO_MODER_MODE8_Pos );

	 // Включить  у PA8 и PA9 режим Pull UP
	 MODIFY_REG(GPIOA_PUPDR,  GPIO_MODER_MODE9 | GPIO_MODER_MODE8, 1<<GPIO_MODER_MODE9_Pos | 1<<GPIO_MODER_MODE8_Pos);

	 //Выбрать для PA8 и PA9 альтернативную функцию 4 (I2C2_SDA и I2C2_SCL)
	 MODIFY_REG(GPIOA_AFRH, GPIO_AFRH_AFSEL9_Msk | GPIO_AFRH_AFSEL8_Msk,  (4UL << GPIO_AFRH_AFSEL9_Pos) | (4UL << GPIO_AFRH_AFSEL8_Pos)) ;

	//Настройка тактовой частоты CSL ~ 400 KHz
	WRITE_REG(I2C2_TIMINGR, 0x00C00000 + (200 << 8) + 200);

	//Разрешить генерацию запроса на DMA
	SET_BIT(I2C2_CR1, I2C_CR1_TXDMAEN);

	//Запустить модуль I2C2
	SET_BIT(I2C2_CR1, I2C_CR1_PE);
}

// Стандартная последовательность инициализации OLED SSD1306
void OLED_StandartInit(void)
{
	OLED_SEND_CMD(0xAE);//OFF

	OLED_SEND_CMD(0x40);//---set low column address
	OLED_SEND_CMD(0xB0);//---set high column address

	OLED_SEND_CMD(0xC8);//-not offset

	OLED_SEND_CMD(0x81);//Set Contrast Control
	OLED_SEND_CMD(0xff);

	OLED_SEND_CMD(0xa1);//Set Segment Re-map

	OLED_SEND_CMD(0xa6);//Set Normal Display

	OLED_SEND_CMD(0xa8);//Set Multiplex Ratio
	OLED_SEND_CMD(0x1f);

	OLED_SEND_CMD(0xd3);//Set Display Offset
	OLED_SEND_CMD(0x00);

	OLED_SEND_CMD(0xd5);//Set Display Clock
	OLED_SEND_CMD(0xf0);

	OLED_SEND_CMD(0xd9);//Set Pre-charge Period
	OLED_SEND_CMD(0x22);

	OLED_SEND_CMD(0xda);//Set COM Pins
	OLED_SEND_CMD(0x02);

	OLED_SEND_CMD(0xdb);//Set V COMH Deselect Level
	OLED_SEND_CMD(0x49);

	OLED_SEND_CMD(0x8d);//Set Charge Pump
	OLED_SEND_CMD(0x14);

	OLED_SEND_CMD(0xAF); //ON

	OLED_Clear();

}


//Настройка контролера DMA1 для работы с I2C2
void DMA1_I2C2_Init(void)
{

    SET_BIT(RCC_AHB1ENR, RCC_AHB1ENR_DMAMUX1EN); //Подача тактового сигнала на DMAMUX1
    SET_BIT(RCC_AHB1ENR, RCC_AHB1ENR_DMA1EN);	 //Подача тактового сигнала на DMA1

    //Настройка для I2C2 на DMA1_Channel3
	MODIFY_REG(DMAMUX1_Channel2_CCR, DMAMUX_CxCR_DMAREQ_ID, DMAMUX_REQ_I2C2_TX);    //Подключаем I2C2 к 3-му каналу DMA. В DMAMUX каналы идут с 0, DMA каналы идут с 1
	MODIFY_REG(DMA1_Channel3_CCR, DMA_CCR_DIR | DMA_CCR_MEM2MEM, DMA_CCR_DIR);   	//DMA_DIRECTION_MEMORY_TO_PERIPH
	MODIFY_REG(DMA1_Channel3_CCR, DMA_CCR_PL, DMA_CCR_PL_1); 	//DMA_PRIORITY_HIGH
	MODIFY_REG(DMA1_Channel3_CCR, DMA_CCR_CIRC, 0); 		 	//DMA_MODE_NORMAL
	MODIFY_REG(DMA1_Channel3_CCR, DMA_CCR_PINC, 0);  		 	//DMA_PERIPH_NOINCREMENT
	MODIFY_REG(DMA1_Channel3_CCR, DMA_CCR_MINC, DMA_CCR_MINC); 	//DMA_MEMORY_INCREMENT
	MODIFY_REG(DMA1_Channel3_CCR, DMA_CCR_PSIZE, 0); 			//DMA_PDATAALIGN_BYTE   (8 bits)
	MODIFY_REG(DMA1_Channel3_CCR, DMA_CCR_MSIZE, 0); 			//LL_DMA_MDATAALIGN_BYTE (8 bits)

	SET_BIT(NVIC_ISER0, (1 << 13));	 //Разрешить в NVIC прерывание #13 (DMA1_CH3)
	NVIC_IPR3 = 0x00000000;			 //Установка приоритета  0 для прерывания 13.

	// Конфигурирование источника и приемника данных
	WRITE_REG(DMA1_Channel3_CMAR, (uint32_t)I2C_buffer);     //Source
	WRITE_REG(DMA1_Channel3_CPAR, (uint32_t)&(I2C2_TXDR));   //Destination

	SET_BIT(DMA1_Channel3_CCR, DMA_CCR_TCIE); //Разрешить прерывания от DMA - transfer complete
}


//I2C2 DMA interrupt handler
void DMA1_CH3_IRQHandler()
{

	if(READ_BIT(DMA1_ISR, DMA_ISR_TCIF3) == DMA_ISR_TCIF3){
		WRITE_REG(DMA1_IFCR, DMA_IFCR_CTCIF3);
		TransferComplete = 1;				   //Установить флаг "Завершение DMA посылки"
		if(OutCharState != 0) print_process(); //Если активна операция print, то запустить обработчик
	}

}


//Инициализация OLED
void OLED_Init (void)
{
	I2C2_init();
	DMA1_I2C2_Init();
	OLED_StandartInit();
}

//Обработчик функции "print"
void print_process(void)
{
	int i;
	char c = *CharCurrentPosition - ' ';
	switch(OutCharState)
	{
		case 1: OLED_Set_Pos(oled_x, oled_y, NO_WAIT_DMA);
				break;
		case 2: I2C_buffer[0]   = 0x40;
				for(i=1;i<9;i++)
					I2C_buffer[i]   = F8X16[c*16+i -1];
				OLED_Send_Data(9, NO_WAIT_DMA);
				break;
		case 3: OLED_Set_Pos(oled_x, oled_y+1, NO_WAIT_DMA);
				break;
		case 4:	I2C_buffer[0]   = 0x40;
				for(i=1;i<9;i++) I2C_buffer[i] = F8X16[c*16+i+ 7];
				OLED_Send_Data(9, NO_WAIT_DMA);
				break;
		case 5:	CharCurrentPosition++;
				if(*CharCurrentPosition == 0){
					OutCharState = 0;
					return;
				} else {
					oled_x +=8;
					OLED_Set_Pos(oled_x, oled_y, NO_WAIT_DMA);
					OutCharState = 2;
					return;
				}
				break;
		default: OutCharState = 0;
	}
	OutCharState++;
}

//Печать строки на OLED в асинхронном режиме
void print(uint8_t x, uint8_t y, char *str)
{
	while (OutCharState != 0);
	OutCharState = 1;
	CharCurrentPosition = str;
	oled_x = x;
	oled_y = y;
	print_process();

}

//DMA выводит символ ?
int IsDMA_Busy(void)
{
	if(OutCharState == 0) return 0;
	return 1;
}

