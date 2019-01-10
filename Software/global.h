
//
//  global.h
//  Will_keyboard


#ifndef global_h
#define global_h

#define _NOP() do { __asm__ __volatile__ ("nop"); } while (0)
// used for a very short delay

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "pindefinitions.h"

#define MAX_BUFFER_SIZE_TEMP 8

#define MAX_BUFFER_SIZE 256

// Indexes for the 2 arrays holding the data received and transmitted
// Automatically get reset if they overflow in code
static uint8_t index_adb = 0;

static uint8_t index_ps2 = 0;

// Temporary scan code(PS2)/data packet(ADB)  buffers
static uint8_t data_buffer_temp_1[MAX_BUFFER_SIZE_TEMP];

static uint8_t data_buffer_temp_2[MAX_BUFFER_SIZE_TEMP];

// Contain MAX_BUFFER_SIZE of data received
static uint8_t data_buffer_ps2[MAX_BUFFER_SIZE];

static uint8_t data_buffer_adb[MAX_BUFFER_SIZE];

#endif /* global_h */
