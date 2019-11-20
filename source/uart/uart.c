/**
  * File Name 		- uart.c
  * Description 	- contains test cases for the program
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	-
  * URL				-
  */


#include "uart.h"

/**
 * uart_init
 * Initializes UART Peripheral
 * @param
 * 		uart_config - pointer to uart configuration structure
 * Leveraged Code - Alexander Dean Example
 */
void uart_init(UARTConfig_t* uart_config) {
    uint16_t sbr;

    // Enable clock gating for UART0 and Port A
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

    // Disable TX and RX modules before changing registers
    UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

    // Set UART clock to 48 MHz clock
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
    SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;

    // Set PORT pins to UART TX and RX
    PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
    PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

    // Calculate baud rate and oversampling rate
    sbr = (uint16_t)((SYS_CLOCK)/(uart_config->baud_rate * (uart_config->osr + 1)));
    UART0->BDH &= ~UART0_BDH_SBR_MASK;
    UART0->BDH |= UART0_BDH_SBR(sbr >> 8);
    UART0->BDL = UART0_BDL_SBR(sbr);
    UART0->C4 |= UART0_C4_OSR(uart_config->osr);

    // Setting for stop bit
    UART0->BDH |= UART0_BDH_SBNS(uart_config->stop_bit);

    // Setting for parity bit
    UART0->C1 |= uart_config->parity;

    // Clear error flags
    UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

    // Data format selection - LSB First, and No inversion
    UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

    // Enable UART TX/RX
    UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

    // Do a dummy read and reset receive flag
    __attribute__((unused)) uint8_t temp = UART0->D;
    UART0->S1 &= ~UART0_S1_RDRF_MASK;
}

// Enable Interrupts only in IRQN modes
#if defined(APP_IRQN) || defined(ECHO_IRQN)
/**
 * uart_enable_irq
 * Enable UART Interrupts
 */
void uart_enable_irq(void)
{
	// Enable Interrupts
	UART0->C2 |= UART_C2_RIE(1);

    // Set up C3 register for error based interrupts
    UART0->C3 |= UART_C3_ORIE(1) | UART_C3_NEIE(1) | UART_C3_FEIE(1) | UART_C3_PEIE(1);

    // Set up NVIC registers
	NVIC->ICPR[0] |= 1 << (UART0_IRQn);
	NVIC->ISER[0] |= 1 << (UART0_IRQn);
}
#endif

/**
 * uart_tx_available
 * Checks if TX is available
 * @return
 * 		UART status
 */
UARTStatus_t uart_tx_available(void)
{
	if(UART0->S1 & UART0_S1_TDRE_MASK)
		return TX_available;
	else
		return TX_not_available;
}

/**
 * uart_tx_action
 * Send data through UART
 * @param
 * 		data - 8 bit data
 */
void uart_tx_action(uint8_t data)
{
	UART0->D = data;
}

/**
 * uart_tx
 * Check if TX is available, and send the data
 * @param
 * 		data - 8 bit data
 */
void uart_tx(uint8_t* data)
{
	// Logic for polling
#if defined(APP_POLLING) || defined(ECHO_POLLING)
	while(uart_tx_available() != TX_available);
	Turn_On_Only_LED(Green);
#endif
	uart_tx_action(*data);
}

/**
 * uart_rx_check
 * Check if RX is available
 * @return
 * 		UART status
 */
UARTStatus_t uart_rx_check(void)
{
	if(UART0->S1 & UART0_S1_RDRF_MASK)
		return RX_available;
	else
		return RX_not_available;
}

/**
 * uart_rx_action
 * Receives data from UART
 * @return
 * 		return 8 bit data
 */
uint8_t uart_rx_action(void)
{
	return UART0->D;
}

/**
 * uart_rx
 * Check if RX is available, and get data from UART
 * @return
 * 		return 8 bit data
 */
void uart_rx(uint8_t* data)
{
	// Logic for polling
#if defined(APP_POLLING) || defined(ECHO_POLLING)
	while(uart_rx_check() != RX_available);
	Turn_On_Only_LED(Blue);
#endif
	*data = uart_rx_action();
}

/**
 * uart_tx_handler
 * Enables the TX Interrupt when there is data to send
 */
void uart_tx_handler(void)
{
	if(cb_check_empty(tx_buffer) == CB_buffer_not_empty)
	{
		UART0->C2 |= UART_C2_TIE_MASK;
	}
}

/**
 * uart_error_handler
 * Handles UART errors
 */
void uart_error_handler(void)
{
	if(system_info.pe_flag)
	{
		errno = eUART_Parity_Error;
		logger.Log_Write(__func__, mError, Get_Error_Message(errno));
	    // Do a dummy read and reset receive flag
	    __attribute__((unused)) uint8_t temp = UART0->D;
	    UART0->S1 &= ~UART0_S1_RDRF_MASK;
	}
	else if(system_info.ne_flag)
	{
		errno = eUART_Noise_Error;
		logger.Log_Write(__func__, mError, Get_Error_Message(errno));
	    // Do a dummy read and reset receive flag
	    __attribute__((unused)) uint8_t temp = UART0->D;
	    UART0->S1 &= ~UART0_S1_RDRF_MASK;
	}
	else if(system_info.or_flag)
	{
		errno = eUART_Overrun_Error;
		logger.Log_Write(__func__, mError, Get_Error_Message(errno));
	    // Do a dummy read and reset receive flag
	    __attribute__((unused)) uint8_t temp = UART0->D;
	    UART0->S1 &= ~UART0_S1_RDRF_MASK;
	}
	else if(system_info.fe_flag)
	{
		errno = eUART_Framing_Error;
		logger.Log_Write(__func__, mError, Get_Error_Message(errno));
	    // Do a dummy read and reset receive flag
	    __attribute__((unused)) uint8_t temp = UART0->D;
	    UART0->S1 &= ~UART0_S1_RDRF_MASK;
	}
}

/**
 * uart_echo
 * UART Echo function
 * Works with IRQ and Polling both
 */
void uart_echo(void)
{
#if defined(ECHO_POLLING)
	uint8_t temp = uart_getchar();
	uart_putchar(temp);
#elif defined(ECHO_IRQN)
	if(cb_check_empty(rx_buffer) != CB_buffer_empty)
	{
		uint8_t temp = uart_getchar();
		uart_putchar(temp);
	}
#endif
}

// Interrupt Handler for Interrupt Based Operation
#if defined(APP_IRQN) || defined(ECHO_IRQN)
/**
 * UART0_IRQHandler
 * UART Interrupt Service Routine
 */
void UART0_IRQHandler(void)
{
	// Receive interrupt
	if(UART0->S1 & UART0_S1_RDRF_MASK && UART0->C2 & UART0_C2_RIE_MASK)
	{
		uint8_t data = 0;
		uart_rx(&data);
		cb_add_item(rx_buffer, data);
		UART0->S1 &= ~UART0_S1_OR_MASK;
		Turn_On_Only_LED(Blue);
	}

	else if(UART0->S1 & UART0_S1_TDRE_MASK && UART0->C2 & UART0_C2_TIE_MASK)
	{
		uint8_t data = 0;
		uint8_t x = cb_remove_item(tx_buffer, &data);
		if(x != CB_buffer_empty)
		{
			uart_tx(&data);
			while(UART0->S1 & UART0_S1_TC_MASK);
		}
		Turn_On_Only_LED(Green);
		UART0->C2 &= ~UART_C2_TIE_MASK;
	}

	// Overrun Error Interrupt
	if(UART0->S1 & UART0_S1_OR_MASK)
	{
		system_info.or_flag = 1;
		Turn_On_Only_LED(Red);
	}

	// Noise Error Interrupt
	if(UART0->S1 & UART0_S1_NF_MASK)
	{
		system_info.ne_flag = 1;
		Turn_On_Only_LED(Red);
	}

	// Framing Error Interrupt
	if(UART0->S1 & UART0_S1_FE_MASK)
	{
		system_info.fe_flag = 1;
		Turn_On_Only_LED(Red);
	}

	// Parity Error Interrupt
	if(UART0->S1 & UART0_S1_PF_MASK)
	{
		system_info.pe_flag = 1;
		Turn_On_Only_LED(Red);
	}
}
#endif

/**
 * uart_putchar
 * Sends a character to UART
 * @param
 * 		ch - 8 bit character
 * 	Works with IRQ and Polling
 */
void uart_putchar(uint8_t ch)
{
#if defined(APP_POLLING) || defined(ECHO_POLLING)
	uart_tx(&ch);
#elif defined(APP_IRQN) || defined(ECHO_IRQN)
	cb_add_item(tx_buffer, ch);
#endif
}

/**
 * uart_getchar
 * Gets a character from UART
 * @return
 * 		return 8 bit data
 * Works with IRQ and Polling
 */
uint8_t uart_getchar (void)
{
	uint8_t temp;
#if defined(APP_POLLING) || defined(ECHO_POLLING)
    uart_rx(&temp);
#elif defined(APP_IRQN) || defined(ECHO_IRQN)
    cb_remove_item(rx_buffer, &temp);
#endif
    return temp;

}
/**
 * put_string
 * Prints string
 * @param
 * 		string - string to print
 */
void put_string(const char* string)
{
    uint8_t ch;
    char* pointer = (char *) string;

    // Till the end of string
    while((ch = *pointer) != '\0')
    {
        uart_putchar(ch);
        pointer++;
    }
}

/**
 * vpprintf
 * My implementation of va_list based printf
 * @param
 * 		fmt - string with formatting
 * @param
 * 		args - va_list
 */
void vpprintf(const char* fmt, va_list args)
{
	char* string = NULL;
	string = (char *) malloc(sizeof(char) * 200);
	vsprintf(string, fmt, args);
	put_string(string);
	free(string);
}

/**
 * pprintf
 * My implementation of printf
 * @param
 * 		fmt - string with formatting
 */
void pprintf(const char* fmt, ...)
{
	va_list(args);
	va_start(args, fmt);
	vpprintf(fmt, args);
}
