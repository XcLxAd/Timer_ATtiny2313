#ifndef initreg_h
#define initreg_h

#include <avr/io.h>

// макросы для доступа к регистрам порта (макрос принимает в качестве параметра адрес регистра PORTx)
// PORTx, регистр управляет значениями пина HIGH/LOW - (PORT_REG(&PORTx))
#define PORT_REG(port) (*(port))
// DDRx, регистр управляет режимами пина INPUT/OUTPUT - (DIR_REG(&PORTx))	
#define DIR_REG(port) (*((port) - 1))
// PINx, считывает значения с пинов, режим которых установлен как INPUT - (PIN_REG(&PORTx
#define PIN_REG(port) (*((port) - 2))))
// конвертирует номер бита в байтовую маску
#define _BV(bit) (1 << (bit))
// установить бит в LOW
#define LOW_BIT(bit) &= ~(1 << bit)
// установить бит в HIGH
#define HIGH_BIT(bit) |= (1 << bit)
// инвертировать бит
#define INV_BIT(bit) ^= (1 << bit)
// прочитать значение определенного бита в указанной переменной
#define BIT_READ(value, bit) (((value) >> (bit)) & 0x01)
#define BIT_SET(value, bit) ((value) |= (1UL << (bit)))//установить бит в 1 - например BIT_SET (a,6);
#define BIT_CLEAR(value, bit) ((value) &= ~(1UL << (bit)))//установить бит в 0
#define BIT_WRITE(value, bit, bitvalue) (bitvalue ? BIT_SET(value, bit) : BIT_CLEAR(value, bit))//позволяет записать определенный бит
#define swap_nibble(a) { asm("swap %0": "=r" (a) : "r" (a)); }


#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1

#endif