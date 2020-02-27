#include <avr/io.h>
#include <avr/interrupt.h>
#include "initreg.h"
#include "SEG_IND.h"

PROGMEM const uint8_t setDigits[] = {
	IND_DIGIT1,
	IND_DIGIT2,
	IND_DIGIT3,
	IND_DIGIT4
};

PROGMEM const uint8_t setSegments[] = {
	IND_A,
	IND_B,
	IND_C,
	IND_D,
	IND_E,
	IND_F,
	IND_G,
	IND_DP
};

PROGMEM const uint8_t symbol [] = {
	((1<<IND_A)|(1<<IND_B)|(1<<IND_C)|(1<<IND_D)|(1<<IND_E)|(1<<IND_F)),			// 0
	((1<<IND_B)|(1<<IND_C)),														// 1
	((1<<IND_A)|(1<<IND_B)|(1<<IND_D)|(1<<IND_E)|(1<<IND_G)),						// 2
	((1<<IND_A)|(1<<IND_B)|(1<<IND_C)|(1<<IND_D)|(1<<IND_G)),						// 3
	((1<<IND_B)|(1<<IND_C)|(1<<IND_F)|(1<<IND_G)),									// 4
	((1<<IND_A)|(1<<IND_C)|(1<<IND_D)|(1<<IND_F)|(1<<IND_G)),						// 5
	((1<<IND_A)|(1<<IND_C)|(1<<IND_D)|(1<<IND_E)|(1<<IND_F)|(1<<IND_G)),			// 6
	((1<<IND_A)|(1<<IND_B)|(1<<IND_C)),												// 7
	((1<<IND_A)|(1<<IND_B)|(1<<IND_C)|(1<<IND_D)|(1<<IND_E)|(1<<IND_F)|(1<<IND_G)),	// 8
	((1<<IND_A)|(1<<IND_B)|(1<<IND_C)|(1<<IND_D)|(1<<IND_F)|(1<<IND_G))				// 9
};


uint8_t latestData[4];	// ����� �������
uint8_t segmentData[4];	// ����� ������� ���������
volatile uint8_t count = 0;

// ������������� ����������
void SEG_IND_init(void) {
	// ��������� ������� ����� ��������� ��� OUTPUT
	DIR_REG(&IND_PORT_SEG) = (1<<IND_A)|(1<<IND_B)|(1<<IND_C)|(1<<IND_D)|(1<<IND_E)|(1<<IND_F)|(1<<IND_G)|(1<<IND_DP);
	// ��������� ������� ����� �������� ��� OUTPUT
	DIR_REG(&IND_PORT_DIG) = (1<<IND_DIGIT1)|(1<<IND_DIGIT2)|(1<<IND_DIGIT3)|(1<<IND_DIGIT4);
	SEG_IND_clearAll();
	TCCR0B &= ~((1<<CS02) | (1<<CS01) | (1<<CS00)); // ������� �������
	TCCR0B |= ((1<<WGM01) | (1<<WGM02));	// ����� FASTPWM
	TCCR0A |= ((1<<WGM01) | (1<<WGM02));	// ����� FASTPWM

	TCCR0B |= (1<<CS01);	// prescaler CLK/8

	TIMSK |= ((1<<TOIE0) | (1<<OCIE0B));	// ���������� ���������� �� ���������� � �� ������������  �������
	SEG_IND_setBrightness(10);
	sei();	// ���������� ����������
}
 
// ��������� ������� �� 0 �� 10
void SEG_IND_setBrightness(uint8_t brightness) {
	if (brightness > 10) return;
	OCR0B = 255 - brightness*25;
}
 
// ����������� ���� �������� � ���������
void SEG_IND_clearAll(void)  {
	
	/*
	for(uint8_t i = 0; i < 4; i++) {
		IND_ClearDig(IND_PORT_DIG, pgm_read_byte(&(setDigits[i])));
	}
	for(uint8_t i = 0; i < 8; i++) {
		IND_ClearSeg(IND_PORT_SEG, pgm_read_byte(&(setSegments[i])));
	}
	*/
	
	// ����� ������ ��� ������ ��������� ���������������� ��� �������� �� 60 ����
	IND_ClearDig(IND_PORT_DIG, IND_DIGIT1);	
	IND_ClearDig(IND_PORT_DIG, IND_DIGIT2);
	IND_ClearDig(IND_PORT_DIG, IND_DIGIT3);
	IND_ClearDig(IND_PORT_DIG, IND_DIGIT4);
	IND_ClearSeg(IND_PORT_SEG, IND_A);
	IND_ClearSeg(IND_PORT_SEG, IND_B);
	IND_ClearSeg(IND_PORT_SEG, IND_C);
	IND_ClearSeg(IND_PORT_SEG, IND_D);
	IND_ClearSeg(IND_PORT_SEG, IND_E);
	IND_ClearSeg(IND_PORT_SEG, IND_F);
	IND_ClearSeg(IND_PORT_SEG, IND_G);
	IND_ClearSeg(IND_PORT_SEG, IND_DP);
	
}

// ����� �� ��������� ���� 7-�� ����������� ���������� segData � ������ digit
void SEG_IND_sendSegment(uint8_t segData, uint8_t digit) {	
	if (digit > 3) return;
	IND_SetDig(IND_PORT_DIG, pgm_read_byte(&(setDigits[digit])));
	for (uint8_t i = 0; i < 8; i++) {
		if ((segData) & _BV(i)) {
			IND_SetSeg(IND_PORT_SEG, pgm_read_byte(&(setSegments[i])));
		}
		else {
			IND_ClearSeg(IND_PORT_SEG, pgm_read_byte(&(setSegments[i])));
		}
	}
}

// �������������� ������ � ��� 7-�� ����������� ����������
void SEG_IND_encode_2_7seg (void) {	
	bool noZero = false;
	for (uint8_t i = 0; i < 4; i++) {
		if ((latestData[i] == 0) && (noZero == false)) {
			segmentData[i] = 0;
		} 
		else {
		segmentData[i] = pgm_read_byte(&(symbol[latestData[i]]));
		noZero = true;
		}
	}
}

// ���������� ������� ���������� �� 4-� �������� (�������������� � ����������� ���������� �� ���������� ������� �2)
void SEG_IND_update(void) {
	SEG_IND_encode_2_7seg();
	if (count < 4) {
		SEG_IND_sendSegment(segmentData[count], count);
		count ++;
	}
	else {
		count = 0;
	}
}

//������� ���� � ������
void SEG_IND_displayClock(uint8_t minute, uint8_t second) {
	latestData[0] = minute / 10;
	latestData[1] = minute % 10;
	latestData[2] = second / 10;
	latestData[3] = second % 10; 
}


// ���������� ���������� �� ���������� �������2
ISR(TIMER0_COMPB_vect) {
	SEG_IND_update();
}	

// ���������� ���������� �� ������������ ������� 2
ISR (TIMER0_OVF_vect) {
	SEG_IND_clearAll();	
}
