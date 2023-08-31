/*
 *	File Name: I2C.h
 *
 *	Created: 11/12/2019
 *
 *	Author: Luigi Moca
 */
#ifndef I2C__H
#define I2C__H

#define F_CPU 16000000UL  //16MHz CPU

#include <util/twi.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

extern void initI2C(void);
extern void i2cStart(void);
extern void i2cStop(void);

extern void writeAddr(void);
extern void writeI2C(uint8_t);
extern void I2CCMD(uint8_t, uint8_t);

extern uint8_t readI2C(void);
extern void readAddr(void);
extern uint8_t getStatus(void);

extern int16_t accelTemp(void);

extern int16_t xAccel(void);
extern int16_t yAccel(void);
extern int16_t zAccel(void);

extern int16_t xGyro(void);
extern int16_t yGyro(void);
extern int16_t zGyro(void);

typedef struct
{

	int16_t A[3];	// Accelerometer
	int16_t G[3];	// Gyroscope
	
	int16_t temp;	// Accelerometer Temperature
}sensor;
#endif