//
//  ps2_encode.c
//  Index
//

#include "ps2_encode.h"
#include "scan_codes.h"


static uint8_t check_parity(uint8_t *data) {
    uint8_t count = 0;
    for (int i = 0; i <= 8; i++) {
        if (data[i] == 1) {
            count++;
        }
    }
    if(count%2 == 0) {
        return 1;
    } else {
        return 0;
    }
}


void send_ps2_command(uint8_t *data) {
    
    //put data into ps2 frame
    struct ps2_frame new_command;
    
    new_command.data_buffer = data;
    new_command.parity = check_parity(data);
    new_command.stop = 1;
    
    //initiate change of state in data and clock line
    PS2_GND_PIN_PORT |= (1 << PS2_GND_PIN_CLOCK); //Pulls clock low
    _delay_us(100);
    PS2_GND_PIN_PORT |= (1 << PS2_GND_PIN_DATA); //Pulls data low
    PS2_GND_PIN_PORT &= ~(1 << PS2_GND_PIN_CLOCK); //release clock
    
    loop_until_bit_is_clear(PIND, PS2_PIN_CLOCK); //Wait until device pulls clock low
    
    //send the command
    for(int i = 0; i < 8; i++) {
        PS2_PORT |= (new_command.data_buffer[i] << PS2_PIN_DATA);
        //Then it waits for the device to do stuff
        loop_until_bit_is_set(PIND, PS2_PIN_CLOCK);
        loop_until_bit_is_clear(PIND, PS2_PIN_CLOCK);
    }
    //send the parity bit
    PS2_PORT |= (new_command.parity << PS2_PIN_DATA);
    loop_until_bit_is_set(PIND, PS2_PIN_CLOCK);
    loop_until_bit_is_clear(PIND, PS2_PIN_CLOCK);
    
    //send the stop bit
    PS2_PORT |= (new_command.stop << PS2_PIN_DATA);
    
    //wait for acknowledge bit from device
    loop_until_bit_is_clear(PIND, PS2_PIN_DATA);
    
    /* Things to consider
     * Device needs to generate clock frequency 10-16.7kHz
     * Clock is high and low only for 30-50us at a time
     * Any data sampling needs to happen in the middle of each rise and fall
     * Middle is 5us from rising edge and 5us from falling edge
     */
}


void receive_ps2_code(void) {
    //It will immediately re-transmit the code it has received back out to the ADB
    if(bit_is_clear(PIND, PS2_GND_PIN_CLOCK) && bit_is_clear(PIND, PS2_PIN_DATA)) {
        _delay_us(5);
        //The time from the rising edge of a clock pulse to a Data transition
        //must be at least 5 microseconds
        DEBUG_LED_DDR |= (1 << DEBUG_LED1);
        
        for(int i = 0; i < 8; i++) {
            if(bit_is_set(PIND, PS2_PIN_DATA)) {
                data_buffer_temp_1[i] = 1;
            } else if (bit_is_clear(PIND, PS2_PIN_DATA)) {
                data_buffer_temp_1[i] = 0;
            }
        }
        loop_until_bit_is_set(PIND, PS2_PIN_DATA); //Parity
        loop_until_bit_is_set(PIND, PS2_PIN_DATA); //Stop
        DEBUG_LED_DDR &= ~(1 << DEBUG_LED1);
    }
    
    //TODO: needs to collect the bits into one number from the data buffer
    convert_to_full_byte_ps2();
}


uint8_t convert_scan_code(void) {
    
    static uint8_t state = 0;
    uint8_t s = data_buffer_ps2[index_ps2];
    char c;
    
        if (s == 0xF0) {
            state |= BREAK;
        } else if (s == 0xE0) {
            state |= MODIFIER;
        } else {
            if (state & BREAK) {
                if (s == 0x12) {
                    state &= ~SHIFT_L;
                } else if (s == 0x59) {
                    state &= ~SHIFT_R;
                } else if (s == 0x11 && (state & MODIFIER)) {
                    state &= ~ALTGR;
                }
                // CTRL, ALT & WIN keys could be added
                // but is that really worth the overhead?
                state &= ~(BREAK | MODIFIER);
            }
            if (s == 0x12) {
                state |= SHIFT_L;
            } else if (s == 0x59) {
                state |= SHIFT_R;
            } else if (s == 0x11 && (state & MODIFIER)) {
                state |= ALTGR;
            }
            c = 0;
            if (state & MODIFIER) {
                switch (s) {
                    case 0x70: c = PS2_INSERT;      break;
                    case 0x6C: c = PS2_HOME;        break;
                    case 0x7D: c = PS2_PAGEUP;      break;
                    case 0x71: c = PS2_DELETE;      break;
                    case 0x69: c = PS2_END;         break;
                    case 0x7A: c = PS2_PAGEDOWN;    break;
                    case 0x75: c = PS2_UPARROW;     break;
                    case 0x6B: c = PS2_LEFTARROW;   break;
                    case 0x72: c = PS2_DOWNARROW;   break;
                    case 0x74: c = PS2_RIGHTARROW;  break;
                    case 0x4A: c = '/';             break;
                    case 0x5A: c = PS2_ENTER;       break;
                    default: break;
                }
            }
        }
    
    return c;

}

void convert_to_full_byte_ps2(void) {
    
    uint8_t decimal = 0;
    
    for(int i = 0; i < 8; i++) {
        decimal = decimal << 1 | data_buffer_temp_1[i];
    }
    
    data_buffer_ps2[index_ps2] = decimal;
    index_ps2++;
    
    if(index_ps2 == MAX_BUFFER_SIZE) {
        index_ps2 = 0;
    }
    char letter = convert_scan_code();
}
