//
//  ps2_encode.h
//  Index
//

#ifndef ps2_encode_h
#define ps2_encode_h

#include "global.h"

struct ps2_frame {
    
    uint8_t *data_buffer; //LSB -> MSB
    uint8_t parity;
    uint8_t stop;
    
};

/*
 * Used to convert the array of 1s and 0s that are received by the receive_ps2_code function
 * into just one decimal value that is placed into the global data_buffer_ps2 array
 * Also controls the overflow of the global arrays by reseting the index values once they reach a certain value
 */
extern void convert_to_full_byte_ps2(void);

/* Sends code to the other ps2 device. TODO: figure out how to let the converter know if it's the host/device
 * Similar idea to how the adb protocol works.
 * Pulls data low and clock high and sends data.
 */
extern void send_ps2_command(uint8_t *data);

/* Receives code from the other ps2 device
* Similar idea to how the adb protocol works.
* Listens for when both lines go low to begin receiving data
*/
extern void receive_ps2_code(void);

/*
 * Converts the scan code it receives into the equivalent ASCII char code
 * The ASCII char code is then retransmitted (WIP)
 *
 */
extern uint8_t convert_scan_code(void);

#endif /* ps2_encode_h */
