/**
 * File - uart.c
 * Author - Atharva Nandanwar
 * Email - atharva.nandanwar@colorado.edu
 * Principles of Embedded Software
 * University of Colorado Boulder
 */
#include "uart.h"

void UART_Init(UARTConfig_t* uart_config)
{
	// Parity Bit
	uint8_t parity_bit;
	switch(uart_config->parity)
	{
	case parity_off:
		parity_bit = 0x00;
		break;
	case parity_even:
		parity_bit = 0x02;
		break;
	case parity_odd:
		parity_bit = 0x03;
		break;
	default:
		parity_bit = 0x00;
		break;
	}

	// Stop Bit
	uint8_t stop_bit;
	switch(uart_config->stop_bit)
	{
	case single_stop_bit:
		stop_bit = 0x00;
		break;
	case double_stop_bit:
		stop_bit = 0x01 << UART0_BDH_SBNS_SHIFT;
		break;
	default:
		stop_bit = 0x00;
		break;
	}

	// Enable clock gating for UART peripheral and PORT A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	// Make sure transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

    // Set UART clock to 48 MHz clock
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
    SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;

    // Set up Port A pins
    PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
    PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

    // Calculate essential parameters
    UART0->BDH &= ~UART0_BDH_SBR_MASK;
    //UART0->BDH = UART0_BDH_SBR(0) & UART0_BDH_SBNS(stop_bit);
    UART0->BDL |= UART0_BDL_SBR(13);
    UART0->C1 = 0;
    UART0->C3 = 0;
    UART0->C4 = UART0_C4_OSR(uart_config->osr);

    UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);
    UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

    // Enable UART receiver and transmitter
    UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

    uint8_t temp = UART0->D;
    UART0->S1 &= ~UART0_S1_RDRF_MASK;
}

uint8_t getchar (void)
{
    while (!(UART0->S1 & UART0_S1_RDRF_MASK));
    return UART0->D;
}

void putchar(uint8_t ch)
{
    while (!(UART0->S1 & UART0_S1_TDRE_MASK));
	UART0->D = ch;
}

void puts(const char* string)
{
    uint8_t index = 0;
    uint8_t c;
    while((c = string[index]) != '\0')
    {
        putchar(c);
    }
}

void put_num(uint32_t number)
{
    uint8_t string[11];
    uint8_t index = 0;
    uint32_t temp = number;
    while(temp != 0)
    {
        temp /= 10;
        index++;
    }
    string[index + 1] = '\0';
    while(index != 0)
    {
        string[index] = number % 10 + '0';
        number /= 10;
        index--;
    }
    puts((const char *)string);
}

 void put_float(float number)
 {
     number *= 100;

     uint8_t string[11];
     uint8_t index = 0;
     uint32_t temp = number;
     uint8_t size = 0;

     while(temp != 0)
     {
         temp /= 10;
         index++;
     }
     temp = number;
     string[index + 1] = '\0';
     while(index != 0)
     {
         string[index] = temp % 10  + '0';
         temp = temp / 10;
         index--;
         if(index == size - 2)
         {
             string[index] = '.';
             index--;
         }
     }
     puts((const char *)string);
 }

void put_hex(uint32_t hex, uint8_t capital)
{
    uint8_t string[9];
    uint8_t index = 0;
    uint32_t temp = hex;
    while(temp != 0)
    {
        temp /= 16;
        index++;
    }
    string[index + 1] = '\0';
    while(index != 0)
    {
        string[index] = hex % 16;
        if(string[index] >= 0 && string[index] <= 9)
        {
            string[index] += '0';
        }
        else if(string[index] >= 10 && string[index] <= 15)
        {
            if(capital)
                string[index] += 'A';
            else
                string[index] += 'a';
        }
        hex /= 16;
        index--;
    }
    puts((const char *)string);
}

void printf(const char* string, ...)
{
    va_list args;
    char* p, *str, ch;
    uint32_t integer;
    // float float_num;
    uint8_t index = 0;
    uint8_t padding = ' ';
    uint8_t loop;
    va_start(args, string);

    for(p = string; *p; p++)
    {
        if(*p != '%')
        {
            putchar(*p);
            continue;
        }
        switch(*++p)
        {
            case 'c':
                ch = va_arg(args, char);
                putchar(ch);
                break;
            case 'd':
                integer = va_arg(args, int);
                put_num(integer);
                break;
            case 's':
                str = va_arg(args, char *);
                puts(str);
                break;
            // case 'f':
            //     float_num = va_arg(args, float);
            //     put_float(float_num);
            case 'x':
                integer = va_arg(args, int);
                put_hex(integer, 0);
                break;
            case 'X':
                integer = va_arg(args, int);
                put_hex(integer, 0);
                break;
            case '0':
                padding = '0';
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                loop = (uint8_t) *p - '0';
                for(uint8_t i = 0; i < loop; i++)
                {
                    putchar(padding);
                }
            case '#':
                puts("0x");
        }
    }
}
