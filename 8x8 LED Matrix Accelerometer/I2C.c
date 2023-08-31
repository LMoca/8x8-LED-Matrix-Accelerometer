#define F_CPU 16000000UL  // 16MHz CPU
#define write	0x00
#define read	0x01

#define ADDR 0xD0

#include <util/twi.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void initI2C(void);
void i2cStart(void);
void i2cStop(void);

void writeAddr(void);
void writeI2C(uint8_t);
void I2CCMD(uint8_t, uint8_t);

uint8_t readI2C(void);
uint8_t getStatus(void);
int16_t accelTemp(void);

int16_t xAccel(void);
int16_t yAccel(void);
int16_t zAccel(void);

int16_t xGyro(void);
int16_t yGyro(void);
int16_t zGyro(void);

/*
*	Function initializes I2C peripheral.
*/
void initI2C()
{
	TWSR = 0x00;		// Pre-scaler
	TWCR = (1<<TWEN);	// Enable I2C
	TWBR = 0x03;
	//printf("I2C status Init: %X\n", getStatus());
}

/*
*	Function starts communication to the accelerometer.
*/
void i2cStart()
{
	//SDA goes LOW, SCL is high
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	//printf("Start Before: %X\n", getStatus());
	while(!(TWCR & (1<<TWINT)));
	
	//if((TWSR & 0xF8) == 0x08) printf("%X - Start transmitted\n", getStatus());
	//else if((TWSR & 0xF8) == 0x10) printf("%X - Repeated Start transmitted\n", getStatus());
	//else printf("Start error\n");
}

/*
*	Function stops communication to the accelerometer.
*/
void i2cStop()
{
	TWCR = (1<<TWSTO) | (1<<TWEN) | (1<<TWINT);
	_delay_us(100);
}

/*
*	Function sends a SLA+W.
*/
void writeAddr()
{
	TWDR = ADDR | write;
	TWCR = (1<<TWINT) | (1<<TWEN);
	//printf("Write Addr Before: %X\n", getStatus());
	while(!(TWCR & (1<<TWINT)));
	//printf("Write Addr After: %X\n", getStatus());
}

/*
*	Function writes data to send.
*	Passing Parameters:
*	uint8_t - data - Data to send to slave device.
*/
void writeI2C(uint8_t data)
{
	TWDR = data;
	TWCR = ((1<<TWINT) | (1<<TWEN));
	//printf("Write Data Before: %X\n", getStatus());
	while(!(TWCR & (1<<TWINT)));
	//printf("Write Data After: %X\n", getStatus());
}

/*
*	Function writes data to the internal registers.
*	Passing Parameters:
*	uint8_t reg		-	Internal Register Address
*	uint8_t data	-	Data to be written to the Register Address.
*/
void I2CCMD(uint8_t reg, uint8_t data)
{
	i2cStart();			// Sends a start signal
	writeAddr();		// SLA+W
	
	writeI2C(reg);		// The register to write data to
	writeI2C(data);		// Data to be written to the internal register
	i2cStop();			// Sends a stop signal
	
	//printf("Register %X - %X\n", reg, data);
}

/*
*	Function reads incoming I2C data.
*/
uint8_t readI2C()
{
	//printf("Read Data Before\n");
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	
	//printf("Read Data After: %X\n", TWCR);
	//printf("TWDR Read: %X\n", TWDR);
	return TWDR;
}

/*
*	Function sends a SLA+R.
*/
void readAddr()
{
	TWDR = ADDR | read;
	TWCR = (1<<TWINT) | (1<<TWEN);
	//printf("Read Addr Before: %X\n", getStatus());
	while(!(TWCR & (1<<TWINT)));
	//printf("Read Addr After : %X\n", getStatus());
	
	//if(getStatus() == 0x48) i2cStart();
}

/*
*	Function obtains the status.
*/
uint8_t getStatus(void)
{
	uint8_t status;
	//mask status
	status = TWSR & 0xF8;
	return status;
}

/*
*	Reads the output temperature from accelerometer.
*/
int16_t accelTemp()
{
	int16_t temp = 0x0000;
	
	i2cStart();					// Send start signal
	writeAddr();				// Send SLA+W
	
	// 0x41 - register address for Temperature [15:8]
	writeI2C(0x41);				// MPU Internal Register Addr
	
	i2cStart();					// Repeated Start
	readAddr();					// SLA+R
	
	temp = readI2C();				// Receive Data
	
	i2cStop();					// Send Stop Command
	
	temp<<=8;

	i2cStart();					// Send start signal
	writeAddr();				// Send SLA+W
	
	writeI2C(0x42);				// MPU Internal Register Addr
	
	i2cStart();					// Repeated Start
	readAddr();					// SLA+R
	
	temp |= readI2C();				// Receive Data
	
	i2cStop();					// Send Stop Command
	
	temp = (temp/340) + 36.53;
	
	return temp;
}

/*
*	Reads the output x position from accelerometer.
*/
int16_t xAccel()
{
	int16_t x = 0x0000;
	
	i2cStart();					// Send start signal
	writeAddr();				// Send SLA+W
	
	// 0x41 - register address for Temperature [15:8]
	writeI2C(0x3B);				// MPU Internal Register Addr
	
	i2cStart();					// Repeated Start
	readAddr();					// SLA+R
	
	x = readI2C();				// Receive Data
	
	i2cStop();					// Send Stop Command
	
	x<<=8;

	i2cStart();					// Send start signal
	writeAddr();				// Send SLA+W
	
	writeI2C(0x3C);				// MPU Internal Register Addr
	
	i2cStart();					// Repeated Start
	readAddr();					// SLA+R
	
	x |= readI2C();				// Receive Data
	
	i2cStop();					// Send Stop Command
	
	x = ~x + 1;
	x /=10;
	return x;
}

/*
*	Reads the output y Accel from accelerometer.
*/
int16_t yAccel()
{
	int16_t y = 0x0000;
	
	i2cStart();					// Send start signal
	writeAddr();				// Send SLA+W
	
	// 0x41 - register address for Temperature [15:8]
	writeI2C(0x3D);				// MPU Internal Register Addr
	
	i2cStart();					// Repeated Start
	readAddr();					// SLA+R
	
	y = readI2C();				// Receive Data
	
	i2cStop();					// Send Stop Command
	
	y<<=8;

	i2cStart();					// Send start signal
	writeAddr();				// Send SLA+W
	
	writeI2C(0x3E);				// MPU Internal Register Addr
	
	i2cStart();					// Repeated Start
	readAddr();					// SLA+R
	
	y |= readI2C();				// Receive Data
	
	i2cStop();					// Send Stop Command
	
	y = ~y + 1;
	y /=10;
	return y;
}

/*
*	Reads the output z Accel from accelerometer.
*/
int16_t zAccel()
{
	int16_t z = 0x0000;
	
	i2cStart();					// Send start signal
	writeAddr();				// Send SLA+W
	
	// 0x41 - register address for Temperature [15:8]
	writeI2C(0x3F);				// MPU Internal Register Addr
	
	i2cStart();					// Repeated Start
	readAddr();					// SLA+R
	
	z = readI2C();				// Receive Data
	
	i2cStop();					// Send Stop Command
	
	z<<=8;

	i2cStart();					// Send start signal
	writeAddr();				// Send SLA+W
	
	writeI2C(0x40);				// MPU Internal Register Addr
	
	i2cStart();					// Repeated Start
	readAddr();					// SLA+R
	
	z |= readI2C();				// Receive Data
	
	i2cStop();					// Send Stop Command
	
	z = ~z + 1;
	z /=10;
	return z;
}

/*
*	Reads the output x Gyro from accelerometer.
*/
int16_t xGyro()
{
	int16_t x = 0x0000;
	
	i2cStart();					// Send start signal
	writeAddr();				// Send SLA+W
	
	// 0x41 - register address for Temperature [15:8]
	writeI2C(0x43);				// MPU Internal Register Addr
	
	i2cStart();					// Repeated Start
	readAddr();					// SLA+R
	
	x = readI2C();				// Receive Data
	
	i2cStop();					// Send Stop Command
	
	x<<=8;

	i2cStart();					// Send start signal
	writeAddr();				// Send SLA+W
	
	writeI2C(0x44);				// MPU Internal Register Addr
	
	i2cStart();					// Repeated Start
	readAddr();					// SLA+R
	
	x |= readI2C();				// Receive Data
	
	i2cStop();					// Send Stop Command
	
	x = ~x + 1;
	x /=10;
	return x;
}

/*
*	Reads the output y Gyro from accelerometer.
*/
int16_t yGyro()
{
	int16_t y = 0x0000;
	
	i2cStart();					// Send start signal
	writeAddr();				// Send SLA+W
	
	// 0x41 - register address for Temperature [15:8]
	writeI2C(0x45);				// MPU Internal Register Addr
	
	i2cStart();					// Repeated Start
	readAddr();					// SLA+R
	
	y = readI2C();				// Receive Data
	
	i2cStop();					// Send Stop Command
	
	y<<=8;

	i2cStart();					// Send start signal
	writeAddr();				// Send SLA+W
	
	writeI2C(0x46);				// MPU Internal Register Addr
	
	i2cStart();					// Repeated Start
	readAddr();					// SLA+R
	
	y |= readI2C();				// Receive Data
	
	i2cStop();					// Send Stop Command
	
	y = ~y + 1;
	y /=10;
	return y;
}

/*
*	Reads the output z Gyro from accelerometer.
*/
int16_t zGyro()
{
	int16_t z = 0x0000;
	
	i2cStart();					// Send start signal
	writeAddr();				// Send SLA+W
	
	// 0x41 - register address for Temperature [15:8]
	writeI2C(0x47);				// MPU Internal Register Addr
	
	i2cStart();					// Repeated Start
	readAddr();					// SLA+R
	
	z = readI2C();				// Receive Data
	
	i2cStop();					// Send Stop Command
	
	z<<=8;

	i2cStart();					// Send start signal
	writeAddr();				// Send SLA+W
	
	writeI2C(0x48);				// MPU Internal Register Addr
	
	i2cStart();					// Repeated Start
	readAddr();					// SLA+R
	
	z |= readI2C();				// Receive Data
	
	i2cStop();					// Send Stop Command
	
	z = ~z + 1;
	z /=10;
	return z;
}