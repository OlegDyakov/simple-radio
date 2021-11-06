/* Точка входа - функция sturt_up */
ENTRY(start_up)

/* 
   Начальные адреса и размер RAM и FLASH
   FLASH также аппаратно отображается в адреса 0x00000000 - 0x00020000
*/

MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000,  LENGTH = 32K
  FLASH  (rx)     : ORIGIN = 0x8000000,   LENGTH = 128K 
}

/* Размер таблицы векторов прерывания */
_size_vector_table = 0x0400;

/* 
   Вершина стека 
   Находится в конце RAM 
*/
_estack = ORIGIN(RAM) + LENGTH(RAM);	


SECTIONS {

/* Секция векторов прерывания */
	.vectors : {
		. = ALIGN(4);
		_svector = .;
	/*ключевое слово KEEP сохраняет раздел даже если на него нет ссылок*/
		KEEP(*(.vectors)); 							
		. = _svector + _size_vector_table;
		. = ALIGN(4);
		_evector = .;
	} > FLASH

/* Секция кода программы и констант*/
	.text : {
		*(.text*)
		*(.rodata*)
	} > FLASH

/* Секция глобальных переменных, инициализированных нулем */
	.bss : {
		_sbss = .;
		*(.bss*)
   		*(COMMON)
		. = ALIGN(4);
		_ebss = .;
	} > RAM

/* Секция глобальных переменных, инициализированных константами  */
	.data : {
		_sdata_ram = .;
		*(.data*)
		. = ALIGN(4);
		_edata_ram = .;
	} > RAM AT > FLASH

}

/* Начальный и конечный адрес секции .data во flash памяти */
_sdata_flash = LOADADDR(.data);
_edata_flash = _sdata_flash + SIZEOF(.data);


