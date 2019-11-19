# Principles of Embedded Software Project 5

## Documentation

### README

```markdown
Principles of Embedded Software Project 5
README

Name: Atharva Nandanwar

This repository contains source files for Principles of Embedded Software Project 5 - UART and Circular Buffer

Source Folder:

    main.c/h - main subroutine

    circular_buffer/circular_buffer.c/h - functions and structure definition for circular buffer

    led_control/led_control.c/h - functions to control LED

    logger/logger.c/h - functions to do logging

    logger/errno.c/h - error handling routines

    logger/timestamp.c/h - timestamp functionality

    test/test.c/h - test cases and test subroutine

    uart/uart.c/h - uart subroutines, initialization, and operation drivers

    uctest - uCunit testing files

    common.h - common include file for system-wide implementation

Observations:

    There was serious problems with how integration would work in this aspect. How do I interface interrupt service routines with circular buffer operations? How do I interface UART functions with circular buffer? How do I manage getchar and putchar functionality with everything in the system? I still haven't found solution to this problem. I did not want to make a program with hodgepodge integration, so will take time to find it.

    The way I had designed logger required me to implement a lot of complicated printf operations. I rather used sprintf to format data into a string and printing the string directly.

Installation/Execution Notes:

Use basic options to compile RUN, LOG, and TEST builds from Build Targets.

Compiler - gcc-arm-none-eabi

There are different functionalities that can be configured in RUN and LOG mode:

    UART Non-blocking Echo Mode - #define ECHO_POLLING

    UART Non-blocking Application Mode - #define APP_POLLING

    UART Interrupts Echo Mode - #define ECHO_IRQN

    UART Interrupts Application Mode - #define APP_IRQN

```

---

### Makefile

```makefile

# Makefile for PES Project 5
# Author : Atharva Nandanwar
# Date: 11/14/2019

#####################################################################
# Build Variables

# Program for removing files
RM := rm -rf

# Program for making directories
MK := mkdir -p

# ARM compiler
ARM_CC := arm-none-eabi-gcc

# ARM linker
ARM_LL := arm-none-eabi-gcc

#####################################################################
# ARM Compiler Flags
ARM_FLAGS := -c \
			 -std=c99 \
			 -O0 \
			 -g3 \
			 -ffunction-sections \
			 -fmessage-length=0 \
			 -fno-common \
			 -fdata-sections \
			 -fno-builtin \
			 -mcpu=cortex-m0plus \
			 -mthumb \
			 -Wall \
			 -Werror
			 
# ARM Linker Flags
ARM_LL_FLAGS := -v \
				-nostdlib \
			    -Xlinker -Map="./Debug/pes_project_5.map" \
			    -Xlinker --gc-sections \
			    -Xlinker -print-memory-usage \
			    -Xlinker --sort-section=alignment \
			    -Xlinker --cref \
			    -mcpu=cortex-m0plus \
			    -mthumb \
			    -T linkerfile.ld \
			    -o $(EXE)
			
# ARM Defines
ARM_DEFS := \
		   -D__REDLIB__ \
		   -DCPU_MKL25Z128VLK4 \
		   -DCPU_MKL25Z128VLK4_cm0plus \
		   -DSDK_OS_BAREMETAL \
		   -DFSL_RTOS_BM \
		   -DCR_INTEGER_PRINTF \
		   -DPRINTF_FLOAT_ENABLE=0 \
		   -DSCANF_FLOAT_ENABLE=0 \
		   -DPRINTF_ADVANCED_ENABLE=0 \
		   -DSCANF_ADVANCED_ENABLE=0 \
		   -D__MCUXPRESSO \
		   -D__USE_CMSIS \
		   -DDEBUG \
		   -DFRDM_KL25Z \
		   -DFREEDOM \
		   -specs=redlib.specs \
		   
# Build Folders
SOURCE := ./source
DEBUG := ./Debug

# ARM Include Files
ARM_INCS := \
		   -I"$(SOURCE)" \
		   -I"$(SOURCE)/uart" \
		   -I"$(SOURCE)/led_control" \
		   -I"$(SOURCE)/logger" \
		   -I"$(SOURCE)/application" \
		   -I"$(SOURCE)/circular_buffer" \
		   -I"$(SOURCE)/test" \
		   -I"$(SOURCE)/uctest" \
		   -I"board" \
		   -I"CMSIS" \
		   -I"drivers" \
		   -I"startup" \
		   
# ARM Object Files 
ARM_OBJS := \
		   $(DEBUG)/source/led_control/led_control.o \
		   $(DEBUG)/source/circular_buffer/circular_buffer.o \
		   $(DEBUG)/source/logger/logger.o \
		   $(DEBUG)/source/logger/errno.o \
		   $(DEBUG)/source/logger/timestamp.o \
		   $(DEBUG)/source/uart/uart.o \
		   $(DEBUG)/source/uctest/System.o \
		   $(DEBUG)/startup/startup_mkl25z4.o \
		   $(DEBUG)/CMSIS/system_MKL25Z4.o \
		   $(DEBUG)/board/board.o \
		   $(DEBUG)/board/clock_config.o \
		   $(DEBUG)/board/peripherals.o \
		   $(DEBUG)/board/pin_mux.o \
		   $(DEBUG)/drivers/fsl_clock.o \
		   $(DEBUG)/drivers/fsl_common.o \
		   $(DEBUG)/drivers/fsl_flash.o \
		   $(DEBUG)/drivers/fsl_gpio.o \
		   $(DEBUG)/drivers/fsl_lpsci.o \
		   $(DEBUG)/drivers/fsl_smc.o \
	
# ARM Dependencies Files	   
ARM_DEPS := \
		   $(DEBUG)/source/led_control/led_control.d \
		   $(DEBUG)/source/application/application.d \
		   $(DEBUG)/source/circular_buffer/circular_buffer.d \
		   $(DEBUG)/source/logger/logger.d \
		   $(DEBUG)/source/logger/errno.d \
		   $(DEBUG)/source/logger/timestamp.d \
		   $(DEBUG)/source/uart/uart.d \
		   $(DEBUG)/source/uctest/System.d \
		   $(DEBUG)/startup/startup_mkl25z4.d \
		   $(DEBUG)/CMSIS/system_MKL25Z4.d \
		   $(DEBUG)/board/board.d \
		   $(DEBUG)/board/clock_config.d \
		   $(DEBUG)/board/peripherals.d \
		   $(DEBUG)/board/pin_mux.d \
		   $(DEBUG)/drivers/fsl_clock.d \
		   $(DEBUG)/drivers/fsl_common.d \
		   $(DEBUG)/drivers/fsl_flash.d \
		   $(DEBUG)/drivers/fsl_gpio.d \
		   $(DEBUG)/drivers/fsl_lpsci.d \
		   $(DEBUG)/drivers/fsl_smc.d \
		   		   
# Executable file
EXE := $(DEBUG)/pes_project_5.axf

#####################################################################
# Build Rules
# Rules for making all
all : $(EXE)

#####################################################################
# Selecting Platform
ifeq ($(BUILD), TEST)
build_option := test
else ifeq ($(BUILD), LOG)
build_option := log
else ifeq ($(BUILD), RUN)
build_option := run
endif
#####################################################################

$(EXE) : $(build_option)
	
#####################################################################
# Rule for compiling tests
test : directories $(ARM_OBJS) $(SOURCE)/test/test.c
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) ./source/test/test.c -o $(DEBUG)/source/test/test.o
	@arm-none-eabi-gcc -nostdlib -Xlinker -Map="./Debug/pes_project_5.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m0plus -mthumb -T linkerfile.ld -o ./Debug/pes_project_5.axf $(ARM_OBJS) $(DEBUG)/source/test/test.o
	@echo "Testing Code Compiled"

#####################################################################
# Rule for compiling detailed debug log
log : directories $(ARM_OBJS) $(SOURCE)/main.c
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -DLOG $(SOURCE)/main.c -o $(DEBUG)/source/main.o
	@arm-none-eabi-gcc -nostdlib -Xlinker -Map="./Debug/pes_project_5.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m0plus -mthumb -T linkerfile.ld -o ./Debug/pes_project_5.axf $(ARM_OBJS) $(DEBUG)/source/main.o
	@echo "KL25Z with logging on"

#####################################################################
# Rule for compiling program with normal logging	
run : directories $(ARM_OBJS) $(SOURCE)/main.c
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -DRUN $(SOURCE)/main.c -o $(DEBUG)/source/main.o
	@arm-none-eabi-gcc -nostdlib -Xlinker -Map="./Debug/pes_project_5.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m0plus -mthumb -T linkerfile.ld -o ./Debug/pes_project_5.axf $(ARM_OBJS) $(DEBUG)/source/main.o
	@echo "KL25Z with logging off"

#####################################################################
# Essesntial Source Files
$(DEBUG)/source/application/%.o: ./source/application/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(DEBUG)/source/circular_buffer/%.o: ./source/circular_buffer/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	
$(DEBUG)/source/led_control/%.o: ./source/led_control/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(DEBUG)/source/logger/%.o: ./source/logger/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	
$(DEBUG)/source/uart/%.o: ./source/uart/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(DEBUG)/source/uctest/%.o: ./source/uctest/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

#####################################################################
# Essesntial ARM Object Files
$(DEBUG)/board/%.o: ./board/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(DEBUG)/CMSIS/%.o: ./CMSIS/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(DEBUG)/drivers/%.o: ./drivers/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	
$(DEBUG)/startup/%.o: ./startup/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	
$(DEBUG)/utilities/%.o: ./utilities/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	
#####################################################################
# Making directories
.PHONY : directories
directories :
	$(MK) \
	$(DEBUG) \
	$(DEBUG)/board \
	$(DEBUG)/CMSIS \
	$(DEBUG)/drivers \
	$(DEBUG)/startup \
	$(DEBUG)/utilities \
	$(DEBUG)/source/application \
	$(DEBUG)/source/circular_buffer \
	$(DEBUG)/source/led_control \
	$(DEBUG)/source/logger \
	$(DEBUG)/source/test \
	$(DEBUG)/source/uart \
	$(DEBUG)/source/uctest
	
# Clean target
.PHONY : clean
clean:
	@$(RM) \
	$(DEBUG)/board \
	$(DEBUG)/CMSIS \
	$(DEBUG)/drivers \
	$(DEBUG)/startup \
	$(DEBUG)/utilities \
	$(DEBUG)/source \
	$(DEBUG)/pes_project_5.axf \
	$(DEBUG)/pes_project_5.map
	@echo "Build cleaned"
	
```

---

### Source Files

**Logger**

1. logger.c
   
   ```c
   /**
     * File Name 		- logger.c
     * Description 	- contains logger subroutines
     * Author			- Atharva Nandanwar
     * Tools			- GNU C Compiler / ARM Compiler Toolchain
     * Leveraged Code 	- https://github.com/ntpeters/SimpleLogger/
     * URL				-
     */
   #include "logger.h"
   
   // Struct for storing logger data
   typedef struct {
   		log_level_t Logger_Log_Level;
   }logger_data;
   
   logger_data thisLogger;
   
   // Character codes for colors
   // Leveraged Code - https://stackoverflow.com/questions/3585846/color-text-in-terminal-applications-in-unix
   const char* red = "\x1B[31m";
   const char* green = "\x1B[32m";
   const char* blue = "\x1B[34m";
   const char* end = "\x1B[0m";
   
   /**
    * Init
    * initializes logger by initializing timestamp
    */
   void Init(void)
   {
   	vTimestamp_Init();
   }
   
   /*
    * Function - Log_Write
    * Brief - Prints a log message
    * Arguments -
    * function_name -> name of the calling function
    * message_type -> Error, Debug or Status message
    * msg, ... -> printf style argument to hold a string and format specifiers
    * Leveraged Code - https://www.ozzu.com/cpp-tutorials/tutorial-writing-custom-printf-wrapper-function-t89166.html
    */
   void Log_Write(const char* function_name, message_type_t message_type, const char *msg, ... )
   {
   	// To process variable argument list
   	va_list args;
   	va_start(args, msg);
   
   	// Activate color based on message type
   	switch(message_type)
   	{
   	case mError:
   		pprintf("%s", red);
   		break;
   	case mDebug:
   		pprintf("%s", blue);
   		break;
   	case mStatus:
   		pprintf("%s", green);
   		break;
   	}
   
   	// Timestamp related routine
   	timestamp_t currentTime = tTimestamp_Get_Timestamp();
   	pprintf("[%02d:%02d:%02d.%d]", currentTime.hours, \
   			currentTime.minutes, currentTime.seconds, \
   			currentTime.deciseconds);
   
   	// Log Level Logic
   	switch(thisLogger.Logger_Log_Level)
   	{
   	case lTest:
   		pprintf("Test:  ");
   		break;
   	case lDebug:
   		pprintf("Debug: ");
   		break;
   	case lNormal:
   		pprintf("Run:   ");
   		break;
   	}
   
   	// Printing function names
   	pprintf("%-27s:\t", function_name);
   
   
   	// Message print with color termination code
   	vpprintf(msg, args);
   	pprintf("%s\n\r", end);
   }
   
   /*
    * Function - Get_Log_Level
    * Brief - returns the current log level
    * Return -
    * returns log_level_t enum value
    */
   log_level_t Get_Log_Level (void)
   {
   	return thisLogger.Logger_Log_Level;
   }
   
   /*
    * Function - Set_Log_Level
    * Brief - sets the current log level
    * Arguments -
    * log_level_t enum value
    */
   void Set_Log_Level (log_level_t level)
   {
   	thisLogger.Logger_Log_Level = level;
   }
   
   // Declaration for logger struct
   logger_instance const logger = {Init, Log_Write, Set_Log_Level, Get_Log_Level};
   ```

2. logger.h
   
   ```c
   /**
     * File Name 		- logger.h
     * Description 	- header file for logger.c
     * Author			- Atharva Nandanwar
     * Tools			- GNU C Compiler / ARM Compiler Toolchain
     * Leveraged Code 	-
     * URL				-
     */
   
   #ifndef LOGGER_LOGGER_H_
   #define LOGGER_LOGGER_H_
   
   // Include Files
   #include <stdint.h>
   #include <errno.h>
   #include <stdarg.h>
   #include "timestamp.h"
   #include "uart.h"
   
   // Log Level and Message Type enums
   typedef enum {lTest, lDebug, lNormal} log_level_t;
   typedef enum {mError, mDebug, mStatus} message_type_t;
   
   // Logger Instance struct
   typedef struct {
   	void ( * const Init )( void );
   	void ( * const Log_Write )( const char* function_name, \
   			message_type_t message_type, const char *msg, ... );
   	void ( * const Set_Log_Level )( log_level_t level );
   	log_level_t ( * const Get_Log_Level )( void );
   }logger_instance;
   
   extern logger_instance const logger;
   #endif /* LOGGER_LOGGER_H_ */
   ```

3. timestamp.c
   
   ```c
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
   ```

4. timestamp.h
   
   ```c
   /**
    * File - timestamp.h
    * Author - Atharva Nandanwar
    * Email - atharva.nandanwar@colorado.edu
    * Principles of Embedded Software
    * University of Colorado Boulder
    */
   #ifndef LOGGER_TIMESTAMP_H_
   #define LOGGER_TIMESTAMP_H_
   // Include files
   #include "MKL25Z4.h"
   
   // Struct for timestamp information
   typedef struct {
   	uint8_t hours;
   	uint8_t minutes;
   	uint8_t seconds;
   	uint8_t deciseconds;
   } timestamp_t;
   
   // Prototype Functions
   void vTimestamp_Init(void);
   timestamp_t tTimestamp_Get_Timestamp(void);
   #endif /* LOGGER_TIMESTAMP_H_ */
   ```

5. errno.c
   
   ```c
   
   ```

6. errno.h
   
   ```c
   
   ```

**Circular Buffer**

1. circular_buffer.c
   
   ```c
   /**
    * File - circular_buffer.c
    * Author - Atharva Nandanwar
    * Email - atharva.nandanwar@colorado.edu
    * Principles of Embedded Software
    * University of Colorado Boulder
    */
   
   #include "circular_buffer.h"
   
   /**
    * cb_init_buffer
    * Creates a circular buffer
    * @param
    * 		length - size of circular buffer
    * @return
    * 		pointer to circular buffer
    */
   circular_buffer_t* cb_init_buffer(uint16_t length)
   {
   	// Allocate memory for buffer structure, and memory for buffer
   	circular_buffer_t* buffer_pointer = NULL;
   	buffer_pointer = (circular_buffer_t *) malloc(sizeof(circular_buffer_t));
   	buffer_pointer->pointer = (uint8_t *) malloc(length);
   
   	// Set all the parameters
   	buffer_pointer->head = buffer_pointer->pointer;
   	buffer_pointer->tail = buffer_pointer->pointer;
   	buffer_pointer->count = 0;
   	buffer_pointer->length = length;
   	return buffer_pointer;
   }
   
   /**
    * cb_destroy_buffer
    * Destroys the circular buffer
    * @param
    * 		buffer - pointer to circular buffer
    * @return
    * 		status of operation
    */
   CB_status_t cb_destroy_buffer(circular_buffer_t* buffer)
   {
   	// Free the memory for buffer, and buffer structure
   	free(buffer->pointer);
   	buffer->pointer = NULL;
   	free(buffer);
   	buffer = NULL;
   	return CB_buffer_destroyed;
   }
   
   /**
    * cb_check_full
    * Checks if buffer is full
    * @param
    * 		buffer - pointer to circular buffer
    * @return
    * 		status of operation
    */
   CB_status_t cb_check_full(circular_buffer_t* buffer)
   {
   	// Flag error
   	if(buffer == NULL)
   	{
   		return CB_buffer_error;
   	}
   
   	// Check full
   	if(buffer->count == buffer->length)
   	{
   		return CB_buffer_full;
   	}
   	else
   	{
   		return CB_buffer_not_full;
   	}
   }
   
   /**
    * cb_check_empty
    * Checks if buffer is empty
    * @param
    * 		buffer - pointer to circular buffer
    * @return
    * 		status of operation
    */
   CB_status_t cb_check_empty(circular_buffer_t* buffer)
   {
   	// Flag error
   	if(buffer == NULL)
   	{
   		return CB_buffer_error;
   	}
   
   	// Check empty
   	if(buffer->count == 0)
   	{
   		return CB_buffer_empty;
   	}
   	else
   	{
   		return CB_buffer_not_empty;
   	}
   }
   
   /**
    * cb_add_item
    * Checks if buffer is full, and adds item if not full
    * @param
    * 		buffer - pointer to circular buffer
    * @param
    * 		item - data to be added into circular buffer
    * @return
    * 		status of operation
    */
   CB_status_t cb_add_item(circular_buffer_t* buffer, uint8_t item)
   {
   	// Flag error
   	if(buffer == NULL)
   	{
   		return CB_buffer_error;
   	}
   
   	// If not full, then update parameters
   	if(cb_check_full(buffer) == CB_buffer_full)
   	{
   		return CB_buffer_full;
   	}
   	else
   	{
   		*(buffer->head) = item;
   		buffer->head += 1;
   		buffer->head = (uint32_t) (buffer->head - buffer->pointer) % buffer->length + buffer->pointer;
   		buffer->count += 1;
   	}
   	return CB_buffer_operation_success;
   }
   
   
   /**
    * cb_remove_item
    * Checks if circular buffer is empty, and removes the item
    * @param
    * 		buffer - pointer to circular buffer
    * @return
    * 		status of operation
    */
   CB_status_t cb_remove_item(circular_buffer_t* buffer)
   {
   	// Flag error
   	if(buffer == NULL)
   	{
   		return CB_buffer_error;
   	}
   
   	// If not empty, then update parameters
   	if(cb_check_empty(buffer) == CB_buffer_empty)
   	{
   		return CB_buffer_empty;
   	}
   	else
   	{
   		buffer->tail += 1;
   		buffer->tail = (uint32_t) (buffer->tail - buffer->pointer) % buffer->length + buffer->pointer;
   		buffer->count -= 1;
   	}
   	return CB_buffer_operation_success;
   }
   
   /**
    * cb_verify_init
    * Verifies buffer initialization
    * @param
    * 		buffer - pointer to circular buffer
    * @return
    * 		status of operation
    */
   CB_status_t cb_verify_init(circular_buffer_t* buffer)
   {
   	// Flag error
   	if(buffer == NULL)
   	{
   		return CB_buffer_error;
   	}
   
   	// Checks for initialization error
   	if(buffer->pointer == NULL)
   	{
   		return CB_buffer_error_init;
   	}
   	else if(buffer->head != buffer->pointer || buffer->tail != buffer->pointer)
   	{
   		return CB_buffer_error_init;
   	}
   	else if(buffer->count != 0)
   	{
   		return CB_buffer_error_init;
   	}
   	return CB_buffer_initialized;
   }
   ```

2. circular_buffer.h
   
   ```c
   /**
    * File - circular_buffer.h
    * Author - Atharva Nandanwar
    * Email - atharva.nandanwar@colorado.edu
    * Principles of Embedded Software
    * University of Colorado Boulder
    */
   #ifndef CIRCULAR_BUFFER_CIRCULAR_BUFFER_H_
   #define CIRCULAR_BUFFER_CIRCULAR_BUFFER_H_
   
   // Include files
   #include <stdint.h>
   #include <stdlib.h>
   #include "MKL25Z4.h"
   #include "common.h"
   
   // Enum for status
   typedef enum {
   	CB_buffer_full,
   	CB_buffer_not_full,
   	CB_buffer_empty,
   	CB_buffer_not_empty,
   	CB_buffer_initialized,
   	CB_buffer_error_init,
   	CB_buffer_destroyed,
   	CB_buffer_error,
   	CB_buffer_operation_success,
   } CB_status_t;
   
   // Structure for circular buffer
   typedef struct {
   	uint8_t* pointer;
   	uint8_t* head;
   	uint8_t* tail;
   	uint16_t length;
   	uint16_t count;
   } circular_buffer_t;
   
   // Prototype functions
   CB_status_t cb_add_item(circular_buffer_t* buffer, uint8_t item);
   CB_status_t cb_remove_item(circular_buffer_t* buffer);
   CB_status_t cb_check_full(circular_buffer_t* buffer);
   CB_status_t cb_check_empty(circular_buffer_t* buffer);
   CB_status_t cb_verify_init(circular_buffer_t* buffer);
   circular_buffer_t* cb_init_buffer(uint16_t length);
   CB_status_t cb_destroy_buffer(circular_buffer_t* buffer);
   
   #endif /* CIRCULAR_BUFFER_CIRCULAR_BUFFER_H_ */
   ```

**UART Drivers**

1. uart.c
   
   ```c
   /**
     * File Name 		- test.c
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
   
   /**
    * uart_enable_irq
    * Enable UART Interrupts
    */
   void uart_enable_irq(void)
   {
   	// Enable Interrupts
   	UART0->C2 |= UART_C2_TIE(1) | UART_C2_RIE(1);
   
       // Set up C3 register for error based interrupts
       UART0->C3 |= UART_C3_ORIE(1) | UART_C3_NEIE(1) | UART_C3_FEIE(1) | UART_C3_PEIE(1);
   
       // Set up NVIC registers
   	NVIC->ICPR[0] |= 1 << (UART0_IRQn);
   	NVIC->ISER[0] |= 1 << (UART0_IRQn);
   }
   
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
   void uart_tx(uint8_t data)
   {
   	while(uart_tx_available() != TX_available);
   	uart_tx_action(data);
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
   uint8_t uart_rx(void)
   {
   	while(uart_rx_check() != RX_available);
   	return uart_rx_action();
   }
   
   /**
    * uart_echo
    * UART Echo function
    */
   void uart_echo(void)
   {
   	uint8_t temp = uart_rx();
   	uart_tx(temp);
   }
   
   /**
    * UART0_IRQHandler
    * UART Interrupt Service Routine
    */
   void UART0_IRQHandler(void)
   {
   	if((UART0->S1 & UART0_S1_RDRF_MASK)  && (system_info.rx_ready_flag == 0))
   	{
   		system_info.rx_ready_flag = 1;
   	}
   	else if((UART0->S1 & UART0_S1_TDRE_MASK) && (system_info.tx_ready_flag == 0))
   	{
   		system_info.tx_ready_flag = 1;
   	}
   }
   
   /**
    * uart_putchar
    * Sends a character to UART
    * @param
    * 		ch - 8 bit character
    */
   void uart_putchar(uint8_t ch)
   {
   	uart_tx(ch);
   }
   
   /**
    * uart_getchar
    * Gets a character from UART
    * @return
    * 		return 8 bit data
    */
   uint8_t uart_getchar (void)
   {
       return uart_rx();
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
   ```

2. uart.h
   
   ```c
   /*
    * uart.h
    *
    *  Created on: Nov 12, 2019
    *      Author: akshh
    */
   
   #ifndef UART_H_
   #define UART_H_
   // Include Files
   #include <stdio.h>
   #include <stdlib.h>
   #include <stdint.h>
   #include <stdarg.h>
   #include "MKL25Z4.h"
   #include "common.h"
   
   // Macros
   #define SYS_CLOCK	48000000UL
   
   // Enum for Baud Rate selection
   typedef enum {
   	baud_115200 = 115200,
   	baud_57600 = 57600,
   	baud_38400 = 38400,
   	baud_19200 = 19200,
   	baud_14400 = 14400,
   	baud_9600 = 9600,
   } UARTBaudRate_t;
   
   // Enum for Parity Setup
   typedef enum {
   	parity_off = 0x0,
   	parity_even = 0x2,
   	parity_odd = 0x3,
   } UARTParityMode_t;
   
   // Enum for Stop Bit Configuration
   typedef enum {
   	single_stop_bit = 0x0,
   	double_stop_bit = 0x1,
   } UARTStopBit_t;
   
   // Enum for Oversampling Rate
   typedef enum {
   	OSR_4 = 0x03,
   	OSR_5,
   	OSR_6,
   	OSR_7,
   	OSR_8,
   	OSR_9,
   	OSR_10,
   	OSR_11,
   	OSR_12,
   	OSR_13,
   	OSR_14,
   	OSR_15,
   	OSR_16,
   	OSR_17,
   	OSR_18,
   	OSR_19,
   	OSR_20,
   	OSR_21,
   	OSR_22,
   	OSR_23,
   	OSR_24,
   	OSR_25,
   	OSR_26,
   	OSR_27,
   	OSR_28,
   	OSR_29,
   	OSR_30,
   	OSR_31,
   	OSR_32,
   
   } UARTOSRSetting_t;
   
   // Structure for UART Configuration
   typedef struct {
   	UARTBaudRate_t baud_rate;
   	UARTParityMode_t parity;
   	UARTStopBit_t stop_bit;
   	UARTOSRSetting_t osr;
   } UARTConfig_t;
   
   // Enum for UART Status
   typedef enum {
   	TX_available,
   	TX_not_available,
   	RX_available,
   	RX_not_available,
   } UARTStatus_t;
   
   // Prototype Functions
   void uart_init(UARTConfig_t* uart_config);
   UARTStatus_t uart_tx_available(void);
   void uart_tx_action(uint8_t data);
   void uart_tx(uint8_t data);
   UARTStatus_t uart_rx_check(void);
   uint8_t uart_rx_action(void);
   uint8_t uart_rx(void);
   void uart_enable_irq(void);
   
   // Prototype Functions for Print operations
   uint8_t uart_getchar(void);
   void uart_putchar(uint8_t ch);
   void vpprintf(const char* fmt, va_list args);
   void pprintf(const char* fmt, ...);
   
   #endif /* UART_H_ */
   ```

**LED Control**

1. led_control.c
   
   ```c
   /**
    * File - led_control.c
    * Author - Atharva Nandanwar
    * Email - atharva.nandanwar@colorado.edu
    * Principles of Embedded Software
    * University of Colorado Boulder
    */
   #include "led_control.h"
   
   /**
    * LED_Init
    * Initialization of LEDs
    */
   void LED_Init(void)
   {
   	// Set up clock for Port peripheral
   	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;
   
   	// Set mux for each port pins
   	PORTB->PCR[18] = PORT_PCR_MUX(1);
   	PORTB->PCR[19] = PORT_PCR_MUX(1);
   	PORTD->PCR[1] = PORT_PCR_MUX(1);
   
   	// Set data directions, and turn the LEDs off
   	GPIOB->PDDR |= RED_LED | GREEN_LED;
   	GPIOD->PDDR |= BLUE_LED;
   	Turn_Off_LEDs();
   }
   
   /**
    * Turn_On_Only_LED
    * Turns on only specified color LED
    * @param
    * 		LED - color of LED
    */
   void Turn_On_Only_LED(led_color_t LED)
   {
   	switch(LED)
   	{
   	case Red:
   		GPIOB->PCOR |= RED_LED;			// Turn On
   		GPIOB->PSOR |= GREEN_LED;		// Turn Off
   		GPIOD->PSOR |= BLUE_LED;		// Turn Off
   		break;
   	case Green:
   		GPIOB->PCOR |= GREEN_LED;		// Turn On
   		GPIOB->PSOR |= RED_LED;			// Turn Off
   		GPIOD->PSOR |= BLUE_LED;		// Turn Off
   		break;
   	case Blue:
   		GPIOD->PCOR |= BLUE_LED;		// Turn On
   		GPIOB->PSOR |= RED_LED;			// Turn Off
   		GPIOB->PSOR |= GREEN_LED;		// Turn Off
   		break;
   	}
   }
   
   /**
    * Turn_Off_LEDs
    * turns off all LEDs
    */
   void Turn_Off_LEDs(void)
   {
   	GPIOD->PSOR |= (0x1 << 1U);			// Turn Off
   	GPIOB->PSOR |= (0x1 << 18U);		// Turn Off
   	GPIOB->PSOR |= (0x1 << 19U);		// Turn Off
   }
   ```

2. led_control.h
   
   ```c
   /**
    * File - led_control.h
    * Author - Atharva Nandanwar
    * Email - atharva.nandanwar@colorado.edu
    * Principles of Embedded Software
    * University of Colorado Boulder
    */
   #ifndef LED_CONTROL_LED_CONTROL_H_
   #define LED_CONTROL_LED_CONTROL_H_
   
   // Include files
   #include <stdint.h>
   #include "MKL25Z4.h"
   
   // Macros
   #define RED_LED		(0x1 << 18U)
   #define GREEN_LED	(0x1 << 19U)
   #define BLUE_LED	(0x1 << 1U)
   
   // Enum for LED Color
   typedef enum {
   	Red,
   	Green,
   	Blue,
   } led_color_t;
   
   // Prototype functions
   void LED_Init(void);
   void Turn_On_Only_LED(led_color_t LED);
   void Turn_Off_LEDs(void);
   
   #endif /* LED_CONTROL_LED_CONTROL_H_ */
   ```

**Test**

1. test.c
   
   ```c
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
   
   	UCUNIT_TestcaseBegin("Test Case for Circular Buffer Add Item\n\r");
   	cb_add_item(buff1, 2);
   	UCUNIT_CheckIsEqual(100, buff1->length);
   	UCUNIT_CheckIsEqual(1, buff1->count);
   	UCUNIT_CheckIsEqual(buff1->pointer + 1, buff1->head);
   	UCUNIT_CheckIsEqual(buff1->pointer, buff1->tail);
   	printf("Data is %d\n\r", *buff1->tail);
   	UCUNIT_TestcaseEnd();
   
   	UCUNIT_TestcaseBegin("Test Case for Circular Buffer Remove Item\n\r");
   	cb_remove_item(buff1);
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
   		cb_remove_item(buff1);
   	}
   	UCUNIT_TestcaseBegin("Test Case for Circular Buffer Empty\n\r");
   	UCUNIT_CheckIsEqual(0, buff1->count);
   	UCUNIT_CheckIsEqual(CB_buffer_empty, cb_remove_item(buff1));
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
   ```

2. test.h
   
   ```c
   /**
     * File Name 		- test.h
     * Description 	- header file for test.c
     * Author			- Atharva Nandanwar
     * Tools			- GNU C Compiler / ARM Compiler Toolchain
     * Leveraged Code 	-
     * URL				-
     */
   
   #ifndef TEST_TEST_H_
   #define TEST_TEST_H_
   
   // Include Files
   #include <stdint.h>
   
   #include "pin_mux.h"
   #include "peripherals.h"
   #include "clock_config.h"
   #include "board.h"
   
   // Includes for test functions
   #include "System.h"
   #include "uCUnit.h"
   
   // Includes for functions
   #include "logger.h"
   #include "errno.h"
   #include "uart.h"
   #include "circular_buffer.h"
   #include "led_control.h"
   #include "common.h"
   
   #endif /* TEST_TEST_H_ */
   ```

**Main Subroutine**

1. common.h
   
   ```c
   /**
     * File Name 		- common.h
     * Description 	- file commonly to be included across system
     * Author			- Atharva Nandanwar
     * Tools			- GNU C Compiler / ARM Compiler Toolchain
     * Leveraged Code 	-
     * URL				-
     */
   
   #ifndef COMMON_H_
   #define COMMON_H_
   // Include Files
   #include <stdint.h>
   #include "MKL25Z4.h"
   
   // Macros for Critical Section
   #define START_CRITICAL() 	UART0->C2 &= ~(UART_C2_TIE_MASK | UART_C2_RIE_MASK)
   #define END_CRITICAL() 		UART0->C2 |= UART_C2_TIE(1) | UART_C2_RIE(1)
   
   // Structure to hold system status
   typedef struct {
   	uint8_t tx_ready_flag;
   	uint8_t tx_data;
   	uint8_t rx_ready_flag;
   	uint8_t rx_data;
   } system_t;
   
   // Macros for system info
   #define TX_FLAG_RESET	0
   #define RX_FLAG_RESET	0
   #define TX_DATA_RESET	0
   #define RX_DATA_RESET	0
   
   // Extern for system_info global variable
   extern system_t system_info;
   
   #endif /* COMMON_H_ */
   ```

2. main.c
   
   ```c
   /*
    * File - main.c
    * Brief -
    * Author - Atharva Nandanwar
    * University of Colorado Boulder
    * Principles of Embedded Software
    */
   
   #include "main.h"
   
   system_t system_info = {TX_FLAG_RESET, TX_DATA_RESET, RX_FLAG_RESET, RX_DATA_RESET};
   
   int main(void)
   {
   
   }
   ```

3. main.h
   
   ```c
   
   ```
