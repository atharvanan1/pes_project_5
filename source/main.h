/**
  * File Name 		- main.h
  * Description 	- 
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	-
  * URL				-
  */

#ifndef MAIN_H_
#define MAIN_H_

#include "clock_config.h"
#include "peripherals.h"
#include "board.h"
#include "pin_mux.h"
#include "MKL25Z4.h"
#include "common.h"
#include "logger.h"
#include "uart.h"
#include "led_control.h"
#include "circular_buffer.h"

#if defined(APP_POLLING) || defined(APP_IRQN)

static inline void reset_array(uint8_t* char_array);
void print_report(uint8_t *char_array);
void application_init(void);
void application(void);

typedef struct {
	uint8_t count;
	uint8_t* char_array;
} application_t;

#endif

#endif /* MAIN_H_ */
