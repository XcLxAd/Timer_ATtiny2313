
#ifndef SEG_IND_H_
#define SEG_IND_H_

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdbool.h>

// ���� ����������� ��������� ����������
#define IND_PORT_SEG PORTB

// ���� ����������� �������� ����������
#define IND_PORT_DIG PORTD

// �������� ������� �������� 0 ��� 1
#define IND_ACTIVE_LEVEL_SEG   0

//�������� ������� ������� 0 ��� 1
#define IND_ACTIVE_LEVEL_DIG   1

// � ����� ������� ����� ���������� �������� ����������
#define IND_A	0
#define IND_B	1
#define IND_C	2
#define IND_D	3
#define IND_E	4
#define IND_F	5
#define IND_G	6
#define IND_DP	7

//� ����� ������� ����� ���������� ������� ����������
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




void SEG_IND_init(void); // ������������� ����������
void SEG_IND_clearAll(void);	// ����������� ���� �������� � ���������
void SEG_IND_sendSegment(uint8_t segData, uint8_t digit);	// ����� �� ��������� ���� 7-�� ����������� ���������� segData � ������ digit
void SEG_IND_encode_2_7seg (void);	// �������������� ������ � ��� 7-�� ����������� ����������
void SEG_IND_update(void);	//	���������� ������� �� ������, ����������� � ����������� ���������� �� ���������� �������
void SEG_IND_displayClock(uint8_t minute, uint8_t second); //������� ���� � ������
void SEG_IND_setBrightness(uint8_t brightness); // ��������� ������� �� 0 �� 10

#endif