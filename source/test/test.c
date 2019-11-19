/**
  * File Name 		- test.c
  * Description 	- contains test cases for the program
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	-
  * URL				-
  */

#include "test.h"

system_t system_info = {0, 0, 0, 0};

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

	UCUNIT_TestcaseBegin("Test Case for Destroying Buffer\n\r");
	cb_destroy_buffer(buff1);
	UCUNIT_CheckIsNull(buff1);
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

	UCUNIT_TestcaseBegin("Testing application code\n\r");
	logger.Log_Write(__func__, mStatus, "Starting Application");
	application();
	logger.Log_Write(__func__, mStatus, "Ending Application");
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


void application(void)
{
	// Put it somewhere else later on
	uint8_t* char_array = NULL;
	char_array = (uint8_t *) malloc(128);
	for(int i = 0; i < 128; i++)
	{
		*(char_array + i) = 0;
	}

	//Table
	uint8_t table[50] = {0x40, 0x42, 0x31, 0x01, 0x42, 0x20, 0x33, \
			0x23, 0x52, 0x32, 0x54, 0x14, 0x04, 0x05, 0x23, 0x34, \
			0x32, 0x53, 0x10, 0x49, 0x51, 0x66, 0x15, 0x46, \
			0x11, 0x23, 0x28, 0x27, 0x39, 0x37, 0x34, 0x49, \
			0x32, 0x53, 0x10, 0x49, 0x51, 0x66, 0x15, 0x46, \
			0x11, 0x23, 0x28, 0x27, 0x39, 0x37, 0x34, 0x49, \
			0x56, 0x55};
	// Loop this part
	for(int i = 0; i < 50; i++)
	{
		*(char_array + table[i]) += 1;
	}
	print_report(char_array);
	free(char_array);
}

// Print report function
void print_report(uint8_t *char_array)
{
	pprintf("UART Report\n\r");
	char* LookUp[34] = {"NULL", "SOH", "STX", \
			"ETX", "EOT", "ENQ", "ACK", "BEL", \
			"BS", "HT", "LF", "VT", "FF", "CR", \
			"SO", "SI", "DLE", "DC1", "DC2", "DC3", \
			"DC4", "NAK", "SYN", "ETB", "CAN", \
			"EM", "SUB", "ESC", "FS", "GS", "RS", \
			"US", "SPACE", "DEL"
	};

 	for(uint8_t i = 0; i < 128; i++)
	{
		if(*(char_array + i))
		{
			if(i <= 0x20 || i == 0x7F)
			{
				pprintf("%-5s - %3d\n\r", LookUp[i], *(char_array + i));
			}
			else
			{
				pprintf("%-5c - %3d\n\r", i, *(char_array + i));
			}
		}
	}
}



