/*
 * stepper.h
 *
 *  Created on: Sep 27, 2023
 *      Author: jorge
 */

#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_

#include <ti/drivers/GPIO.h>
#define STEP_LOW  0
#define STEP_HIGH 1

typedef enum{
    STATE_NONE = -1,
    DISABLE,
    ENABLE,
}state_t;

typedef struct{
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
}channels_t;

typedef struct{
    channels_t * channel;
    state_t state;
    uint8_t sleep;
}stepper_t;

void stepper_channel_init(channels_t * channel);
void stepper_motor_init(stepper_t * const stepper);
void stepper_stop(stepper_t * const stepper);
void stepper_right(stepper_t * const stepper);
void stepper_left(stepper_t * const stepper);

#endif /* INC_STEPPER_H_ */
