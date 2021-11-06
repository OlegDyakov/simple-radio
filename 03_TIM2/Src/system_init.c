#include <stm32g431.h>

/* Константы определены в файле stm32f4.ld  */
extern uint32_t _estack;
extern uint32_t _sdata_ram, _edata_ram;
extern uint32_t _sbss, _ebss;
extern uint32_t _svector, _evector;
extern uint32_t _sdata_flash, _edata_flash;

int main(void);
void SystemClock_Config(void);


/* Точка входа  */
void start_up(void)
{
	uint32_t *src, *dst;

	/* Копирование данных секции .data из FLASH в RAM */
	src = &_sdata_flash;
	dst = &_sdata_ram;
	while (dst < &_edata_ram) {
		*dst++ = *src++;
	}

	/* Запись нулей в область памяти секции .bss */
	dst = &_sbss;
	while (dst < &_ebss) {
		*dst++ = 0;
	}

	SystemClock_Config();
	main();
}




void SystemClock_Config(void)
{

  //Включить режим повышения выходного напряжения  главного регулятора до 1,28 вольт
  CLEAR_BIT(PWR_CR5, PWR_CR5_R1MODE);

  //Установка задержки чтения FLASH памяти до 4-х тактов
  FLASH_ACR=FLASH_ACR_DBG_SWEN  | FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_LATENCY_4WS ;
  while((FLASH_ACR & 0xf) != FLASH_ACR_LATENCY_4WS );

  //Подключение внешнего кварцевого резонатора (HSE ON)
  SET_BIT(RCC_CR, RCC_CR_HSEON);
  while(READ_BIT(RCC_CR, RCC_CR_HSERDY) != (RCC_CR_HSERDY));

  //Настройка PLL, используемого для SYSCLK домена
  //PLLM=6 (значение поля 5), PLLN=85 (значение поля 85), PLLR=2 (значение поля 0)
  MODIFY_REG(RCC_PLLCFGR, RCC_PLLCFGR_PLLSRC     | RCC_PLLCFGR_PLLM         | RCC_PLLCFGR_PLLN             | RCC_PLLCFGR_PLLR,
		                  RCC_PLLCFGR_PLLSRC_HSE | 5 <<RCC_PLLCFGR_PLLM_Pos | (85 << RCC_PLLCFGR_PLLN_Pos) | 0<<RCC_PLLCFGR_PLLR_Pos);

  //Включение PLL
  SET_BIT(RCC_CR, RCC_CR_PLLON);
  while(READ_BIT(RCC_CR, RCC_CR_PLLRDY) != (RCC_CR_PLLRDY));

  //Включение выхода PLL, используемого для  SYSCLK
  SET_BIT(RCC_PLLCFGR, RCC_PLLCFGR_PLLREN);

  //Подключение выхода PLL в качестве источника  SYSCLK
  MODIFY_REG(RCC_CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);


}
