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

int main(void)
{
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	logger.Init();
	logger.Set_Log_Level(lTest);
	UARTConfig_t uart_config = {
			baud_115200,
			parity_off,
			single_stop_bit,
			OSR_32,
	};
	uart_init(&uart_config);
	LED_Init();
	logger.Log_Write(__func__, mStatus, "Starting Program");
	while(1)
	{
#if defined(APP_IRQN)
		uart_interrupt_handler();
#elif defined(ECHO_IRQN)
		uart_echo();
#elif defined(APP_POLLING)
		application();
#elif defined(ECHO_POLLING)
		uart_echo();
#endif
	}
}

#if defined(APP_POLLING) || defined(APP_IRQN)
static inline void reset_array(uint8_t* char_array)
{
	for(int i = 0; i < 128; i++)
	{
		*(char_array + i) = 0;
	}
}

void application(void)
{
	// Put it somewhere else later on
	uint8_t* char_array = NULL;
	char_array = (uint8_t *) malloc(128);

	reset_array(char_array);

	uint16_t count = 0;
	// Need to implement as it would be in main
	// Loop this part
	while(1)
	{

		*(char_array + uart_getchar()) += 1;
		count++;
		if(count % 50 == 0)
		{
			print_report(char_array);
			reset_array(char_array);
		}
	}
	free(char_array);
}

// Print report function
void print_report(uint8_t *char_array)
{
	logger.Log_Write(__func__, mStatus, "Printing Debug Report");
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
	pprintf("UART Report Ends\n\r");
}
#endif
