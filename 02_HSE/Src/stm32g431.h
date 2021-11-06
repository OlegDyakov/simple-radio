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
#define RCC_CR		(*(volatile uint32_t *) (RCC_BASE + 0x00))
#define RCC_CFGR	(*(volatile uint32_t *) (RCC_BASE + 0x08))
#define RCC_PLLCFGR	(*(volatile uint32_t *) (RCC_BASE + 0x0c))
#define RCC_AHB2ENR	(*(volatile uint32_t *) (RCC_BASE + 0x4c))

// Bits definition for RCC_CR register

#define RCC_CR_HSEON                         0x00010000                  /*!< External High Speed oscillator (HSE) clock enable */
#define RCC_CR_HSERDY                        0x00020000                  /*!< External High Speed oscillator (HSE) clock ready */
#define RCC_CR_PLLON                         0x01000000                  /*!< System PLL clock enable */
#define RCC_CR_PLLRDY                        0x02000000

//Bit definition for RCC_CFGR register

#define RCC_CFGR_SW                           0x00000003                /*!< SW[1:0] bits (System clock Switch) */
/*SWS configuration */
#define RCC_CFGR_SWS                         0x0000000C                 /*!< SWS[1:0] bits (System Clock Switch Status) */

#define RCC_CFGR_SW_HSI                      (0x00000001U)              /*!< HSI16 oscillator selection as system clock */
#define RCC_CFGR_SW_HSE                      (0x00000002U)              /*!< HSE oscillator selection as system clock */
#define RCC_CFGR_SW_PLL                      (0x00000003U)              /*!< PLL selection as system clock */

#define RCC_CFGR_SWS_HSI                     (0x00000004U)              /*!< HSI16 oscillator used as system clock */
#define RCC_CFGR_SWS_HSE                     (0x00000008U)              /*!< HSE oscillator used as system clock */
#define RCC_CFGR_SWS_PLL                     (0x0000000CU)              /*!< PLL used as system clock */

/*PPRE1 configuration */
#define RCC_CFGR_PPRE1                       0x00000700                 /*!< PRE1[2:0] bits (APB2 prescaler) */
#define RCC_CFGR_PPRE1_DIV1                  (0x00000000U)              /*!< HCLK not divided */
#define RCC_CFGR_PPRE1_DIV2                  (0x00000400U)              /*!< HCLK divided by 2 */
#define RCC_CFGR_PPRE1_DIV4                  (0x00000500U)              /*!< HCLK divided by 4 */
#define RCC_CFGR_PPRE1_DIV8                  (0x00000600U)              /*!< HCLK divided by 8 */
#define RCC_CFGR_PPRE1_DIV16                 (0x00000700U)              /*!< HCLK divided by 16 */

/*PPRE2 configuration */
#define RCC_CFGR_PPRE2                       0x00003800                /*!< PRE2[2:0] bits (APB2 prescaler) */
#define RCC_CFGR_PPRE2_DIV1                  (0x00000000U)             /*!< HCLK not divided */
#define RCC_CFGR_PPRE2_DIV2                  (0x00002000U)             /*!< HCLK divided by 2 */
#define RCC_CFGR_PPRE2_DIV4                  (0x00002800U)             /*!< HCLK divided by 4 */
#define RCC_CFGR_PPRE2_DIV8                  (0x00003000U)             /*!< HCLK divided by 8 */
#define RCC_CFGR_PPRE2_DIV16                 (0x00003800U)             /*!< HCLK divided by 16 */

/*HPRE configuration */
#define RCC_CFGR_HPRE                        0x000000F0        		   /*!< HPRE[3:0] bits (AHB prescaler) */
#define RCC_CFGR_HPRE_DIV1                   (0x00000000U)             /*!< SYSCLK not divided */
#define RCC_CFGR_HPRE_DIV2                   (0x00000080U)             /*!< SYSCLK divided by 2 */
#define RCC_CFGR_HPRE_DIV4                   (0x00000090U)             /*!< SYSCLK divided by 4 */
#define RCC_CFGR_HPRE_DIV8                   (0x000000A0U)             /*!< SYSCLK divided by 8 */
#define RCC_CFGR_HPRE_DIV16                  (0x000000B0U)             /*!< SYSCLK divided by 16 */
#define RCC_CFGR_HPRE_DIV64                  (0x000000C0U)             /*!< SYSCLK divided by 64 */
#define RCC_CFGR_HPRE_DIV128                 (0x000000D0U)             /*!< SYSCLK divided by 128 */
#define RCC_CFGR_HPRE_DIV256                 (0x000000E0U)             /*!< SYSCLK divided by 256 */
#define RCC_CFGR_HPRE_DIV512                 (0x000000F0U)             /*!< SYSCLK divided by 512 */

// Bits definition for RCC_PLLCFGR register
#define RCC_PLLCFGR_PLLSRC                   0x00000003
#define RCC_PLLCFGR_PLLSRC_HSE               0x00000003   /*!< HSE oscillator source clock selected */
#define RCC_PLLCFGR_PLLM_Pos                 (4U)
#define RCC_PLLCFGR_PLLM                     0x000000F0
#define RCC_PLLCFGR_PLLN_Pos                 (8U)
#define RCC_PLLCFGR_PLLN                     0x00007F00
#define RCC_PLLCFGR_PLLREN                   0x01000000
#define RCC_PLLCFGR_PLLR_Pos                 (25U)
#define RCC_PLLCFGR_PLLR                     0x06000000

// Bits definition for RCC_AHB2ENR register
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

/*********************** FLASH *********************************/
#define FLASH_BASE	(0x40022000)
#define FLASH_ACR   (*(volatile uint32_t *) (FLASH_BASE + 0x00))

// Bits definition for FLASH_ACR register

#define FLASH_ACR_LATENCY_4WS             (0x00000004U)
#define FLASH_ACR_ICEN                    0x00000200
#define FLASH_ACR_DCEN                    0x00000400
#define FLASH_ACR_DBG_SWEN                0x00040000               /*!< Software disable for debugger */

/*********************** PWR *********************************/
#define PWR_BASE	(0x40007000)
#define PWR_CR5    (*(volatile uint32_t *) (PWR_BASE + 0x80))

// Bits definition for PWR_CR5 register
#define PWR_CR5_R1MODE   0x00000100    /*!< selection for Main Regulator in Range1 */

#endif /* __STM32G431_H */

