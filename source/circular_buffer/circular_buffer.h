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
CB_status_t cb_remove_item(circular_buffer_t* buffer, uint8_t* data);
CB_status_t cb_check_full(circular_buffer_t* buffer);
CB_status_t cb_check_empty(circular_buffer_t* buffer);
CB_status_t cb_verify_init(circular_buffer_t* buffer);
circular_buffer_t* cb_init_buffer(uint16_t length);
CB_status_t cb_destroy_buffer(circular_buffer_t* buffer);

#endif /* CIRCULAR_BUFFER_CIRCULAR_BUFFER_H_ */
