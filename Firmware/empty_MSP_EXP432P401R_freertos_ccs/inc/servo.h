/*
 * servo.h
 *
 *  Created on: Mar 23, 2023
 *      Author: jorge
 */

#ifndef INC_SERVO_H_
#define INC_SERVO_H_

#include "ti_drivers_config.h"
#include <ti/drivers/PWM.h>
#include <stdint.h>
#include <stddef.h>

#define SERVO_MIN 6000
#define SERVO_AVERAGE 18000
#define SERVO_MAX 30000

typedef struct{
    uint16_t duty;
    uint8_t pwm;
    PWM_Handle handle;
}servo_t;

typedef struct{
    servo_t * joint_0;
    servo_t * joint_1;
    servo_t * joint_2;
    servo_t * joint_3;
}arm_t;

void servo_init(servo_t * const servo);
void arm_init(arm_t * const arm);
void servo_start(servo_t * const servo);
void servo_stop(servo_t * const servo);

void arm_start(arm_t * const arm);
void arm_stop(arm_t * const arm);
void servo_set_duty(servo_t * const servo, uint16_t duty);
void arm_set_duty_all(arm_t * const arm, uint16_t duty);
void arm_set_duty(arm_t * const arm, uint16_t duty, uint8_t joint);


#endif /* INC_SERVO_H_ */
