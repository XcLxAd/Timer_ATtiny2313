/*
 * myTimer.c
 *
 * Created: 29.10.2019 19:40:18
 * Author : XcLxAd
 */
#include "main.h"


volatile signed char second, minute, hour; // �������� ���������� ����� �������� �����

bool timer1Start = false;
bool timer1ButUp = false;
bool timer1ButDown = false;


uint8_t but = 0;	// ����� ������� ������� ������
uint8_t code = 0;	// ����� ������� ���� �������
uint8_t butSer = 0;	// ������� ��� ���������� �������� ����������/���������� ����� ������������� ������


void TIMER1_Stop(void) {
	TCCR1B &= ~((1<<CS12)|(1<<CS11)|(1<<CS10));	// ������� 1-�� �������
	TCNT1 = 0;		//����� ��������	
}

void TIMER1_Start (uint16_t div) {
	TIMER1_Stop ();
	OCR1A = div;	// ������ �������� � ������� ���������
	TCCR1B |= (1<<CS12);	// CLK/256
	TIMSK |= (1<<OCIE1A);	// ��������� ���� ���������� ���������� 1-�� ������� �� ���������� � OCR1A(H � L)
	TCCR1B |= (1<<WGM12);	// ����� ��� ����������
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




ISR (TIMER1_COMPA_vect) {	// ���������� ���������� �� 1-�� ������� �� ����������
	// ��������� ����������� ������� ������� Up
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

	// ��������� ����������� ������� ������� Down
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
	
	// ��������� ������/����� �������
	if (timer1Start == true) {
		time_decrement ();
	}
	
	// ��������� ������� ��������� �����		
	if ((second == 0) && (minute == 0 )) {
	TIMER1_Stop ();
	//PORTC |= (1<<2);
	}
	else {
		//PORTC &= ~(1<<2);
	}
	
	// ���������
	SEG_IND_displayClock(minute, second);
}



	
int main(void) {
	minute = 0;// ��������� ��������� �������
	second = 0;
	SEG_IND_init(); // ������������� �������
	BUT_Init();	//����� ������� ������������� ������
	
	while(1) {
		_delay_ms(2);
		BUT_Poll();			//����� ������
		but = BUT_GetBut();	//�������� ������
		
		if (but) {	//���� ������������� �������
			code = BUT_GetBut();	//����� ��� �������
			if (timer1Start == false) {
				if (but == 1) {	// ��������� ������� ������ �����
					TIMER1_Start (31249);	// 8MHz/256/(1+31249)=1Hz
					timer1Start = true;
				}
				if (but == 2) {	// ��������� ������� ������ ���������� Up
					switch (code) {
						case 1:	// ��������� ��������� �������
							time_increment ();
							break;
						case 2:	// ��������� ����������� �������
							TIMER1_Start (6249);	//8MHz/256/(1+6249)=5Hz
							timer1ButUp = true;	
							break;
						case 4:	// ��������� ���������� ����� ����������� �������
							TIMER1_Stop ();
							timer1ButUp = false;
							butSer = 0;
							break;
					}
				}
				if (but == 3) {	// ��������� ������� ������ ���������� Down
					switch (code) {
						case 1:	// ��������� ��������� �������
							time_decrement ();
							break;
						case 2:	// ��������� ����������� �������
							TIMER1_Start (6249);
							timer1ButDown = true;
							break;
						case 4:	// ��������� ���������� ����� ����������� �������
							TIMER1_Stop ();
							timer1ButDown = false;
							butSer = 0;
							break;
					}
				}
			}
			else {
				if (but == 1) {	// ��������� ������� ������ ����
					TIMER1_Stop ();
					timer1Start = false;
				}
			}
		}
	// ���������
	SEG_IND_displayClock(minute, second);	
	}
	return 0;
}
