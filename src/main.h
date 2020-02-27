#ifndef MAIN_H_
#define MAIN_H_

//F_CPU должно быть задано в свойствах проекта: Toolchain -> AVR/GNU C Compiler -> Simbols -DF_CPU=8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "buttons.h"	//подключение драйвера кнопок
#include "SEG_IND.h"	//подключение драйвера семисегментного индикатора

#endif /* MAIN_H_ */