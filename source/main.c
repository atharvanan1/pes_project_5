/*
 * File - main.c
 * Brief -
 * Author - Atharva Nandanwar
 * University of Colorado Boulder
 * Principles of Embedded Software
 */

#include "main.h"

system_t system_info = {
		TX_FLAG_RESET,
		RX_FLAG_RESET,
		OR_FLAG_RESET,
		NE_FLAG_RESET,
		FE_FLAG_RESET,
		PE_FLAG_RESET
};

circular_buffer_t* rx_buffer = NULL;
circular_buffer_t* tx_buffer = NULL;

#if defined(APP_IRQN) || defined(APP_POLLING)
application_t application_data = {
		0,
		NULL,
};
#endif

int main(void)
{
	// All initialization functions - Logging Disabled
	// Due to reliance on UART peripheral
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();

	// RX/TX buffer initialization
	rx_buffer = cb_init_buffer(500);
	tx_buffer = cb_init_buffer(500);

	// Initializing Logger
	logger.Init();
#ifdef DEBUG
	logger.Set_Log_Level(lDebug);
#endif
#ifdef RUN
	logger.Set_Log_Level(lNormal);
#endif

	// Initializing UART
	UARTConfig_t uart_config = {
			baud_115200,
			parity_off,
			single_stop_bit,
			OSR_32,
	};
	uart_init(&uart_config);

	// LED Initialization - Logging can be used from here
	LED_Init();
	logger.Log_Write(__func__, mStatus, "Starting Program");

#if defined(APP_IRQN) || defined(APP_POLLING)
	application_init();
#endif

#if defined(APP_IRQN) || defined(ECHO_IRQN)
	uart_enable_irq();
#endif

#if defined(APP_IRQN)
	logger.Log_Write(__func__, mStatus, "Starting in Application Mode in IRQ");
#elif defined(ECHO_IRQN)
	logger.Log_Write(__func__, mStatus, "Starting in Echo Mode in IRQ");
#elif defined(APP_POLLING)
	logger.Log_Write(__func__, mStatus, "Starting in Application Mode in Polling");
#elif defined(ECHO_POLLING)
	logger.Log_Write(__func__, mStatus, "Starting in Echo Mode in Polling");
#endif

	while(1)
	{
#if defined(APP_IRQN)
		application();
		uart_tx_handler();
		uart_error_handler();
#elif defined(ECHO_IRQN)
		uart_echo();
		uart_tx_handler();
#elif defined(APP_POLLING)
		application();
#elif defined(ECHO_POLLING)
		uart_echo();
#endif
	}
}

// Application Mode Functions
#if defined(APP_POLLING) || defined(APP_IRQN)

/**
 * reset_array
 * Resets character counts
 */
static inline void reset_array(uint8_t* char_array)
{
	if(logger.Get_Log_Level() == lDebug)
		logger.Log_Write(__func__, mDebug, "Resetting character count");
	for(int i = 0; i < 128; i++)
	{
		*(char_array + i) = 0;
	}
}

/**
 * application_init
 * Finishes initialization for application
 */
void application_init(void)
{
	// Make an array and reset it
	application_data.char_array = (uint8_t *) malloc(128);
	reset_array(application_data.char_array);
}

/**
 * application
 * Code for application
 */
void application(void)
{
// Different implementations for polling and IRQ, IRQ is based on circular buffers
#if defined(APP_POLLING)
	*(application_data.char_array + uart_getchar()) += 1;
	application_data.count++;
	if(application_data.count % 50 == 0)
	{
			print_report(application_data.char_array);
			reset_array(application_data.char_array);
			application_data.count = 0;
	}
#elif defined(APP_IRQN)
	if(cb_check_empty(rx_buffer) != CB_buffer_empty)
	{
		*(application_data.char_array + uart_getchar()) += 1;
		application_data.count++;
		if(application_data.count % 50 == 0)
		{
			print_report(application_data.char_array);
			reset_array(application_data.char_array);
		}
	}
#endif
}

// Print report function
void print_report(uint8_t *char_array)
{
	logger.Log_Write(__func__, mStatus, "Printing Debug Report");
	pprintf("UART Report\n\r");
	// Look up table for escape characters in ascii
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
			// Print escape characters from Lookup table
			if(i <= 0x20 || i == 0x7F)
			{
				pprintf("%-5s - %3d\n\r", LookUp[i], *(char_array + i));
			}
			// Print rest of them normally
			else
			{
				pprintf("%-5c - %3d\n\r", i, *(char_array + i));
			}
		}
	}
	pprintf("UART Report Ends\n\r");
}
#endif
