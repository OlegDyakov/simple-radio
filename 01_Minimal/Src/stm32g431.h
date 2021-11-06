#ifndef __STM32G431_H
#define __STM32G431_H

//Определение типов данных, принятых в embedded  программировании
typedef     unsigned int 	uint32_t;
typedef     unsigned short 	uint16_t;
typedef 	unsigned char 	uint8_t ;
typedef 	int 			int32_t ;

//Макросы, упрощающие работу с регистрами, где
//REG - адрес регистра
//BIT - 32 битное слово - биты для которых будет выполнена данная функция.
//Например: INVERT(GPIOB_ODR,0x6)  проинвертирует биты 1 и 2 в регистре GPIOB_ODR

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define INVERT_BIT(REG, BIT)  ((REG) ^= (BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#define CLEAR_REG(REG)        ((REG) = (0x0))
#define READ_REG(REG)         ((REG))

//Макрос WRITE_REG выполняет запись 32 битного слова в регистр, где
//REG - адрес регистра
//VAL - значение
#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

//Макрос MODIFY_REG выполняет модификацию поля в регистре, где
//REG - адрес регистра
//CLEARMASK - 32 битное слово, биты для которых будет выполнена операция сброса в 0
//SETMASK   - 32 битное слово, биты для которых будет выполнена операция установки  в 1
//Например: 	MODIFY_REG(GPIOB_MODER,  0x00030000, 0x00010000) - сбрасывает биты 16 и 17 в 0,
//и устанавливает бит 16 в 1.

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

/*********************** RCC (Reset and clock control)  *********************************/
#define RCC_BASE	(0x40021000)
#define RCC_AHB2ENR	(*(volatile uint32_t *) (RCC_BASE + 0x4c))

#define RCC_AHB2ENR_GPIOBEN_Pos              (1U)
#define RCC_AHB2ENR_GPIOBEN                  0x00000002

/*********************** GPIO (General-purpose I/Os) *********************************/
#define GPIOB_BASE	(0x48000400)
#define GPIOB_MODER	(*(volatile uint32_t *) (GPIOB_BASE + 0x00))
#define GPIOB_ODR	(*(volatile uint32_t *) (GPIOB_BASE + 0x14))

//Bits definition for GPIO_MODER register
#define GPIO_MODER_MODE0_Pos           (0U)
#define GPIO_MODER_MODE0               0x00000003
#define GPIO_MODER_MODE1_Pos           (2U)
#define GPIO_MODER_MODE1               0x0000000C
#define GPIO_MODER_MODE2_Pos           (4U)
#define GPIO_MODER_MODE2               0x00000030
#define GPIO_MODER_MODE3_Pos           (6U)
#define GPIO_MODER_MODE3               0x000000C0
#define GPIO_MODER_MODE4_Pos           (8U)
#define GPIO_MODER_MODE4               0x00000300
#define GPIO_MODER_MODE5_Pos           (10U)
#define GPIO_MODER_MODE5               0x00000C00
#define GPIO_MODER_MODE6_Pos           (12U)
#define GPIO_MODER_MODE6               0x00003000
#define GPIO_MODER_MODE7_Pos           (14U)
#define GPIO_MODER_MODE7               0x0000C000
#define GPIO_MODER_MODE8_Pos           (16U)
#define GPIO_MODER_MODE8               0x00030000
#define GPIO_MODER_MODE9_Pos           (18U)
#define GPIO_MODER_MODE9               0x000C0000
#define GPIO_MODER_MODE10_Pos          (20U)
#define GPIO_MODER_MODE10              0x00300000
#define GPIO_MODER_MODE11_Pos          (22U)
#define GPIO_MODER_MODE11              0x00C00000
#define GPIO_MODER_MODE12_Pos          (24U)
#define GPIO_MODER_MODE12              0x03000000
#define GPIO_MODER_MODE13_Pos          (26U)
#define GPIO_MODER_MODE13              0x0C000000
#define GPIO_MODER_MODE14_Pos          (28U)
#define GPIO_MODER_MODE14              0x30000000
#define GPIO_MODER_MODE15_Pos          (30U)
#define GPIO_MODER_MODE15              0xC0000000

//Bits definition for GPIO_ODR register

#define GPIO_ODR_OD0_Pos               (0U)
#define GPIO_ODR_OD0                   0x00000001
#define GPIO_ODR_OD1_Pos               (1U)
#define GPIO_ODR_OD1                   0x00000002
#define GPIO_ODR_OD2_Pos               (2U)
#define GPIO_ODR_OD2                   0x00000004
#define GPIO_ODR_OD3_Pos               (3U)
#define GPIO_ODR_OD3                   0x00000008
#define GPIO_ODR_OD4_Pos               (4U)
#define GPIO_ODR_OD4                   0x00000010
#define GPIO_ODR_OD5_Pos               (5U)
#define GPIO_ODR_OD5                   0x00000020
#define GPIO_ODR_OD6_Pos               (6U)
#define GPIO_ODR_OD6                   0x00000040
#define GPIO_ODR_OD7_Pos               (7U)
#define GPIO_ODR_OD7                    0x00000080
#define GPIO_ODR_OD8_Pos               (8U)
#define GPIO_ODR_OD8                   0x00000100
#define GPIO_ODR_OD9_Pos               (9U)
#define GPIO_ODR_OD9                   0x00000200
#define GPIO_ODR_OD10_Pos              (10U)
#define GPIO_ODR_OD10                  0x00000400
#define GPIO_ODR_OD11_Pos              (11U)
#define GPIO_ODR_OD11                  0x00000800
#define GPIO_ODR_OD12_Pos              (12U)
#define GPIO_ODR_OD12                  0x00001000
#define GPIO_ODR_OD13_Pos              (13U)
#define GPIO_ODR_OD13                  0x00002000
#define GPIO_ODR_OD14_Pos              (14U)
#define GPIO_ODR_OD14                  0x00004000
#define GPIO_ODR_OD15_Pos              (15U)
#define GPIO_ODR_OD15                  0x00008000


#endif /* __STM32G431_H */

