#define F_CPU 16000000UL // 16MHz

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

#include <avr/io.h>
#include <util/delay.h>

/*
*	Function initializes Master SPI.
*/
void SPIMasterInit()
{
	//PORTB is the port where SPI peripheral is located
	// PB2 - DIN (MOSI)
	// PB1 - CLK (SCK)
	// PB0 - CS  (SS)
	DDRB = (1<<DIN) | (1<<SCK) | (1<<CS);  // MOSI, SCK, & CS

	//enable SPI interrupt, enable SPI, Master Select, MSB transmitted first, F_CPU/16
	SPCR = /*(1<< SPIE) |*/ (1<<SPE) | (1<<MSTR) | (0<<DORD) | (0<<SPR1) | (1<<SPR0);
	SPSR = 0x00;
}

// Because the LED matrix has 16-bits on its register,
// the SPDR must be able to shift the bits to account for it.
void sendData(unsigned char data)
{
	//load the first 8-bit data to buffer
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
}

/*
*	Function writes data to the 8x8 LED Matrix's
*	internal address register.
*	Passing Parameters:
*	uint8_t - address	- 8x8 LED Matrix's Internal Address.
*	uint8_t - data		- Data to be sent to the Internal Address.
*/
void writeMax(uint8_t address, uint8_t data)
{
	LOADSTART;  //CS is set LOW to start loading data
	CLKHIGH;	//set CLK HIGH for incoming register address
	
	//send address first
	sendData(address);
	CLKLOW;		//set CLK LOW
	
	CLKHIGH;	//set CLK HIGH for incoming register address
	//send data
	sendData(data);
	CLKLOW;		//set CLK LOW
	LOADSTOP;	//CS is set HIGH to end loading data
	_delay_ms(5);
}

/*
*	Function initializes the 8x8 LED Matrix
*/
void initLEDMatrix()
{
	//add,  hex
	writeMax(DECODE, 0x00);		 //decode mode
	writeMax(SCAN_LIMIT, 0x07);  //scan limit
	writeMax(NORMAL_OP, 0x01);   //Normal Operation
	writeMax(INTENSITY, 0x04);   //define intensity
	writeMax(TEST, 0x01);		 //display test
	_delay_ms(500);
	writeMax(TEST, 0x00);		 //display test
}

/*
*	Function clears the 8x8 LED Matrix display.
*/
void clearDisplay()
{
	for(uint8_t i = 1; i < 9; ++i)
	{
		writeMax(i,0x00);
	}
}