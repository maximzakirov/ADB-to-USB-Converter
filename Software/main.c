

#include "global.h"

#include "ps2_encode.h"
#include "adb_encode.h"

//ADB to PS2 converter

void initIO(void) {
    //Setting pin modes in data direction register
    //Set to output
    
    ADB_DDR |= (1 << ADB_DATA_PIN);
    ADB_DDR |= (1 << ADB_POWER_ON); //just in case
    
    PS2_DDR |= (1 << PS2_PIN_DATA);
    PS2_DDR |= (1 << PS2_PIN_CLOCK);
    PS2_GND_PIN_DDR |= (1 << PS2_GND_PIN_DATA);
    PS2_GND_PIN_DDR |= (1 << PS2_GND_PIN_CLOCK);
    
    //LED ports
    DEBUG_LED_DDR |= (1 << DEBUG_LED1);
    DEBUG_LED_DDR |= (1 << DEBUG_LED2);
    DEBUG_LED_DDR |= (1 << DEBUG_LED3);
    
    _NOP(); //for synchronization.
}

void init_ps2(void) {
    //TODO: let coverter know if it's the host or the device. Currently configured to be the host.
    
    //initiate data and clock line in "idle" status
    PS2_PORT |= (1 << PS2_PIN_CLOCK);
    PS2_PORT |= (1 << PS2_PIN_DATA);
    
    //Make sure the open drain collector pins are off
    PS2_GND_PIN_PORT &= ~(1 << PS2_GND_PIN_DATA);
    PS2_GND_PIN_PORT &= ~(1 << PS2_GND_PIN_CLOCK);
    
}

void init_adb(void) {
    //TODO: let converter know if it's the host or the device. Currently it is the device for the mac, but there is code for it to send information the way that a mac computer would.
    
    //These need to be on so that when the Mac turns on,
    ADB_PORT |= (1 << ADB_POWER_ON);
    ADB_PORT |= (1 << ADB_DATA_PIN);
   
    if(bit_is_clear(PIND, ADB_DATA_PIN)) {
        _delay_ms(3); //On startup, the mac will go through reset sequence where it pulls down line for 3 ms.
    }
    
}

int main(void) {
    initIO();
    init_ps2();
    init_adb();
    
	while (1) {
        receive_ps2_code(); //retransmits to adb
        receive_adb_code(); //retransmits to ps2
    }
    
	return 0; // never reached
}

/* PS2 frame */
/*  12 bits/11 bits (depends on if it's the host/device)
 *  Start bit (usually a pull-down of data line)
 *  8 bits of data
 *  Parity bit (0 or 1 if even/odd)
 *  Stop bit (1 sent out)
 *  Acknowledge (pull down at the end)
 */

/* ADB frame */
/* 800us Attention
 * 70us Sync
 * 100x8us Data Bit Cells
 * Stop Bit Cell
 * (potential data return based on last 2 bits of data)
 */
