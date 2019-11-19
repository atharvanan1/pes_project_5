/**
  * File Name 		- common.h
  * Description 	- file commonly to be included across system
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	-
  * URL				-
  */

#ifndef COMMON_H_
#define COMMON_H_
// Include Files
#include <stdint.h>
#include "MKL25Z4.h"

// Macros for Critical Section
#define START_CRITICAL() 	UART0->C2 &= ~(UART_C2_TIE_MASK | UART_C2_RIE_MASK)
#define END_CRITICAL() 		UART0->C2 |= UART_C2_TIE(1) | UART_C2_RIE(1)

// Structure to hold system status
typedef struct {
	uint8_t tx_ready_flag;
	uint8_t tx_data;
	uint8_t rx_ready_flag;
	uint8_t rx_data;
} system_t;

// Macros for system info
#define TX_FLAG_RESET	0
#define RX_FLAG_RESET	0
#define TX_DATA_RESET	0
#define RX_DATA_RESET	0

// Extern for system_info global variable
extern system_t system_info;

#endif /* COMMON_H_ */
