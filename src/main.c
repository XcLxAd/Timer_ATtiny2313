/*
 * myTimer.c
 *
 * Created: 29.10.2019 19:40:18
 * Author : XcLxAd
 */
#include "main.h"


volatile signed char second, minute, hour; // значение переменных может меняться извне

bool timer1Start = false;
bool timer1ButUp = false;
bool timer1ButDown = false;


uint8_t but = 0;	// буфер события нажатия кнопки
uint8_t code = 0;	// буфер события типа нажатия
uint8_t butSer = 0;	// счетчик для увеличения скорости инкремента/декремента после определенного порога


void TIMER1_Stop(void) {
	TCCR1B &= ~((1<<CS12)|(1<<CS11)|(1<<CS10));	// останов 1-го таймера
	TCNT1 = 0;		//сброс счетчика	
}

void TIMER1_Start (uint16_t div) {
	TIMER1_Stop ();
	OCR1A = div;	// запись делителя в регистр сравнения
	TCCR1B |= (1<<CS12);	// CLK/256
	TIMSK |= (1<<OCIE1A);	// установка бита разрешения прерывания 1-го таймера по совпадению с OCR1A(H и L)
	TCCR1B |= (1<<WGM12);	// сброс при совпадении
}

void time_increment (void) {
	if (minute < 60) {
		second++;
		if(second >= 60) {
			second = 0;
			minute++;
		}
	}
	else {
		minute = 60, second = 00;	
	}
}

void time_decrement (void) {
	if ((second > 0) || (minute > 0)) {
		second--;
		if (second < 0) {
			minute--;
			second = 59;
		}
	}
}




ISR (TIMER1_COMPA_vect) {	// обработчик прерывания от 1-го таймера по совпадению
	// обработка длительного нажатия клавиши Up
	if (timer1ButUp == true) {
		if (butSer < 30) {
			time_increment ();
			butSer++;
		}
		if (butSer >= 30) {
			second = second + 14;
			time_increment ();
			butSer++;
		}
		if (butSer >= 60) {
			second = second + 59;
			time_increment ();
			butSer++;
		}
	}

	// обработка длительного нажатия клавиши Down
	if (timer1ButDown == true) {
		if (butSer < 30) {
			time_decrement ();
			butSer++;
		}
		if (butSer >= 30) {
			second = second - 14; 
			time_decrement ();
			butSer++;
		}
	}
	
	// обработка старта/стопа таймера
	if (timer1Start == true) {
		time_decrement ();
	}
	
	// обработка события окончания счета		
	if ((second == 0) && (minute == 0 )) {
	TIMER1_Stop ();
	//PORTC |= (1<<2);
	}
	else {
		//PORTC &= ~(1<<2);
	}
	
	// индикация
	SEG_IND_displayClock(minute, second);
}



	
int main(void) {
	minute = 0;// начальная установка времени
	second = 0;
	SEG_IND_init(); // инициализация дисплея
	BUT_Init();	//вызов функции инициализации кнопок
	
	while(1) {
		_delay_ms(2);
		BUT_Poll();			//опрос кнопок
		but = BUT_GetBut();	//проверка буфера
		
		if (but) {	//если зафиксировано событие
			code = BUT_GetBut();	//берем код события
			if (timer1Start == false) {
				if (but == 1) {	// обработка нажатия кнопки старт
					TIMER1_Start (31249);	// 8MHz/256/(1+31249)=1Hz
					timer1Start = true;
				}
				if (but == 2) {	// обработка нажатий кнопки инкремента Up
					switch (code) {
						case 1:	// обработка короткого нажатия
							time_increment ();
							break;
						case 2:	// обработка длительного нажатия
							TIMER1_Start (6249);	//8MHz/256/(1+6249)=5Hz
							timer1ButUp = true;	
							break;
						case 4:	// обработка отпускания после длительного нажатия
							TIMER1_Stop ();
							timer1ButUp = false;
							butSer = 0;
							break;
					}
				}
				if (but == 3) {	// обработка нажатий кнопки декремента Down
					switch (code) {
						case 1:	// обработка короткого нажатия
							time_decrement ();
							break;
						case 2:	// обработка длительного нажатия
							TIMER1_Start (6249);
							timer1ButDown = true;
							break;
						case 4:	// обработка отпускания после длительного нажатия
							TIMER1_Stop ();
							timer1ButDown = false;
							butSer = 0;
							break;
					}
				}
			}
			else {
				if (but == 1) {	// обработка нажатия кнопки стоп
					TIMER1_Stop ();
					timer1Start = false;
				}
			}
		}
	// индикация
	SEG_IND_displayClock(minute, second);	
	}
	return 0;
}
