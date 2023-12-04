/*
 * sensor.h
 *
 *  Created on: Apr 4, 2023
 *      Author: jorge
 */

#ifndef INC_SENSOR_H_
#define INC_SENSOR_H_

#include "ti_drivers_config.h"
#include <ti/drivers/ADC.h>
#include <stdint.h>
#include <unistd.h>

/* ADC Setup */
#define SENSOR_SAMPLE 5
typedef struct{
    uint8_t sensor;
    uint16_t adc_avg;
    uint16_t adc_value[SENSOR_SAMPLE];
    ADC_Handle adc_handle;
}sensor_t;

void sensor_init(sensor_t * const sensor);
void sensor_read(sensor_t * const sensor);


#endif /* INC_SENSOR_H_ */
