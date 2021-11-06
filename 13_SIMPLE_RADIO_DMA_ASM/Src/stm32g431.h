#ifndef __STM32G431_H
#define __STM32G431_H


//Определение типов данных, принятых в embedded  программировании
typedef     unsigned int 	uint32_t;
typedef     unsigned short 	uint16_t;
typedef     unsigned short 	int16_t;
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

#define PERIPH_BASE      (0x40000000UL) /*!< Peripheral base address */

/*********************** RCC (Reset and clock control)  *********************************/
#define RCC_BASE		(0x40021000)
#define RCC_CR			(*(volatile uint32_t *) (RCC_BASE + 0x00))
#define RCC_CFGR		(*(volatile uint32_t *) (RCC_BASE + 0x08))
#define RCC_PLLCFGR		(*(volatile uint32_t *) (RCC_BASE + 0x0c))
#define RCC_AHB2RSTR    (*(volatile uint32_t *) (RCC_BASE + 0x2C)) /*RCC AHB2 peripheral reset register, Address offset: 0x2C */
#define RCC_AHB1ENR    (*(volatile uint32_t *) (RCC_BASE + 0x48)) /*RCC AHB1 peripheral clocks enable register, Address offset: 0x48 */
#define RCC_AHB2ENR		(*(volatile uint32_t *) (RCC_BASE + 0x4c))
#define RCC_APB1ENR1	(*(volatile uint32_t *) (RCC_BASE + 0x58))
#define RCC_CCIPR		(*(volatile uint32_t *) (RCC_BASE + 0x88))

// Bits definition for RCC_CR register

#define RCC_CR_HSEON                         0x00010000                  /* External High Speed oscillator (HSE) clock enable */
#define RCC_CR_HSERDY                        0x00020000                  /* External High Speed oscillator (HSE) clock ready */
#define RCC_CR_PLLON                         0x01000000                  /* System PLL clock enable */
#define RCC_CR_PLLRDY                        0x02000000

//Bit definition for RCC_CFGR register

#define RCC_CFGR_SW                           0x00000003                /* SW[1:0] bits (System clock Switch) */
/*SWS configuration */
#define RCC_CFGR_SWS                         0x0000000C                 /* SWS[1:0] bits (System Clock Switch Status) */

#define RCC_CFGR_SW_HSI                      (0x00000001U)              /* HSI16 oscillator selection as system clock */
#define RCC_CFGR_SW_HSE                      (0x00000002U)              /* HSE oscillator selection as system clock */
#define RCC_CFGR_SW_PLL                      (0x00000003U)              /* PLL selection as system clock */

#define RCC_CFGR_SWS_HSI                     (0x00000004U)              /* HSI16 oscillator used as system clock */
#define RCC_CFGR_SWS_HSE                     (0x00000008U)              /* HSE oscillator used as system clock */
#define RCC_CFGR_SWS_PLL                     (0x0000000CU)              /* PLL used as system clock */

/*PPRE1 configuration */
#define RCC_CFGR_PPRE1                       0x00000700                 /* PRE1[2:0] bits (APB2 prescaler) */
#define RCC_CFGR_PPRE1_DIV1                  (0x00000000U)              /* HCLK not divided */
#define RCC_CFGR_PPRE1_DIV2                  (0x00000400U)              /* HCLK divided by 2 */
#define RCC_CFGR_PPRE1_DIV4                  (0x00000500U)              /* HCLK divided by 4 */
#define RCC_CFGR_PPRE1_DIV8                  (0x00000600U)              /* HCLK divided by 8 */
#define RCC_CFGR_PPRE1_DIV16                 (0x00000700U)              /* HCLK divided by 16 */

/*PPRE2 configuration */
#define RCC_CFGR_PPRE2                       0x00003800                /* PRE2[2:0] bits (APB2 prescaler) */
#define RCC_CFGR_PPRE2_DIV1                  (0x00000000U)             /* HCLK not divided */
#define RCC_CFGR_PPRE2_DIV2                  (0x00002000U)             /* HCLK divided by 2 */
#define RCC_CFGR_PPRE2_DIV4                  (0x00002800U)             /* HCLK divided by 4 */
#define RCC_CFGR_PPRE2_DIV8                  (0x00003000U)             /* HCLK divided by 8 */
#define RCC_CFGR_PPRE2_DIV16                 (0x00003800U)             /* HCLK divided by 16 */

/*HPRE configuration */
#define RCC_CFGR_HPRE                        0x000000F0        		   /* HPRE[3:0] bits (AHB prescaler) */
#define RCC_CFGR_HPRE_DIV1                   (0x00000000U)             /* SYSCLK not divided */
#define RCC_CFGR_HPRE_DIV2                   (0x00000080U)             /* SYSCLK divided by 2 */
#define RCC_CFGR_HPRE_DIV4                   (0x00000090U)             /* SYSCLK divided by 4 */
#define RCC_CFGR_HPRE_DIV8                   (0x000000A0U)             /* SYSCLK divided by 8 */
#define RCC_CFGR_HPRE_DIV16                  (0x000000B0U)             /* SYSCLK divided by 16 */
#define RCC_CFGR_HPRE_DIV64                  (0x000000C0U)             /* SYSCLK divided by 64 */
#define RCC_CFGR_HPRE_DIV128                 (0x000000D0U)             /* SYSCLK divided by 128 */
#define RCC_CFGR_HPRE_DIV256                 (0x000000E0U)             /* SYSCLK divided by 256 */
#define RCC_CFGR_HPRE_DIV512                 (0x000000F0U)             /* SYSCLK divided by 512 */

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
#define RCC_AHB2ENR_GPIOAEN_Pos              (0U)
#define RCC_AHB2ENR_GPIOAEN                  0x00000001
#define RCC_AHB2ENR_GPIOBEN_Pos              (1U)
#define RCC_AHB2ENR_GPIOBEN                  0x00000002
#define RCC_AHB2ENR_DAC1EN_Pos               (16U)
#define RCC_AHB2ENR_DAC1EN                   0x00010000

// Bit definition for RCC_APB1ENR1 register
#define RCC_APB1ENR1_TIM2EN_Pos              (0U)
#define RCC_APB1ENR1_TIM2EN                  0x00000001
#define RCC_APB1ENR1_TIM3EN_Pos              (1U)
#define RCC_APB1ENR1_TIM3EN                  0x00000002
#define RCC_APB1ENR1_TIM4EN_Pos              (2U)
#define RCC_APB1ENR1_TIM4EN                  0x00000004
#define RCC_APB1ENR1_USART2EN_Pos            (17U)
#define RCC_APB1ENR1_USART2EN                0x00020000

// Bit definition for RCC_AHB1ENR register
#define RCC_AHB1ENR_DMA1EN_Pos               (0U)
#define RCC_AHB1ENR_DMA1EN_Msk               (0x1UL << RCC_AHB1ENR_DMA1EN_Pos) /*!< 0x00000001 */
#define RCC_AHB1ENR_DMA1EN                   RCC_AHB1ENR_DMA1EN_Msk
#define RCC_AHB1ENR_DMAMUX1EN_Pos            (2U)
#define RCC_AHB1ENR_DMAMUX1EN_Msk            (0x1UL << RCC_AHB1ENR_DMAMUX1EN_Pos)/*!< 0x00000004 */
#define RCC_AHB1ENR_DMAMUX1EN                RCC_AHB1ENR_DMAMUX1EN_Msk

// Bit definition for RCC_CCIPR register
#define RCC_CCIPR_USART2SEL_Pos              (2U)
#define RCC_CCIPR_USART2SEL                  0x0000000C

/*********************** GPIO (General-purpose I/Os) *********************************/
#define GPIOA_BASE    (0x48000000)
#define GPIOB_BASE	  (0x48000400)
#define GPIOA_MODER	  (*(volatile uint32_t *) (GPIOA_BASE + 0x00))
#define GPIOA_OTYPER  (*(volatile uint32_t *) (GPIOA_BASE + 0x04))   /*GPIO port output type register,        Address offset: 0x04      */
#define GPIOA_OSPEEDR (*(volatile uint32_t *) (GPIOA_BASE + 0x08))   /*GPIO port output speed register,       Address offset: 0x08      */
#define GPIOA_PUPDR   (*(volatile uint32_t *) (GPIOA_BASE + 0x0C))   /*GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
#define GPIOA_ODR	  (*(volatile uint32_t *) (GPIOA_BASE + 0x14))
#define GPIOA_AFRL	  (*(volatile uint32_t *) (GPIOA_BASE + 0x20))
#define GPIOA_AFRH	  (*(volatile uint32_t *) (GPIOA_BASE + 0x24))

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

// Bit definition for GPIO_AFRL register

#define GPIO_AFRL_AFSEL2_Pos           (8U)
#define GPIO_AFRL_AFSEL2               0x00000F00
#define GPIO_AFRL_AFSEL3_Pos           (12U)
#define GPIO_AFRL_AFSEL3               0x0000F000
#define GPIO_AFRL_AFSEL5_Pos           (20U)
#define GPIO_AFRL_AFSEL5               0x00F00000
#define GPIO_AFRL_AFSEL6_Pos           (24U)
#define GPIO_AFRL_AFSEL6               0x0F000000
#define GPIO_AFRL_AFSEL7_Pos           (28U)
#define GPIO_AFRL_AFSEL7               0xF0000000

#define GPIO_AFRH_AFSEL8_Pos           (0U)
#define GPIO_AFRH_AFSEL8_Msk           (0xFUL << GPIO_AFRH_AFSEL8_Pos)         /*!< 0x0000000F */
#define GPIO_AFRH_AFSEL9_Pos           (4U)
#define GPIO_AFRH_AFSEL9_Msk           (0xFUL << GPIO_AFRH_AFSEL9_Pos)         /*!< 0x000000F0 */

// Bit definition for GPIO_AFRH register
#define GPIO_AFRH_AFSEL15_Pos          (28U)
#define GPIO_AFRH_AFSEL15              0xF0000000

/*********************** FLASH *********************************/
#define FLASH_BASE	(0x40022000)
#define FLASH_ACR   (*(volatile uint32_t *) (FLASH_BASE + 0x00))

// Bits definition for FLASH_ACR register

#define FLASH_ACR_LATENCY_4WS             (0x00000004U)
#define FLASH_ACR_ICEN                    0x00000200
#define FLASH_ACR_DCEN                    0x00000400
#define FLASH_ACR_DBG_SWEN                0x00040000               /* Software disable for debugger */

/*********************** PWR *********************************/
#define PWR_BASE	(0x40007000)
#define PWR_CR5    (*(volatile uint32_t *) (PWR_BASE + 0x80))

// Bits definition for PWR_CR5 register
#define PWR_CR5_R1MODE   0x00000100    /* selection for Main Regulator in Range1 */

/*********************** NVIC  *********************************/
#define NVIC_BASE           (0xE000E100)                    /* NVIC Base Address */
#define NVIC_ISER0			(*(volatile uint32_t *) (NVIC_BASE + 0x0))
#define NVIC_ISER1			(*(volatile uint32_t *) (NVIC_BASE + 0x04))
#define NVIC_IPR3			(*(volatile uint32_t *) (0xe000e40C))
#define NVIC_IPR4			(*(volatile uint32_t *) (0xe000e410))
#define NVIC_IPR5			(*(volatile uint32_t *) (0xe000e414))
#define NVIC_IPR6			(*(volatile uint32_t *) (0xe000e418))
#define NVIC_IPR7			(*(volatile uint32_t *) (0xe000e41c))


/********************** USART **********************************/
#define USART2_BASE         (0x40004400UL)
#define USART2_CR1			(*(volatile uint32_t *) (USART2_BASE + 0x0))  /* USART Control register 1, Address offset: 0x00  */
#define USART2_CR2			(*(volatile uint32_t *) (USART2_BASE + 0x4))  /* USART Control register 2, Address offset: 0x04  */
#define USART2_CR3			(*(volatile uint32_t *) (USART2_BASE + 0x8))  /* USART Control register 3, Address offset: 0x08  */
#define USART2_BRR			(*(volatile uint32_t *) (USART2_BASE + 0xC))  /* USART Baud rate register, Address offset: 0x0C  */
#define USART2_ISR          (*(volatile uint32_t *) (USART2_BASE + 0x1C)) /* USART Interrupt and status register, Address offset: 0x1C  */
#define USART2_ICR          (*(volatile uint32_t *) (USART2_BASE + 0x20)) /* USART Interrupt flag Clear register, Address offset: 0x20  */
#define USART2_RDR          (*(volatile uint32_t *) (USART2_BASE + 0x24)) /* USART Receive Data register, Address offset: 0x24  */
#define USART2_TDR          (*(volatile uint32_t *) (USART2_BASE + 0x28)) /* USART Transmit Data register, Address offset: 0x28  */
#define USART2_PRESC        (*(volatile uint32_t *) (USART2_BASE + 0x2C)) /* USART Prescaler register, Address offset: 0x2C  */

// Bit definition for USART_CR1 register
#define USART_CR1_UE_Pos             (0U)
#define USART_CR1_UE                 0x00000001 /* USART Enable */
#define USART_CR1_RXNEIE_Pos         (5U)
#define USART_CR1_RXNEIE             0x00000020 /* RXNE Interrupt Enable */
#define USART_CR1_TCIE_Pos           (6U)
#define USART_CR1_TCIE               0x00000040 /* Transmission Complete Interrupt Enable */


// Bit definition for USART_ISR register
#define USART_ISR_RXNE_Pos           (5U)
#define USART_ISR_RXNE               0x00000020 /* Read Data Register Not Empty */
#define USART_ISR_TC_Pos             (6U)
#define USART_ISR_TC                 0x00000040 /* Transmission Complete */
#define USART_ISR_TEACK_Pos          (21U)
#define USART_ISR_TEACK              0x00200000 /* Transmit Enable Acknowledge Flag */
#define USART_ISR_REACK_Pos          (22U)
#define USART_ISR_REACK              0x00400000 /* Receive Enable Acknowledge Flag */

/********************** TIM2 **********************************/
#define TIM2_BASE        0x40000000UL
#define TIM2_CR1         (*(volatile uint32_t *) (TIM2_BASE + 0x0))  /*TIM control register 1,                   Address offset: 0x00 */
#define TIM2_CR2         (*(volatile uint32_t *) (TIM2_BASE + 0x4))  /*TIM control register 2,                   Address offset: 0x04 */
#define TIM2_DIER	     (*(volatile uint32_t *) (TIM2_BASE + 0xC))  /*TIM DMA/interrupt enable register,        Address offset: 0x0C */
#define TIM2_SR          (*(volatile uint32_t *) (TIM2_BASE + 0x10)) /*TIM status register,                      Address offset: 0x10 */
#define TIM2_PSC         (*(volatile uint32_t *) (TIM2_BASE + 0x28)) /*TIM prescaler,                            Address offset: 0x28 */
#define TIM2_ARR         (*(volatile uint32_t *) (TIM2_BASE + 0x2C)) /*TIM auto-reload register,                 Address offset: 0x2C */

/******************* TIM bits **************************************/
// Bit definition for TIM_CR1 register
#define TIM_CR1_CEN_Pos           (0U)
#define TIM_CR1_CEN               0x00000001                              /* Counter enable */

// Bit definition for TIM_CR2 register
#define TIM_CR2_MMS_1             0x00000020


// Bit definition for TIM_SR register
#define TIM_SR_UIF_Pos            (0U)
#define TIM_SR_UIF                0x00000001             /*Update interrupt Flag */
#define TIM_SR_TIF_Pos            (6U)
#define TIM_SR_TIF               0x00000040              /*!<Trigger interrupt Flag */

//  Bit definition for TIM_DIER register
#define TIM_DIER_UIE_Pos          (0U)
#define TIM_DIER_UIE              0x00000001             /*Update interrupt enable */
#define TIM_DIER_TIE_Pos          (6U)
#define TIM_DIER_TIE              0x00000040             /*!<Trigger interrupt enable */

//  Bit definition for TIM_CCER register
#define TIM_CCER_CC1E_Pos         (0U)
#define TIM_CCER_CC1E             0x00000001             /*Capture/Compare 1 output enable */
#define TIM_CCER_CC1P_Pos         (1U)
#define TIM_CCER_CC1P             0x00000002             /*Capture/Compare 1 output Polarity */
#define TIM_CCER_CC2P_Pos         (5U)
#define TIM_CCER_CC2P             0x00000020             /*!<Capture/Compare 2 output Polarity */
#define TIM_CCER_CC2NP_Pos        (7U)
#define TIM_CCER_CC2NP            0x00000080             /*!<Capture/Compare 2 Complementary output Polarity */

//  Bit definition for TIM_CCMR1  register
#define TIM_CCMR1_CC1S_Pos        (0U)
#define TIM_CCMR1_CC1S_Msk        (0x3UL << TIM_CCMR1_CC1S_Pos)                /*!< 0x00000003 */
#define TIM_CCMR1_CC1S            TIM_CCMR1_CC1S_Msk                           /*!<CC1S[1:0] bits (Capture/Compare 1 Selection) */
#define TIM_CCMR1_CC1S_0          (0x1UL << TIM_CCMR1_CC1S_Pos)                /*!< 0x00000001 */
#define TIM_CCMR1_CC1S_1          (0x2UL << TIM_CCMR1_CC1S_Pos)                /*!< 0x00000002 */

#define TIM_CCMR1_IC1F_Pos        (4U)
#define TIM_CCMR1_IC1F_Msk        (0xFUL << TIM_CCMR1_IC1F_Pos)                /*!< 0x000000F0 */
#define TIM_CCMR1_IC1F            TIM_CCMR1_IC1F_Msk                           /*!<IC1F[3:0] bits (Input Capture 1 Filter) */
#define TIM_CCMR1_IC1F_0          (0x1UL << TIM_CCMR1_IC1F_Pos)                /*!< 0x00000010 */
#define TIM_CCMR1_IC1F_1          (0x2UL << TIM_CCMR1_IC1F_Pos)                /*!< 0x00000020 */
#define TIM_CCMR1_IC1F_2          (0x4UL << TIM_CCMR1_IC1F_Pos)                /*!< 0x00000040 */
#define TIM_CCMR1_IC1F_3          (0x8UL << TIM_CCMR1_IC1F_Pos)                /*!< 0x00000080 */

#define TIM_CCMR1_CC2S_Pos        (8U)
#define TIM_CCMR1_CC2S_Msk        (0x3UL << TIM_CCMR1_CC2S_Pos)                /*!< 0x00000300 */
#define TIM_CCMR1_CC2S            TIM_CCMR1_CC2S_Msk                           /*!<CC2S[1:0] bits (Capture/Compare 2 Selection) */
#define TIM_CCMR1_CC2S_0          (0x1UL << TIM_CCMR1_CC2S_Pos)                /*!< 0x00000100 */
#define TIM_CCMR1_CC2S_1          (0x2UL << TIM_CCMR1_CC2S_Pos)

#define TIM_CCMR1_IC2F_Pos        (12U)
#define TIM_CCMR1_IC2F_Msk        (0xFUL << TIM_CCMR1_IC2F_Pos)                /*!< 0x0000F000 */
#define TIM_CCMR1_IC2F            TIM_CCMR1_IC2F_Msk                           /*!<IC2F[3:0] bits (Input Capture 2 Filter) */
#define TIM_CCMR1_IC2F_0          (0x1UL << TIM_CCMR1_IC2F_Pos)                /*!< 0x00001000 */
#define TIM_CCMR1_IC2F_1          (0x2UL << TIM_CCMR1_IC2F_Pos)                /*!< 0x00002000 */
#define TIM_CCMR1_IC2F_2          (0x4UL << TIM_CCMR1_IC2F_Pos)                /*!< 0x00004000 */
#define TIM_CCMR1_IC2F_3          (0x8UL << TIM_CCMR1_IC2F_Pos)                /*!< 0x00008000 */

// Bit definition for TIM_SMCR register
#define TIM_SMCR_SMS_Pos          (0U)
#define TIM_SMCR_SMS_Msk          (0x10007UL << TIM_SMCR_SMS_Pos)              /*!< 0x00010007 */
#define TIM_SMCR_SMS              TIM_SMCR_SMS_Msk                             /*!<SMS[2:0] bits (Slave mode selection) */
#define TIM_SMCR_SMS_0            (0x00001UL << TIM_SMCR_SMS_Pos)              /*!< 0x00000001 */
#define TIM_SMCR_SMS_1            (0x00002UL << TIM_SMCR_SMS_Pos)              /*!< 0x00000002 */
#define TIM_SMCR_SMS_2            (0x00004UL << TIM_SMCR_SMS_Pos)              /*!< 0x00000004 */
#define TIM_SMCR_SMS_3            (0x10000UL << TIM_SMCR_SMS_Pos)              /*!< 0x00010000 */

#define TIM_SMCR_TS_Pos           (4U)
#define TIM_SMCR_TS_Msk           (0x30007UL << TIM_SMCR_TS_Pos)               /*!< 0x00300070 */
#define TIM_SMCR_TS               TIM_SMCR_TS_Msk                              /*!<TS[2:0] bits (Trigger selection) */
#define TIM_SMCR_TS_0             (0x00001UL << TIM_SMCR_TS_Pos)               /*!< 0x00000010 */
#define TIM_SMCR_TS_1             (0x00002UL << TIM_SMCR_TS_Pos)               /*!< 0x00000020 */
#define TIM_SMCR_TS_2             (0x00004UL << TIM_SMCR_TS_Pos)               /*!< 0x00000040 */
#define TIM_SMCR_TS_3             (0x10000UL << TIM_SMCR_TS_Pos)               /*!< 0x00100000 */
#define TIM_SMCR_TS_4             (0x20000UL << TIM_SMCR_TS_Pos)               /*!< 0x00200000 */

/********************** TIM3  **********************************/
#define TIM3_BASE        0x40000400UL
#define TIM3_CR1         (*(volatile uint32_t *) (TIM3_BASE + 0x0))  //TIM control register 1,                   Address offset: 0x00
#define TIM3_CR2         (*(volatile uint32_t *) (TIM3_BASE + 0x4))  //TIM control register 2,
#define TIM3_SMCR        (*(volatile uint32_t *) (TIM3_BASE + 0x8))  // TIM slave mode control register,         Address offset: 0x08 */
#define TIM3_DIER	     (*(volatile uint32_t *) (TIM3_BASE + 0xC))  //TIM DMA/interrupt enable register,        Address offset: 0x0C
#define TIM3_SR          (*(volatile uint32_t *) (TIM3_BASE + 0x10)) //TIM status register,                      Address offset: 0x10
#define TIM3_CCMR1       (*(volatile uint32_t *) (TIM3_BASE + 0x18)) //TIM capture/compare mode register 1,      Address offset: 0x18 */
#define TIM3_CCER        (*(volatile uint32_t *) (TIM3_BASE + 0x20)) //TIM capture/compare enable register,      Address offset: 0x20 */
#define TIM3_CNT         (*(volatile uint32_t *) (TIM3_BASE + 0x24)) //TIM counter register,                     Address offset: 0x24
#define TIM3_PSC         (*(volatile uint32_t *) (TIM3_BASE + 0x28)) //TIM prescaler,                            Address offset: 0x28
#define TIM3_ARR         (*(volatile uint32_t *) (TIM3_BASE + 0x2C)) //TIM auto-reload register,
#define TIM3_CCR1        (*(volatile uint32_t *) (TIM3_BASE + 0x34)) //TIM capture/compare register 1,           Address offset: 0x34
#define TIM3_BDTR        (*(volatile uint32_t *) (TIM3_BASE + 0x44)) //TIM break and dead-time register,

/********************** TIM4  **********************************/
#define TIM4_BASE        (APB1PERIPH_BASE + 0x0800UL)
#define TIM4_CR1         (*(volatile uint32_t *) (TIM4_BASE + 0x0))  //TIM control register 1,                   Address offset: 0x00
#define TIM4_CR2         (*(volatile uint32_t *) (TIM4_BASE + 0x4))  //TIM control register 2,
#define TIM4_SMCR        (*(volatile uint32_t *) (TIM4_BASE + 0x8))  // TIM slave mode control register,         Address offset: 0x08 */
#define TIM4_DIER	     (*(volatile uint32_t *) (TIM4_BASE + 0xC))  //TIM DMA/interrupt enable register,        Address offset: 0x0C
#define TIM4_SR          (*(volatile uint32_t *) (TIM4_BASE + 0x10)) //TIM status register,                      Address offset: 0x10
#define TIM4_CCMR1       (*(volatile uint32_t *) (TIM4_BASE + 0x18)) //TIM capture/compare mode register 1,      Address offset: 0x18 */
#define TIM4_CCER        (*(volatile uint32_t *) (TIM4_BASE + 0x20)) //TIM capture/compare enable register,      Address offset: 0x20 */
#define TIM4_CNT         (*(volatile uint32_t *) (TIM4_BASE + 0x24)) //TIM counter register,                     Address offset: 0x24
#define TIM4_PSC         (*(volatile uint32_t *) (TIM4_BASE + 0x28)) //TIM prescaler,                            Address offset: 0x28
#define TIM4_ARR         (*(volatile uint32_t *) (TIM4_BASE + 0x2C)) //TIM auto-reload register,
#define TIM4_CCR1        (*(volatile uint32_t *) (TIM4_BASE + 0x34)) //TIM capture/compare register 1,           Address offset: 0x34
#define TIM4_BDTR        (*(volatile uint32_t *) (TIM4_BASE + 0x44)) //TIM break and dead-time register,

/********************** EXTI  **********************************/
#define APB2PERIPH_BASE    (PERIPH_BASE + 0x00010000UL)
#define EXTI               (APB2PERIPH_BASE + 0x0400UL)
#define EXTI_IMR1          (*(volatile uint32_t *) (EXTI + 0x0)) // EXTI Interrupt mask register 1,             Address offset: 0x00
#define EXTI_FTSR1         (*(volatile uint32_t *) (EXTI + 0xC)) // EXTI Falling trigger selection register 1,  Address offset: 0x0C
#define EXTI_PR1           (*(volatile uint32_t *) (EXTI + 0x14)) // EXTI Pending register 1,                    Address offset: 0x14

#define EXTI_PR1_PIF5_Pos        (5U)
#define EXTI_PR1_PIF5            0x00000020                             /*!< Pending bit for line 5 */

/********************** DAC1 **********************************/
#define DAC1_BASE             0x50000800

#define DAC1_CR          (*(volatile uint32_t *) (DAC1_BASE + 0x0)) /* DAC control register,                                    Address offset: 0x00 */
#define DAC1_DHR12R1     (*(volatile uint32_t *) (DAC1_BASE + 0x8)) /* DAC channel1 12-bit right-aligned data holding register, Address offset: 0x08 */

/********************** ADC1 **********************************/
#define AHB2PERIPH_BASE       (0x48000000UL)
#define ADC1_BASE             (AHB2PERIPH_BASE + 0x08000000UL)
#define ADC12_COMMON_BASE     (AHB2PERIPH_BASE + 0x08000300UL)

#define ADC1_ISR           (*(volatile uint32_t *) (ADC1_BASE + 0x0)) /* ADC interrupt and status register, Address offset: 0x00 */
#define ADC1_IER           (*(volatile uint32_t *) (ADC1_BASE + 0x4)) /* ADC interrupt enable register,                 Address offset: 0x04 */
#define ADC1_CR            (*(volatile uint32_t *) (ADC1_BASE + 0x8)) /* ADC control register,  Address offset: 0x08 */
#define ADC1_CFGR          (*(volatile uint32_t *) (ADC1_BASE + 0xC)) /* ADC configuration register 1, Address offset: 0x0C */
#define ADC1_SQR1          (*(volatile uint32_t *) (ADC1_BASE + 0x30)) /* ADC group regular sequencer register 1, Address offset: 0x30 */
#define ADC1_DR            (*(volatile uint32_t *) (ADC1_BASE + 0x40)) /* ADC group regular data register, Address offset: 0x40 */
#define ADC1_DIFSEL        (*(volatile uint32_t *) (ADC1_BASE + 0xB0)) /* ADC differential mode selection register,      Address offset: 0xB0 */

#define ADC12_COMMON_CCR   (*(volatile uint32_t *) (ADC12_COMMON_BASE + 0x8)) // ADC common configuration register,     Address offset:  0x08 */

#define ADC_ISR_EOC_Pos                (2U)
#define ADC_ISR_EOC                    0x00000004                         /* ADC group regular end of unitary conversion flag */

// Bit definition for ADC_CFGR register
#define ADC_CFGR_DMAEN_Pos             (0U)
#define ADC_CFGR_DMAEN                 0x00000001                  /*!< ADC DMA transfer enable */
#define ADC_CFGR_DMACFG_Pos            (1U)
#define ADC_CFGR_DMACFG                00000002                    /*!< ADC DMA transfer configuration */

/********************** System control  **********************************/
#define SCS_BASE            (0xE000E000UL)                            /*!< System Control Space Base Address */
#define SCB_BASE            (SCS_BASE +  0x0D00UL)                    /*!< System Control Block Base Address */
#define SCB                  SCB_BASE    /*!< SCB configuration struct */
#define SCB_CPACR           (*(volatile uint32_t *) (SCB_BASE + 0x88))       /*!< Offset: 0x088 (R/W)  Coprocessor Access Control Register */

/********************** I2C2  **********************************/
#define APB1PERIPH_BASE        PERIPH_BASE
#define I2C2_BASE             (APB1PERIPH_BASE + 0x5800UL)

#define I2C2_CR1         (*(volatile uint32_t *) (I2C2_BASE + 0x0))  /*I2C Control register 1,            Address offset: 0x00 */
#define I2C2_CR2         (*(volatile uint32_t *) (I2C2_BASE + 0x4))  /*I2C Control register 2,            Address offset: 0x04 */
#define I2C2_OAR1        (*(volatile uint32_t *) (I2C2_BASE + 0x8))  /*I2C Own address 1 register,        Address offset: 0x08 */
#define I2C2_OAR2        (*(volatile uint32_t *) (I2C2_BASE + 0xC))  /*I2C Own address 2 register,        Address offset: 0x0C */
#define I2C2_TIMINGR     (*(volatile uint32_t *) (I2C2_BASE + 0x10)) /*I2C Timing register,               Address offset: 0x10 */
#define I2C2_TIMEOUTR    (*(volatile uint32_t *) (I2C2_BASE + 0x14)) /*I2C Timeout register,              Address offset: 0x14 */
#define I2C2_ISR         (*(volatile uint32_t *) (I2C2_BASE + 0x18)) /*I2C Interrupt and status register, Address offset: 0x18 */
#define I2C2_ICR         (*(volatile uint32_t *) (I2C2_BASE + 0x1C)) /*I2C Interrupt clear register,      Address offset: 0x1C */
#define I2C2_PECR        (*(volatile uint32_t *) (I2C2_BASE + 0x20)) /*I2C PEC register,                  Address offset: 0x20 */
#define I2C2_RXDR        (*(volatile uint32_t *) (I2C2_BASE + 0x24)) /*I2C Receive data register,         Address offset: 0x24 */
#define I2C2_TXDR        (*(volatile uint32_t *) (I2C2_BASE + 0x28)) /*I2C Transmit data register,        Address offset: 0x28 */

//  Bit definition for I2C_CR1 register
#define I2C_CR1_PE_Pos               (0U)
#define I2C_CR1_PE_Msk               (0x1UL << I2C_CR1_PE_Pos)                 /*!< 0x00000001 */
#define I2C_CR1_PE                   I2C_CR1_PE_Msk                            /*!< Peripheral enable                   */
#define I2C_CR1_DNF_Pos              (8U)
#define I2C_CR1_DNF_Msk              (0xFUL << I2C_CR1_DNF_Pos)                /*!< 0x00000F00 */
#define I2C_CR1_DNF                  I2C_CR1_DNF_Msk                           /*!< Digital noise filter                */
#define I2C_CR1_ANFOFF_Pos           (12U)
#define I2C_CR1_ANFOFF_Msk           (0x1UL << I2C_CR1_ANFOFF_Pos)             /*!< 0x00001000 */
#define I2C_CR1_ANFOFF               I2C_CR1_ANFOFF_Msk                        /*!< Analog noise filter OFF             */
#define I2C_CR1_TXDMAEN_Pos          (14U)
#define I2C_CR1_TXDMAEN_Msk          (0x1UL << I2C_CR1_TXDMAEN_Pos)            /*!< 0x00004000 */
#define I2C_CR1_TXDMAEN              I2C_CR1_TXDMAEN_Msk                       /*!< DMA transmission requests enable    */
#define I2C_CR1_NOSTRETCH_Pos        (17U)
#define I2C_CR1_NOSTRETCH_Msk        (0x1UL << I2C_CR1_NOSTRETCH_Pos)          /*!< 0x00020000 */
#define I2C_CR1_NOSTRETCH            I2C_CR1_NOSTRETCH_Msk                     /*!< Clock stretching disable            */
#define I2C_CR1_GCEN_Pos             (19U)
#define I2C_CR1_GCEN_Msk             (0x1UL << I2C_CR1_GCEN_Pos)               /*!< 0x00080000 */
#define I2C_CR1_GCEN                 I2C_CR1_GCEN_Msk                          /*!< General call enable                 */
#define I2C_CR1_SMBHEN_Pos           (20U)
#define I2C_CR1_SMBHEN_Msk           (0x1UL << I2C_CR1_SMBHEN_Pos)             /*!< 0x00100000 */
#define I2C_CR1_SMBHEN               I2C_CR1_SMBHEN_Msk                        /*!< SMBus host address enable           */
#define I2C_CR1_SMBDEN_Pos           (21U)
#define I2C_CR1_SMBDEN_Msk           (0x1UL << I2C_CR1_SMBDEN_Pos)             /*!< 0x00200000 */
#define I2C_CR1_SMBDEN               I2C_CR1_SMBDEN_Msk                        /*!< SMBus device default address enable */

// Bit definition for I2C_OAR1 register

#define I2C_OAR1_OA1_Pos             (0U)
#define I2C_OAR1_OA1_Msk             (0x3FFUL << I2C_OAR1_OA1_Pos)             /*!< 0x000003FF */
#define I2C_OAR1_OA1                 I2C_OAR1_OA1_Msk                          /*!< Interface own address 1   */
#define I2C_OAR1_OA1MODE_Pos         (10U)
#define I2C_OAR1_OA1MODE_Msk         (0x1UL << I2C_OAR1_OA1MODE_Pos)           /*!< 0x00000400 */
#define I2C_OAR1_OA1MODE             I2C_OAR1_OA1MODE_Msk                      /*!< Own address 1 10-bit mode */
#define I2C_OAR1_OA1EN_Pos           (15U)
#define I2C_OAR1_OA1EN_Msk           (0x1UL << I2C_OAR1_OA1EN_Pos)             /*!< 0x00008000 */
#define I2C_OAR1_OA1EN               I2C_OAR1_OA1EN_Msk                        /*!< Own address 1 enable      */

//  Bit definition for I2C_CR2 register

#define I2C_CR2_SADD_Pos             (0U)
#define I2C_CR2_SADD_Msk             (0x3FFUL << I2C_CR2_SADD_Pos)             /*!< 0x000003FF */
#define I2C_CR2_SADD                 I2C_CR2_SADD_Msk                          /*!< Slave address (master mode)                             */
#define I2C_CR2_RD_WRN_Pos           (10U)
#define I2C_CR2_RD_WRN_Msk           (0x1UL << I2C_CR2_RD_WRN_Pos)             /*!< 0x00000400 */
#define I2C_CR2_RD_WRN               I2C_CR2_RD_WRN_Msk                        /*!< Transfer direction (master mode)                        */
#define I2C_CR2_ADD10_Pos            (11U)
#define I2C_CR2_ADD10_Msk            (0x1UL << I2C_CR2_ADD10_Pos)              /*!< 0x00000800 */
#define I2C_CR2_ADD10                I2C_CR2_ADD10_Msk                         /*!< 10-bit addressing mode (master mode)                    */
#define I2C_CR2_HEAD10R_Pos          (12U)
#define I2C_CR2_HEAD10R_Msk          (0x1UL << I2C_CR2_HEAD10R_Pos)            /*!< 0x00001000 */
#define I2C_CR2_HEAD10R              I2C_CR2_HEAD10R_Msk                       /*!< 10-bit address header only read direction (master mode) */
#define I2C_CR2_START_Pos            (13U)
#define I2C_CR2_START_Msk            (0x1UL << I2C_CR2_START_Pos)              /*!< 0x00002000 */
#define I2C_CR2_START                I2C_CR2_START_Msk                         /*!< START generation                                        */
#define I2C_CR2_STOP_Pos             (14U)
#define I2C_CR2_STOP_Msk             (0x1UL << I2C_CR2_STOP_Pos)               /*!< 0x00004000 */
#define I2C_CR2_STOP                 I2C_CR2_STOP_Msk                          /*!< STOP generation (master mode)                           */
#define I2C_CR2_NACK_Pos             (15U)
#define I2C_CR2_NACK_Msk             (0x1UL << I2C_CR2_NACK_Pos)               /*!< 0x00008000 */
#define I2C_CR2_NACK                 I2C_CR2_NACK_Msk                          /*!< NACK generation (slave mode)                            */
#define I2C_CR2_NBYTES_Pos           (16U)
#define I2C_CR2_NBYTES_Msk           (0xFFUL << I2C_CR2_NBYTES_Pos)            /*!< 0x00FF0000 */
#define I2C_CR2_NBYTES               I2C_CR2_NBYTES_Msk                        /*!< Number of bytes                                         */
#define I2C_CR2_RELOAD_Pos           (24U)
#define I2C_CR2_RELOAD_Msk           (0x1UL << I2C_CR2_RELOAD_Pos)             /*!< 0x01000000 */
#define I2C_CR2_RELOAD               I2C_CR2_RELOAD_Msk                        /*!< NBYTES reload mode                                      */
#define I2C_CR2_AUTOEND_Pos          (25U)
#define I2C_CR2_AUTOEND_Msk          (0x1UL << I2C_CR2_AUTOEND_Pos)            /*!< 0x02000000 */
#define I2C_CR2_AUTOEND              I2C_CR2_AUTOEND_Msk                       /*!< Automatic end mode (master mode)                        */
#define I2C_CR2_PECBYTE_Pos          (26U)
#define I2C_CR2_PECBYTE_Msk          (0x1UL << I2C_CR2_PECBYTE_Pos)            /*!< 0x04000000 */
#define I2C_CR2_PECBYTE              I2C_CR2_PECBYTE_Msk                       /*!< Packet error checking byte                              */

// Bit definition for I2C_OAR2 register

#define I2C_OAR2_OA2_Pos             (1U)
#define I2C_OAR2_OA2_Msk             (0x7FUL << I2C_OAR2_OA2_Pos)              /*!< 0x000000FE */
#define I2C_OAR2_OA2                 I2C_OAR2_OA2_Msk                          /*!< Interface own address 2                        */
#define I2C_OAR2_OA2MSK_Pos          (8U)
#define I2C_OAR2_OA2MSK_Msk          (0x7UL << I2C_OAR2_OA2MSK_Pos)            /*!< 0x00000700 */
#define I2C_OAR2_OA2MSK              I2C_OAR2_OA2MSK_Msk                       /*!< Own address 2 masks                            */
#define I2C_OAR2_OA2EN_Pos           (15U)
#define I2C_OAR2_OA2EN_Msk           (0x1UL << I2C_OAR2_OA2EN_Pos)             /*!< 0x00008000 */
#define I2C_OAR2_OA2EN               I2C_OAR2_OA2EN_Msk                        /*!< Own address 2 enable                           */

// Bit definition for I2C_ISR register
#define I2C_ISR_TXIS_Pos             (1U)
#define I2C_ISR_TXIS_Msk             (0x1UL << I2C_ISR_TXIS_Pos)               /*!< 0x00000002 */
#define I2C_ISR_TXIS                 I2C_ISR_TXIS_Msk                          /*!< Transmit interrupt status       */
#define I2C_ISR_STOPF_Pos            (5U)
#define I2C_ISR_STOPF_Msk            (0x1UL << I2C_ISR_STOPF_Pos)              /*!< 0x00000020 */
#define I2C_ISR_STOPF                I2C_ISR_STOPF_Msk                         /*!< STOP detection flag             */

//  Bit definition for I2C_ICR register
#define I2C_ICR_STOPCF_Pos           (5U)
#define I2C_ICR_STOPCF_Msk           (0x1UL << I2C_ICR_STOPCF_Pos)             /*!< 0x00000020 */
#define I2C_ICR_STOPCF               I2C_ICR_STOPCF_Msk                        /*!< STOP detection clear flag   */

/********************** DMA1 **********************************/
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)
#define DMA1_BASE             (AHB1PERIPH_BASE)


#define DMA1_ISR         	  (*(volatile uint32_t *) (DMA1_BASE + 0x0)) // DMA interrupt status register,                 Address offset: 0x00
#define DMA1_IFCR        	  (*(volatile uint32_t *) (DMA1_BASE + 0x4)) // DMA interrupt flag clear register,             Address offset: 0x04

#define DMA1_Channel3_BASE    (DMA1_BASE + 0x0030UL)
#define DMA1_Channel3_CCR     (*(volatile uint32_t *) (DMA1_Channel3_BASE)) // DMA channel x configuration register
#define DMA1_Channel3_CNDTR   (*(volatile uint32_t *) (DMA1_Channel3_BASE + 0x4))  /*!< DMA channel x number of data register       */
#define DMA1_Channel3_CPAR    (*(volatile uint32_t *) (DMA1_Channel3_BASE + 0x8))  /*!< DMA channel x peripheral address register   */
#define DMA1_Channel3_CMAR    (*(volatile uint32_t *) (DMA1_Channel3_BASE + 0xC))  /*!< DMA channel x memory address register       */

#define DMA1_Channel4_BASE    (DMA1_BASE + 0x0044UL)
#define DMA1_Channel4_CCR     (*(volatile uint32_t *) (DMA1_Channel4_BASE)) // DMA channel x configuration register
#define DMA1_Channel4_CNDTR   (*(volatile uint32_t *) (DMA1_Channel4_BASE + 0x4))  /*!< DMA channel x number of data register       */
#define DMA1_Channel4_CPAR    (*(volatile uint32_t *) (DMA1_Channel4_BASE + 0x8))  /*!< DMA channel x peripheral address register   */
#define DMA1_Channel4_CMAR    (*(volatile uint32_t *) (DMA1_Channel4_BASE + 0xC))  /*!< DMA channel x memory address register       */

// Bit definition for DMA_ISR register
#define DMA_ISR_TCIF3_Pos      (9U)
#define DMA_ISR_TCIF3          0x00000200                               /*!< Channel 3 Transfer Complete flag */
#define DMA_ISR_TCIF4_Pos      (13U)
#define DMA_ISR_TCIF4          0x00002000  /*!< Channel 4 Transfer Complete flag */
#define DMA_ISR_HTIF4_Pos      (14U)
#define DMA_ISR_HTIF4          0x00004000  /*!< Channel 4 Half Transfer flag */

// Bit definition for DMA_IFCR register
#define DMA_IFCR_CTCIF3_Pos    (9U)
#define DMA_IFCR_CTCIF3        0x00000200                             /*!< Channel 3 Transfer Complete clear */
#define DMA_IFCR_CTCIF4_Pos    (13U)
#define DMA_IFCR_CTCIF4        0x00002000                             /*!< Channel 4 Transfer Complete clear */
#define DMA_IFCR_CHTIF4_Pos    (14U)
#define DMA_IFCR_CHTIF4        0x00004000                             /*!< Channel 4 Half Transfer clear */

// Bit definition for DMA_CNDTR register
#define DMA_CNDTR_NDT_Pos      (0U)
#define DMA_CNDTR_NDT          0x0000FFFF                             /*!< Number of data to Transfer          */

// Bit definition for DMA_CCR register
#define DMA_CCR_EN_Pos         (0U)
#define DMA_CCR_EN_Msk         (0x1UL << DMA_CCR_EN_Pos)                       /*!< 0x00000001 */
#define DMA_CCR_EN             DMA_CCR_EN_Msk                                  /*!< Channel enable                      */
#define DMA_CCR_TCIE_Pos       (1U)
#define DMA_CCR_TCIE_Msk       (0x1UL << DMA_CCR_TCIE_Pos)                     /*!< 0x00000002 */
#define DMA_CCR_TCIE           DMA_CCR_TCIE_Msk                                /*!< Transfer complete interrupt enable  */
#define DMA_CCR_HTIE_Pos       (2U)
#define DMA_CCR_HTIE_Msk       (0x1UL << DMA_CCR_HTIE_Pos)                     /*!< 0x00000004 */
#define DMA_CCR_HTIE           DMA_CCR_HTIE_Msk                                /*!< Half Transfer interrupt enable      */
#define DMA_CCR_TEIE_Pos       (3U)
#define DMA_CCR_TEIE_Msk       (0x1UL << DMA_CCR_TEIE_Pos)                     /*!< 0x00000008 */
#define DMA_CCR_TEIE           DMA_CCR_TEIE_Msk                                /*!< Transfer error interrupt enable     */
#define DMA_CCR_DIR_Pos        (4U)
#define DMA_CCR_DIR_Msk        (0x1UL << DMA_CCR_DIR_Pos)                      /*!< 0x00000010 */
#define DMA_CCR_DIR            DMA_CCR_DIR_Msk                                 /*!< Data transfer direction             */
#define DMA_CCR_CIRC_Pos       (5U)
#define DMA_CCR_CIRC_Msk       (0x1UL << DMA_CCR_CIRC_Pos)                     /*!< 0x00000020 */
#define DMA_CCR_CIRC           DMA_CCR_CIRC_Msk                                /*!< Circular mode                       */
#define DMA_CCR_PINC_Pos       (6U)
#define DMA_CCR_PINC_Msk       (0x1UL << DMA_CCR_PINC_Pos)                     /*!< 0x00000040 */
#define DMA_CCR_PINC           DMA_CCR_PINC_Msk                                /*!< Peripheral increment mode           */
#define DMA_CCR_MINC_Pos       (7U)
#define DMA_CCR_MINC_Msk       (0x1UL << DMA_CCR_MINC_Pos)                     /*!< 0x00000080 */
#define DMA_CCR_MINC           DMA_CCR_MINC_Msk                                /*!< Memory increment mode               */

#define DMA_CCR_PSIZE_Pos      (8U)
#define DMA_CCR_PSIZE_Msk      (0x3UL << DMA_CCR_PSIZE_Pos)                    /*!< 0x00000300 */
#define DMA_CCR_PSIZE          DMA_CCR_PSIZE_Msk                               /*!< PSIZE[1:0] bits (Peripheral size)   */
#define DMA_CCR_PSIZE_0        (0x1UL << DMA_CCR_PSIZE_Pos)                    /*!< 0x00000100 */
#define DMA_CCR_PSIZE_1        (0x2UL << DMA_CCR_PSIZE_Pos)                    /*!< 0x00000200 */

#define DMA_CCR_MSIZE_Pos      (10U)
#define DMA_CCR_MSIZE_Msk      (0x3UL << DMA_CCR_MSIZE_Pos)                    /*!< 0x00000C00 */
#define DMA_CCR_MSIZE          DMA_CCR_MSIZE_Msk                               /*!< MSIZE[1:0] bits (Memory size)       */
#define DMA_CCR_MSIZE_0        (0x1UL << DMA_CCR_MSIZE_Pos)                    /*!< 0x00000400 */
#define DMA_CCR_MSIZE_1        (0x2UL << DMA_CCR_MSIZE_Pos)                    /*!< 0x00000800 */

#define DMA_CCR_PL_Pos         (12U)
#define DMA_CCR_PL_Msk         (0x3UL << DMA_CCR_PL_Pos)                       /*!< 0x00003000 */
#define DMA_CCR_PL             DMA_CCR_PL_Msk                                  /*!< PL[1:0] bits(Channel Priority level)*/
#define DMA_CCR_PL_0           (0x1UL << DMA_CCR_PL_Pos)                       /*!< 0x00001000 */
#define DMA_CCR_PL_1           (0x2UL << DMA_CCR_PL_Pos)                       /*!< 0x00002000 */

#define DMA_CCR_MEM2MEM_Pos    (14U)
#define DMA_CCR_MEM2MEM_Msk    (0x1UL << DMA_CCR_MEM2MEM_Pos)                  /*!< 0x00004000 */
#define DMA_CCR_MEM2MEM        DMA_CCR_MEM2MEM_Msk                             /*!< Memory to memory mode               */

/********************** DMAMUX1 ********************************/
#define DMAMUX1_BASE          	 (AHB1PERIPH_BASE + 0x0800UL)

#define DMAMUX1_Channel2_BASE    (DMAMUX1_BASE + 0x0008UL)
#define DMAMUX1_Channel2    	 ((DMAMUX_Channel_TypeDef *) DMAMUX1_Channel2_BASE)
#define DMAMUX1_Channel2_CCR     (*(volatile uint32_t *) (DMAMUX1_Channel2_BASE)) // DMA Multiplexer Channel 2 Control Register


#define DMAMUX1_Channel3_BASE    (DMAMUX1_BASE + 0x000CUL)
#define DMAMUX1_Channel3    	 (DMAMUX1_Channel3_BASE)
#define DMAMUX1_Channel3_CCR     (*(volatile uint32_t *) (DMAMUX1_Channel3_BASE))// DMA Multiplexer Channel 3 Control Register


// Bits definition for DMAMUX_CxCR register

#define DMAMUX_CxCR_DMAREQ_ID_Pos  (0U)
#define DMAMUX_CxCR_DMAREQ_ID      0x000000FF
#define DMAMUX_REQ_I2C2_TX         0x00000013U  /*!< DMAMUX I2C2 TX request     */
#define DMAMUX_REQ_ADC1            0x00000005U  /*!< DMAMUX ADC1 request        */

#endif /* __STM32G431_H */

