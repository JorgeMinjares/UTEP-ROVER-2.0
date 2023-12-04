/*
 * hc_05.c
 *
 *  Created on: Mar 26, 2023
 *      Author: jorge
 */


#define HC_05_BUFFER_SIZE 64

#include "inc/hc_05.h"
#include <stdio.h>
#include <string.h>
void hc_05_init(bluetooth_t * const hc_05){
    UART_init();

    UART_Params hc_05_params;

    UART_Params_init(&hc_05_params);

    hc_05_params.readMode = UART_MODE_BLOCKING;
    hc_05_params.writeMode = UART_MODE_BLOCKING;
    hc_05_params.readTimeout = UART_WAIT_FOREVER;
    hc_05_params.writeTimeout = UART_WAIT_FOREVER;
    hc_05_params.readCallback = NULL;
    hc_05_params.writeCallback = NULL;
    hc_05_params.readReturnMode = UART_RETURN_NEWLINE;
    hc_05_params.readDataMode = UART_DATA_BINARY;
    hc_05_params.writeDataMode = UART_DATA_BINARY;
    hc_05_params.readEcho = UART_ECHO_OFF;
    hc_05_params.baudRate = 9600;
    hc_05_params.dataLength = UART_LEN_8;
    hc_05_params.stopBits = UART_STOP_ONE;
    hc_05_params.parityType = UART_PAR_NONE;

    hc_05->handle = UART_open(hc_05->uart,&hc_05_params);
    if(hc_05->handle == NULL){
        while(1);
    }
}
void hc_05_write(bluetooth_t * const hc_05, char * buffer){
    sprintf(hc_05->buffer, "%s", buffer);
    UART_write(hc_05->handle, hc_05->buffer, strlen(hc_05->buffer));
}
void hc_05_read(bluetooth_t * const hc_05){
    memset(hc_05->buffer, 0, HC_05_BUFFER_SIZE);
    char c;
    uint8_t index = 0;
    do{
        UART_read(hc_05->handle, &c, 1);
        hc_05->buffer[index++] = c;
    }while(c != '\n');
    index = 0;
}

