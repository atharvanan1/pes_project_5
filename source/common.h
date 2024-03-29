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
#include "circular_buffer.h"

// RX and TX buffer globally available
extern circular_buffer_t* rx_buffer;
extern circular_buffer_t* tx_buffer;


// Structure to hold system status
typedef struct {
	uint8_t tx_ready_flag;
	uint8_t rx_ready_flag;
	uint8_t or_flag;
	uint8_t ne_flag;
	uint8_t fe_flag;
	uint8_t pe_flag;
} system_t;

// Macros for system info
#define TX_FLAG_RESET	0
#define RX_FLAG_RESET	0
#define OR_FLAG_RESET	0
#define NE_FLAG_RESET	0
#define FE_FLAG_RESET	0
#define PE_FLAG_RESET	0

// Extern for system_info global variable
extern system_t system_info;



/*-----------------------------*/
/*|     Application Mode      |*/
/*-----------------------------*/
  #define ECHO_POLLING	1
/*  #define APP_POLLING		1  */
/*  #define ECHO_IRQN		1  */
/*	#define APP_IRQN		1  */
/*-----------------------------*/
/*|     Application Mode      |*/
/*-----------------------------*/


#endif /* COMMON_H_ */
