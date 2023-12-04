/*
 * sensor.c
 *
 *  Created on: Apr 4, 2023
 *      Author: jorge
 */

#include "inc/sensor.h"

void sensor_init(sensor_t * const sensor){
    ADC_init();
    ADC_Params params;

    ADC_Params_init(&params);

    sensor->adc_handle = ADC_open(sensor->sensor,&params);

    if(sensor->adc_handle == NULL){
        ADC_close(sensor->adc_handle);
    }
}
void sensor_read(sensor_t * const sensor){
    uint16_t adcAvg = 0;
    uint8_t i = 0;
    for(i = 0; i < SENSOR_SAMPLE; i++){
        ADC_convert(sensor->adc_handle, &sensor->adc_value[i]);
        adcAvg += sensor->adc_value[i];
    }
    sensor->adc_avg = adcAvg/SENSOR_SAMPLE;
    return;
}

