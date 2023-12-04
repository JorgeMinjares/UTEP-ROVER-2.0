/*
 * servo.c
 *
 *  Created on: Mar 23, 2023
 *      Author: jorge
 */

#include "inc/servo.h"
void servo_init(servo_t * const servo){
    PWM_init();

    PWM_Params params;
    PWM_Params_init(&params);
    params.dutyUnits = PWM_DUTY_COUNTS;
    params.dutyValue = servo->duty;
    params.idleLevel = PWM_IDLE_LOW;
    params.periodUnits = PWM_PERIOD_COUNTS;
    params.periodValue = 240000;

    servo->handle = PWM_open(servo->pwm, &params);
    if(servo->handle == NULL){
        while(1);
    }
}

void arm_init(arm_t * const arm){
    servo_init(arm->joint_0);
    servo_init(arm->joint_1);
    servo_init(arm->joint_2);
    servo_init(arm->joint_3);
}

void servo_start(servo_t * const servo){
    PWM_start(servo->handle);
}
void servo_stop(servo_t * const servo){
    PWM_stop(servo->handle);
}

void arm_start(arm_t * const arm){
    servo_start(arm->joint_0);
    servo_start(arm->joint_1);
    servo_start(arm->joint_2);
    servo_start(arm->joint_3);
}
void arm_stop(arm_t * const arm){
    servo_stop(arm->joint_0);
    servo_stop(arm->joint_1);
    servo_stop(arm->joint_2);
    servo_stop(arm->joint_3);
}

void servo_set_duty(servo_t * const servo, uint16_t duty){
    servo->duty = duty;

    if(PWM_setDuty(servo->handle, servo->duty) == PWM_STATUS_ERROR){
        while(1);
    }
}

void arm_set_duty_all(arm_t * const arm, uint16_t duty){
    servo_set_duty(arm->joint_0, duty);
    servo_set_duty(arm->joint_1, duty);
    servo_set_duty(arm->joint_2, duty);
    servo_set_duty(arm->joint_3, duty);
}

void arm_set_duty(arm_t * const arm, uint16_t duty, uint8_t joint){
    switch(joint){
    case 1: servo_set_duty(arm->joint_0, duty);
            break;
    case 2: servo_set_duty(arm->joint_1, duty);
            break;
    case 3: servo_set_duty(arm->joint_2, duty);
            break;
    case 4: servo_set_duty(arm->joint_3, duty);
            break;
    default: break;
    }
}
