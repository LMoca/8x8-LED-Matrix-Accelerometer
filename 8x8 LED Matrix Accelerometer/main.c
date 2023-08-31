/*
 *	Project Name:	8x8 LED Matrix Accelerometer.c
 *
 *	Date Created:	08/20/2023 1:23:00 PM
 *
 *	Author:			Luigi Moca
 *
 *	Description:	LED on the 8x8 LED Matrix moves with
 *	respect to the accelerometer's xyz position.
 *
 */ 
#define F_CPU 16000000UL  // 16Mhz
#include <avr/io.h>
#include <util/delay.h>

#include "MAX7219_LEDMatrix.h"
#include "I2C.h"

void initAccel(void);
uint8_t xDecode(int16_t);
uint8_t yDecode(int16_t);

int main(void)
{
	// Initialize I2C
	initI2C();
	initAccel();
	
	// Initialize SPI & LED Matrix
	SPIMasterInit();
	initLEDMatrix();
	
	clearDisplay();
	
	sensor accel;
	
	uint8_t xPos = 0, yPos = 0;
	
    while(1) 
    {
		accel.A[0] = xAccel();
		accel.A[1] = yAccel();
		
		xPos = xDecode((accel.A[0]));
		yPos = yDecode((accel.A[1]));
		
		writeMax(yPos, xPos);
		
		_delay_ms(20);
		clearDisplay();
    }
}

/*
*	Function initializes Accelerometer.
*/
void initAccel()
{
	I2CCMD(0x19, 0x00);		// 0x19 - Sample Rate Divider	-	Value 0 for 1kHz sample rate
	I2CCMD(0x1A, 0x82);		// 0x1A	- DLPF Config			-	0x02
	I2CCMD(0x1B, 0x10);		// 0x1B - Gyroscope Config		-	0x81	+- 2000°/s
	I2CCMD(0x1C, 0x10);		// 0x1C	- Accelerometer Config	-	0x10	+- 4g
	I2CCMD(0x23, 0x81);		// 0x23	- FIFO Enable			-	Enables data to be written to the FIFO buffer
	I2CCMD(0x24, 0x0D);		// 0x24	- I2C Master CTRL		-	0x0D
	I2CCMD(0x6A, 0x40);		// 0x6A	- FIFO_EN				-	Enable FIFO Buffer
	I2CCMD(0x6B, 0x00);		// 0x6B	- CLKSEL				-	Internal 8MHz Oscillator
}

/*
*	Function translates accelerometer's x position
*	to the 8x8 LED Matrix.
*	Passing Parameters:
*	int16_t - data - Accelerometer's x position
*/
uint8_t xDecode(int16_t data)
{
	uint8_t i = 0;
	
	if(data > 0) // Right Nibble
	{
		while(data > 104)
		{
			i++;
			data -= 104;
		}
		//writeMax(0x01, (0x10<<i));
		return (0x10<<i);
	}
	else         // Left Nibble
	{
		while(data <= 0)
		{
			i++;
			data += 104;
		}
		//writeMax(0x01, (0x10>>i));
		return (0x10>>i);
	}
}

/*
*	Function translates accelerometer's y position
*	to the 8x8 LED Matrix.
*	Passing Parameters:
*	int16_t - data - Accelerometer's y position
*/
uint8_t yDecode(int16_t data)
{
	uint8_t i = 1;
	
	if(data > 0) // Upper Nibble - 412
	{
		while(data - (104*i) > 0)
		{
			i++;
		}
		i = 5 - i;
		//writeMax(i, 0x80);
		return i;
	}
	else         // Bottom Nibble -  -412
	{
		while((data + (104*i)) < 0)
		{
			i++;
		}
		//writeMax(i+4, 0x80);
		return i+4;
	}
}