//
//  pindefinitions.h
//  Will_keyboard
//

#ifndef pindefinitions_h
#define pindefinitions_h

//Pin definitions so it's easier for rewiring later on

//Debug LED
#define DEBUG_LED_DDR DDRB
#define DEBUG_LED_PORT PORTB

#define DEBUG_LED1 PB1 //It's really the same LED, but it's tri-colored
#define DEBUG_LED2 PB5
#define DEBUG_LED3 PB6

//ADB
#define ADB_DDR DDRB
#define ADB_PORT PORTB
#define ADB_DATA_PIN PB0
#define ADB_POWER_ON PB2 //In case I need this later

//PS2
#define PS2_DDR DDRD
#define PS2_PORT PORTD
#define PS2_PIN_DATA PD6 
#define PS2_PIN_CLOCK PD7

#define PS2_GND_PIN_DDR DDRB
#define PS2_GND_PIN_PORT PORTB
#define PS2_GND_PIN_DATA PB3//pull-down resistors for open drain collector
#define PS2_GND_PIN_CLOCK PB4


#endif /* pindefinitions_h */
