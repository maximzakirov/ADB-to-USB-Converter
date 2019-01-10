//
//  adb_encode.c
//  Index
//

#include "global.h"
#include "adb_encode.h"

void bit_cell_one(void) {
    ADB_PORT &= ~(1 << ADB_DATA_PIN);
    _delay_us(35);
    ADB_PORT |= (1 << ADB_DATA_PIN);
    _delay_us(65);
}

void bit_cell_zero(void) {
    ADB_PORT &= ~(1 << ADB_DATA_PIN);
    _delay_us(65);
    ADB_PORT |= (1 << ADB_DATA_PIN);
    _delay_us(35);
    
}


void send_adb_command(uint8_t *data) {
    
    ADB_PORT &= ~(1 << ADB_DATA_PIN); //Attention signal
    _delay_us(800);
    
    ADB_PORT |= (1 << ADB_DATA_PIN); //Sync signal
    _delay_us(70);
    
    for(int i = 0; i < 8; i++) { //Data being send
        if(data[i] == 1) {
            bit_cell_one();
        } else if (data[i] == 0){
            bit_cell_zero();
        }
        
    }
    
    ADB_PORT &= ~(1 << ADB_DATA_PIN); //Stop bit for 65us
    _delay_us(65); //If there is a service request, it literally starts happening mid stop bit
    ADB_PORT |= (1 << ADB_DATA_PIN);
    
    if(ADB_PORT &= ~(1 << ADB_DATA_PIN)) { //There is only a response for talk commands
        _delay_us(205); //240 - 35 = 205
        receive_adb_code();
    }
}

void receive_adb_code(void) {
    
    bit_cell_one(); //Initial start bit
    
    for(int i = 0; i < 8; i++) {
        if (bit_is_clear(PIND, ADB_DATA_PIN)) {
            data_buffer_temp_2[i] = 0;
            
        } else if (bit_is_set(PIND, ADB_DATA_PIN)) {
            data_buffer_temp_2[i] = 1;
        }
    }
}

void convert_to_full_byte_adb(void) {
    
    uint8_t decimal = 0;
    
    for(int i = 0; i < 8; i++) {
     decimal = decimal << 1 | data_buffer_temp_2[i];
    }
    
    data_buffer_adb[index_adb] = decimal;
    index_adb++;
    
    if(index_ps2 == MAX_BUFFER_SIZE) {
        index_ps2 = 0;
    }
}
