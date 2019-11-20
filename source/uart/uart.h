/**
  * File Name 		- uart.c
  * Description 	- contains test cases for the program
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	-
  * URL				-
  */

#ifndef UART_H_
#define UART_H_
// Include Files
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include "MKL25Z4.h"
#include "common.h"
#include "led_control.h"
#include "errno.h"
#include "logger.h"

// Macros
#define SYS_CLOCK	48000000UL

// Enum for Baud Rate selection
typedef enum {
	baud_115200 = 115200,
	baud_57600 = 57600,
	baud_38400 = 38400,
	baud_19200 = 19200,
	baud_14400 = 14400,
	baud_9600 = 9600,
} UARTBaudRate_t;

// Enum for Parity Setup
typedef enum {
	parity_off = 0x0,
	parity_even = 0x2,
	parity_odd = 0x3,
} UARTParityMode_t;

// Enum for Stop Bit Configuration
typedef enum {
	single_stop_bit = 0x0,
	double_stop_bit = 0x1,
} UARTStopBit_t;

// Enum for Oversampling Rate
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

// Structure for UART Configuration
typedef struct {
	UARTBaudRate_t baud_rate;
	UARTParityMode_t parity;
	UARTStopBit_t stop_bit;
	UARTOSRSetting_t osr;
} UARTConfig_t;

// Enum for UART Status
typedef enum {
	TX_available,
	TX_not_available,
	RX_available,
	RX_not_available,
} UARTStatus_t;

// Prototype Functions
void uart_init(UARTConfig_t* uart_config);
UARTStatus_t uart_tx_available(void);
void uart_tx_action(uint8_t data);
void uart_tx(uint8_t* data);
UARTStatus_t uart_rx_check(void);
uint8_t uart_rx_action(void);
void uart_rx(uint8_t* data);
void uart_enable_irq(void);
void uart_echo(void);
void uart_tx_handler(void);
void uart_error_handler(void);

// Prototype Functions for Print operations
uint8_t uart_getchar(void);
void uart_putchar(uint8_t ch);
void vpprintf(const char* fmt, va_list args);
void pprintf(const char* fmt, ...);

#endif /* UART_H_ */
