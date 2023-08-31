#ifndef MAX7219_LEDMatrix_H_
#define MAX7219_LEDMatrix_H_

//Defines for SPI communication
#define DIN  PORTB2
#define SCK  PORTB1
#define CS   PORTB0
//Addresses for MAX7219
#define DECODE	   0x09
#define INTENSITY  0x0A
#define SCAN_LIMIT 0x0B
#define NORMAL_OP  0x0C
#define TEST	   0x0F

//Defines for MAX instruction
#define LOADSTOP	   PORTB |= (1<<CS)
#define LOADSTART	   PORTB &= (1<<CS)
#define CLKHIGH		   PORTB |= (1<<SCK)
#define CLKLOW		   PORTB &= (1<<SCK)

extern void SPIMasterInit(void);
extern void sendData(unsigned char);

extern void initLEDMatrix(void);
extern void clearDisplay(void);
extern void writeMax(uint8_t, uint8_t);

#include <util/delay.h>

#endif