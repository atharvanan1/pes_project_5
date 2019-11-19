/**
 * File - timestamp.c
 * Author - Atharva Nandanwar
 * Email - atharva.nandanwar@colorado.edu
 * Principles of Embedded Software
 * University of Colorado Boulder
 */

#include "timestamp.h"

// System Clock Macro
#define SYSCLOCK 48000000UL

// Global deciseconds count
uint32_t deciseconds = 0;

/**
 * vTimestamp_Init
 * Sets up SysTick timer with 0.1 second
 */
void vTimestamp_Init(void)
{
	SysTick_Config(SYSCLOCK/10);
}

/**
 * tTimestamp_Get_Timestamp
 * Gets time stamp data
 * @return
 * 		returns a struct with timestamp information
 */
timestamp_t tTimestamp_Get_Timestamp(void)
{
	uint32_t temp;
	timestamp_t currentTime;
	currentTime.hours = deciseconds / 36000;
	temp = deciseconds % 36000;
	currentTime.minutes = temp / 600;
	temp = temp % 600;
	currentTime.seconds = temp / 10;
	currentTime.deciseconds = temp % 10;
	return currentTime;
}

/**
 * SysTick_Handler
 * Interrupt handler for systick interrupt
 */
void SysTick_Handler(void)
{
	deciseconds++;
}
