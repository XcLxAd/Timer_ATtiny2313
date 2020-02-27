
#ifndef SEG_IND_H_
#define SEG_IND_H_

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdbool.h>

// порт подключени€ сегментов индикатора
#define IND_PORT_SEG PORTB

// порт подключени€ разр€дов индикатора
#define IND_PORT_DIG PORTD

// активный уровень сегмента 0 или 1
#define IND_ACTIVE_LEVEL_SEG   0

//активный уровень разр€да 0 или 1
#define IND_ACTIVE_LEVEL_DIG   1

// к каким выводам порта подключены сегменты индикатора
#define IND_A	0
#define IND_B	1
#define IND_C	2
#define IND_D	3
#define IND_E	4
#define IND_F	5
#define IND_G	6
#define IND_DP	7

//к каким выводам порта подключены разр€ды индикатора
#define IND_DIGIT1	3
#define IND_DIGIT2	4
#define IND_DIGIT3	5
#define IND_DIGIT4	6


#if (IND_ACTIVE_LEVEL_SEG == 0)
#define IND_SetSeg(port, value)		port &= ~_BV(value)
#define IND_ClearSeg(port, value)	port |= _BV(value)
#else
#define IND_SetSeg(port, value)		port |= _BV(value)
#define IND_ClearSeg(port, value)	port &= ~_BV(value)
#endif

#if (IND_ACTIVE_LEVEL_DIG == 0)
#define IND_SetDig(port, value)		port &= ~_BV(value)
#define IND_ClearDig(port, value)	port |= _BV(value)
#else
#define IND_SetDig(port, value)		port |= _BV(value)
#define IND_ClearDig(port, value)	port &= ~_BV(value)
#endif




void SEG_IND_init(void); // инициализаци€ индикатора
void SEG_IND_clearAll(void);	// деактиваци€ всех разр€дов и сегментов
void SEG_IND_sendSegment(uint8_t segData, uint8_t digit);	// вывод на индикатор кода 7-ми сегментного индикатора segData в разр€д digit
void SEG_IND_encode_2_7seg (void);	// преобразование данных в код 7-ми сегментного индикатора
void SEG_IND_update(void);	//	обновление диспле€ из буфера, выполн€етс€ в обработчике прерывани€ по совпадению таймера
void SEG_IND_displayClock(uint8_t minute, uint8_t second); //выводит часы и минуты
void SEG_IND_setBrightness(uint8_t brightness); // установка €ркости от 0 до 10

#endif