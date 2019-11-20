/**
  * File Name 		- test.c
  * Description 	- contains test cases for the program
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	-
  * URL				-
  */

#include "test.h"

system_t system_info = {0, 0, 0, 0, 0, 0};

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
	logger.Init();
	logger.Set_Log_Level(lTest);
	UARTConfig_t uart_config = {
			baud_115200,
			parity_off,
			single_stop_bit,
			OSR_32,
	};
	uart_init(&uart_config);
	UCUNIT_TestcaseBegin("Starting Test Cases\n\r");
	UCUNIT_TestcaseBegin("Test Case for UART\n\r");
	pprintf("Here is me!\n\r");
	UCUNIT_TestcaseEnd();


	UCUNIT_TestcaseBegin("Test Case for Circular Buffer\n\r");
	circular_buffer_t* buff1 = NULL;
	buff1 = cb_init_buffer(100);
	UCUNIT_CheckIsEqual(CB_buffer_initialized, cb_verify_init(buff1));
	UCUNIT_CheckIsEqual(100, buff1->length);
	UCUNIT_CheckIsEqual(0, buff1->count);
	UCUNIT_TestcaseEnd();


	uint8_t* data = (uint8_t *) malloc(1);
	*data = 0x55;
	UCUNIT_TestcaseBegin("Test Case for Circular Buffer Add Item\n\r");
	cb_add_item(buff1, 2);
	UCUNIT_CheckIsEqual(100, buff1->length);
	UCUNIT_CheckIsEqual(1, buff1->count);
	UCUNIT_CheckIsEqual(buff1->pointer + 1, buff1->head);
	UCUNIT_CheckIsEqual(buff1->pointer, buff1->tail);
	printf("Data is %d\n\r", *buff1->tail);
	UCUNIT_TestcaseEnd();


	UCUNIT_TestcaseBegin("Test Case for Circular Buffer Remove Item\n\r");
	cb_remove_item(buff1, data);
	UCUNIT_CheckIsEqual(100, buff1->length);
	UCUNIT_CheckIsEqual(0, buff1->count);
	UCUNIT_CheckIsEqual(buff1->pointer + 1, buff1->head);
	UCUNIT_CheckIsEqual(buff1->pointer + 1, buff1->tail);
	UCUNIT_TestcaseEnd();

	// Filling the buffer
	for(uint16_t i = 0; i < 100; i++)
	{
		cb_add_item(buff1, i);
	}


	UCUNIT_TestcaseBegin("Test Case for Circular Buffer Full\n\r");
	UCUNIT_CheckIsEqual(100, buff1->count);
	UCUNIT_CheckIsEqual(CB_buffer_full, cb_add_item(buff1, 5));
	UCUNIT_CheckIsEqual(buff1->pointer + 1, buff1->head);
	UCUNIT_CheckIsEqual(buff1->pointer + 1, buff1->tail);
	UCUNIT_TestcaseEnd();

	// empty circular buffer
	for(uint16_t i = 0; i <= 100; i++)
	{
		printf("Data %d is %d\n\r", i, *buff1->tail);
		cb_remove_item(buff1, data);
	}


	UCUNIT_TestcaseBegin("Test Case for Circular Buffer Empty\n\r");
	UCUNIT_CheckIsEqual(0, buff1->count);
	UCUNIT_CheckIsEqual(CB_buffer_empty, cb_remove_item(buff1, data));
	UCUNIT_CheckIsEqual(buff1->pointer + 1, buff1->head);
	UCUNIT_CheckIsEqual(buff1->pointer + 1, buff1->tail);
	UCUNIT_TestcaseEnd();


	UCUNIT_TestcaseBegin("Testing LED Functions\n\r");
	LED_Init();
	Turn_On_Only_LED(Red);
	for(volatile int i = 65535; i > 0; i--);
	Turn_On_Only_LED(Green);
	for(volatile int i = 65535; i > 0; i--);
	Turn_On_Only_LED(Blue);
	for(volatile int i = 65535; i > 0; i--);
	Turn_Off_LEDs();
	UCUNIT_TestcaseEnd();


	UCUNIT_TestcaseBegin("Testing Logger Functions\n\r");
	for(volatile int i = 65535; i > 0; i--);
	for(volatile int i = 65535; i > 0; i--);
	for(volatile int i = 65535; i > 0; i--);
	for(volatile int i = 65535; i > 0; i--);
	for(volatile int i = 65535; i > 0; i--);
	for(volatile int i = 65535; i > 0; i--);
	for(volatile int i = 65535; i > 0; i--);
	logger.Log_Write(__func__, mStatus, "Testing logger");
	for(volatile int i = 65535; i > 0; i--);
	for(volatile int i = 65535; i > 0; i--);
	for(volatile int i = 65535; i > 0; i--);
	for(volatile int i = 65535; i > 0; i--);
	for(volatile int i = 65535; i > 0; i--);
	for(volatile int i = 65535; i > 0; i--);
	for(volatile int i = 65535; i > 0; i--);
	logger.Log_Write(__func__, mError, "Testing logger");
	for(volatile int i = 65535; i > 0; i--);
	for(volatile int i = 65535; i > 0; i--);
	for(volatile int i = 65535; i > 0; i--);
	for(volatile int i = 65535; i > 0; i--);
	for(volatile int i = 65535; i > 0; i--);
	for(volatile int i = 65535; i > 0; i--);
	for(volatile int i = 65535; i > 0; i--);
	logger.Log_Write(__func__, mDebug, "Testing logger");
	logger.Log_Write(__func__, mError, "Testing logger");
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

	while(1)
	{
		uart_echo();
	}
	return 0;
}



