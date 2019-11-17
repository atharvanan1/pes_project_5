/**
  * File Name 		- test.c
  * Description 	- contains test cases for the program
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	-
  * URL				-
  */

#include "uart.h"

extern uint8_t int_flag;

void UART0_Init(UARTConfig_t* uart_config) {
    uint16_t sbr;

    // Enable clock gating for UART0 and Port A
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

    // Make sure transmitter and receiver are disabled before init
    UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

    // Set UART clock to 48 MHz clock
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
    SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;

    // Set pins to UART0 Rx and Tx
    PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
    PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

    // Set baud rate and oversampling ratio
    sbr = (uint16_t)((SYS_CLOCK)/(uart_config->baud_rate * (uart_config->osr + 1)));
    UART0->BDH &= ~UART0_BDH_SBR_MASK;
    UART0->BDH |= UART0_BDH_SBR(sbr >> 8);
    UART0->BDL = UART0_BDL_SBR(sbr);
    UART0->C4 |= UART0_C4_OSR(uart_config->osr);

    // Disable interrupts for RX active edge and LIN break detect, select one stop bit
    UART0->BDH |= UART0_BDH_SBNS(uart_config->stop_bit);

    UART0->C1 |= uart_config->parity;

    UART0->C3 =0;

    // Clear error flags
    UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

    // Send LSB first, do not invert received data
    UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

    // Enable UART receiver and transmitter
    UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

    // Clear the UART RDRF flag
    uint8_t temp = UART0->D;
    UART0->S1 &= ~UART0_S1_RDRF_MASK;

}

#include <stdio.h>

int	putchar (int)
{
        while (!(UART0->S1 & UART0_S1_TDRE_MASK));
        UART0->D = (uint8_t)ch;
}

int	getchar (void)
{
        while (!(UART0->S1 & UART0_S1_RDRF_MASK));
        return UART0->D;
}

//void puts(const char* string)
//{
//    uint8_t ch;
//    char* pointer = (char *) string;
//    while((ch = *pointer) != '\0')
//    {
//        putchar(ch);
//        pointer++;
//    }
//}
//
//void put_num(uint32_t number)
//{
//    uint8_t* string = NULL;
//    string = (uint8_t *) malloc(11);
//    uint8_t index = 0;
//    uint32_t temp = number;
//    while(temp != 0)
//    {
//        temp /= 10;
//        index++;
//    }
//    *(string + index) = '\0';
//    while(index != 0)
//    {
//    	index--;
//        *(string + index) = number % 10 + '0';
//        number /= 10;
//    }
//    puts((const char *)string);
//    free(string);
//}
//
//void printf(const char* string, ...)
//{
//    va_list args;
//    char *p, *str, ch;
//    uint32_t integer;
//    va_start(args, string);
//
//    for(p = (char *) string; *p; p++)
//    {
//        if(*p != '%')
//        {
//            putchar(*p);
//            continue;
//        }
//        switch(*++p)
//        {
//            case 'd':
//                integer = va_arg(args, int);
//                put_num(integer);
//                break;
//            case 's':
//                str = va_arg(args, char *);
//                puts(str);
//                break;
//            case '0':
//            case '-':
//        }
//    }
//}
