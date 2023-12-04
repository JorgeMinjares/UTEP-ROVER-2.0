/*
 * gps.h
 *
 *  Created on: Mar 26, 2023
 *      Author: jorge
 */

#ifndef INC_GPS_H_
#define INC_GPS_H_

#include "ti_drivers_config.h"
#include <ti/drivers/UART.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define GPS_BUFFER_SIZE 128

typedef struct{
    uint8_t uart;
    UART_Handle handle;
    char buffer[GPS_BUFFER_SIZE];
    char stream[12][64];
}gps_t;

void gps_init(gps_t * const gps);
void gps_read(gps_t * const gps);
void tokenize_gps(gps_t * const gps);


#endif /* INC_GPS_H_ */
