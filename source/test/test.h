/**
  * File Name 		- test.h
  * Description 	- header file for test.c
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	-
  * URL				-
  */

#ifndef TEST_TEST_H_
#define TEST_TEST_H_

// Include Files
#include <stdint.h>

#include "pin_mux.h"
#include "peripherals.h"
#include "clock_config.h"
#include "board.h"

// Includes for test functions
#include "System.h"
#include "uCUnit.h"

// Includes for functions
#include "logger.h"
#include "errno.h"
#include "uart.h"
#include "circular_buffer.h"
#include "led_control.h"
#include "common.h"

void application(void);
void print_report(uint8_t *char_array);

#endif /* TEST_TEST_H_ */
