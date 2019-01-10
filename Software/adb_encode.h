//
//  adb_encode.h
//  Index
//

#ifndef adb_encode_h
#define adb_encode_h

#include "global.h"

struct adb_frame {
    
    uint8_t *data_buffer; //LSB -> MSB
    
};


/* Used to convert the array of 1s and 0s that are received by the receive_adb_code function
 * into just one decimal value that is placed into the global data_buffer_adb array
 * Also controls the overflow of the global arrays by reseting the index values once they reach a certain value
 */
extern void convert_to_full_byte_adb(void);

/* Receives code from the other ADB device
 * Similar idea to how the PS2 protocol works.
 * Listens for the line to go high after a long low for service request
 */
extern void receive_adb_code(void);


 /* This will send code to the other ADB device
 * For a command, it's 8 bit cells
 * First 4 bits is the address
 * The two bits after the address are if you want the device to talk back
 * The last two bits are what goes to Register 0 of the device/host. Register 0 is where Apple devices go to
 * retrieve stuff.
 */
extern void send_adb_command(uint8_t *data);

#endif /* adb_encode_h */
