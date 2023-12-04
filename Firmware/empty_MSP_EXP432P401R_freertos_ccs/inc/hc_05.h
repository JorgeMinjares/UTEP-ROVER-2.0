/*
 * hc_05.h
 *
 *  Created on: Mar 26, 2023
 *      Author: jorge
 */

#ifndef INC_HC_05_H_
#define INC_HC_05_H_

#include "ti_drivers_config.h"
#include <ti/drivers/UART.h>
#include <stdint.h>

#define HC_05_BUFFER_SIZE 64

typedef struct{
    uint8_t uart;
    UART_Handle handle;
    char buffer[HC_05_BUFFER_SIZE];
}bluetooth_t;

void hc_05_init(bluetooth_t * const hc_05);
void hc_05_write(bluetooth_t * const hc_05, char * buffer);
void hc_05_read(bluetooth_t * const hc_05);

#endif /* INC_HC_05_H_ */
