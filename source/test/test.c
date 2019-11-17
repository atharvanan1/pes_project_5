/**
  * File Name 		- test.c
  * Description 	- contains test cases for the program
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	-
  * URL				-
  */

#include "test.h"

static inline void delay(void)
{
	for(volatile int i = 10000; i > 0; i--);
}
/*
 * Function - unit_tests
 * Brief - Executes unit tests
 */
void unit_tests(void)
{
	UARTConfig_t uart_config = {
			baud_115200,
			parity_off,
			single_stop_bit,
			OSR_32,
	};
	UART0_Init(&uart_config);
	UCUNIT_TestcaseBegin("Starting Test Cases\n\r");
	UCUNIT_TestcaseBegin("Test Case for UART\n\r");
	UCUNIT_TestcaseEnd();
}

/*
 * Function - Main
 * Brief - Main testing routine
 */
int main(void)
{
	//Initializing board pins
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();

	//Calling function to run tests
	unit_tests();
	return 0;
}

