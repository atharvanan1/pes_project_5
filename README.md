# Principles of Embedded Software Project 5

### README

**Name: Atharva Nandanwar**

This repository contains source files for Principles of Embedded Software Project 5 - UART and Circular Buffer

---

**Source Folder:**

1. main.c/h - main subroutine

2. circular_buffer/circular_buffer.c/h - functions and structure definition for circular buffer

3. led_control/led_control.c/h - functions to control LED

4. logger/logger.c/h - functions to do logging

5. logger/errno.c/h - error handling routines

6. logger/timestamp.c/h - timestamp functionality

7. test/test.c/h - test cases and test subroutine

8. uart/uart.c/h - uart subroutines, initialization, and operation drivers

9. uctest - uCunit testing files

10. common.h - common include file for system-wide implementation

---

**Observations:**

1. There was serious problems with how integration would work in this aspect. How do I interface interrupt service routines with circular buffer operations? How do I interface UART functions with circular buffer? How do I manage getchar and putchar functionality with everything in the system? *Making global buffers was the way to do

2. The way I had designed logger required me to implement a lot of complicated printf operations. I rather used sprintf to format data into a string and printing the string directly.

3. During Interrupt based Application Mode, the program switches to Overrun condition, but it doesn't raise the interrupt. I did not address this problem in this program.

---

**Installation/Execution Notes:**

Use basic options to compile RUN, LOG, and TEST builds from Build Targets.

Compiler - gcc-arm-none-eabi

There are different functionalities that can be configured in RUN and LOG mode:

1. UART Non-blocking Echo Mode        - #define ECHO_POLLING

2. UART Non-blocking Application Mode    - #define APP_POLLING

3. UART Interrupts Echo Mode        - #define ECHO_IRQN

4. UART Interrupts Application Mode    - #define APP_IRQN

You will have to go to common.h to edit different modes. Comment out the mode you want, and comment others.

---

**Note:**

In application mode, report is printed after every 50th character is received.
