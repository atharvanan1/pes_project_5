/*
 * uart.h
 *
 *  Created on: Nov 12, 2019
 *      Author: akshh
 */

#ifndef UART_H_
#define UART_H_
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include "MKL25Z4.h"

#define SYS_CLOCK               48000000UL

typedef enum {
	baud_115200 = 115200,
	baud_57600 = 57600,
	baud_38400 = 38400,
	baud_19200 = 19200,
	baud_14400 = 14400,
	baud_9600 = 9600,
} UARTBaudRate_t;

typedef enum {
	parity_off = 0x0,
	parity_even = 0x2,
	parity_odd = 0x3,
} UARTParityMode_t;

typedef enum {
	single_stop_bit = 0x0,
	double_stop_bit = 0x1,
} UARTStopBit_t;

typedef enum {
	OSR_4 = 0x03,
	OSR_5,
	OSR_6,
	OSR_7,
	OSR_8,
	OSR_9,
	OSR_10,
	OSR_11,
	OSR_12,
	OSR_13,
	OSR_14,
	OSR_15,
	OSR_16,
	OSR_17,
	OSR_18,
	OSR_19,
	OSR_20,
	OSR_21,
	OSR_22,
	OSR_23,
	OSR_24,
	OSR_25,
	OSR_26,
	OSR_27,
	OSR_28,
	OSR_29,
	OSR_30,
	OSR_31,
	OSR_32,

} UARTOSRSetting_t;

typedef struct {
	UARTBaudRate_t baud_rate;
	UARTParityMode_t parity;
	UARTStopBit_t stop_bit;
	UARTOSRSetting_t osr;
} UARTConfig_t;

void UART0_Init(UARTConfig_t* uart_config);

//uint8_t getchar(void);
//void putchar(uint8_t ch);
//void printf(const char* fmt, ...);
#endif /* UART_H_ */
