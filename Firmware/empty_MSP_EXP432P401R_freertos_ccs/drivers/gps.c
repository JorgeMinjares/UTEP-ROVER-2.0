/*
 * gps.c
 *
 *  Created on: Mar 26, 2023
 *      Author: jorge
 */

#include "inc/gps.h"
#include "string.h"
void gps_init(gps_t * const gps){
    UART_init();
    UART_Params gps_params;

    UART_Params_init(&gps_params);

    gps_params.readMode = UART_MODE_BLOCKING;
    gps_params.writeMode = UART_MODE_BLOCKING;
    gps_params.readTimeout = UART_WAIT_FOREVER;
    gps_params.writeTimeout = UART_WAIT_FOREVER;
    gps_params.readCallback = NULL;
    gps_params.writeCallback = NULL;
    gps_params.readReturnMode = UART_RETURN_NEWLINE;
    gps_params.readDataMode = UART_DATA_BINARY;
    gps_params.writeDataMode = UART_DATA_TEXT;
    gps_params.readEcho = UART_ECHO_OFF;
    gps_params.baudRate = 9600;
    gps_params.dataLength = UART_LEN_8;
    gps_params.stopBits = UART_STOP_ONE;
    gps_params.parityType = UART_PAR_NONE;

    gps->handle = UART_open(gps->uart,&gps_params);
    if(gps->handle == NULL){
        while(1);
    }
}

void gps_read(gps_t * const gps){
    char c;
    uint8_t index = 0;
    memset(gps->buffer, 0, sizeof(gps->buffer));
    do{
        UART_read(gps->handle, &c, 1);
        gps->buffer[index++] = c;
    }while(c != '\n');
}
void tokenize_gps(gps_t * const gps){
    uint8_t i;
    uint8_t j = 0;
    uint8_t k = 0;
    for(i = 0; gps->buffer[i] != '\0'; i++){
        if(gps->buffer[i] != ','){
            gps->stream[j][k++] = gps->buffer[i];
        }
        else{
            j++;
            k = 0;
        }
    }
}
