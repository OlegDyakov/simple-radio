#include <stm32g431.h>

/* Константы определены в файле stm32f4.ld  */
extern uint32_t _estack;
extern uint32_t _sdata_ram, _edata_ram;
extern uint32_t _sbss, _ebss;
extern uint32_t _svector, _evector;
extern uint32_t _sdata_flash, _edata_flash;

int main(void);

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

	main();
}


