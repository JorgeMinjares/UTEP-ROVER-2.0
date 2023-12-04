/*
 * motors.h
 *
 *  Created on: Mar 23, 2023
 *      Author: jorge
 */

#ifndef INC_MOTORS_H_
#define INC_MOTORS_H_

#include "ti_drivers_config.h"
#include <stdint.h>
#include <stddef.h>

#define MOTOR_TYPE_PWM

#ifdef MOTOR_TYPE_DC

#include <ti/drivers/GPIO.h>
#define MOTOR_LOW  0
#define MOTOR_HIGH 1

typedef enum{
    NONE = -1,
    DISABLE,
    ENABLE,
}motor_state_t;

typedef struct{
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
}motor_t;

typedef struct{
    motor_t * motor_0;
    motor_t * motor_1;
    uint8_t sleep;
    motor_state_t state;
}motors_t;

void motors_init(motors_t * motor);
void motor_forward(motor_t * const motor);
void motor_backward(motor_t * const motor);
void motors_left(motors_t * const motors_left, motors_t * const motors_right);
void motors_right(motors_t * const motors_left, motors_t * const motors_right);
void motors_forward(motors_t * const motors_left, motors_t * const motors_right);
void motors_backward(motors_t * const motors_left, motors_t * const motors_right);
void motor_stop(motor_t * const motor);
void motors_stop(motors_t * const motors_left, motors_t * const motors_right);
void motors_enable(motors_t * const motors);
void motors_disable(motors_t * const motors);

#else
// pwm motor setup
#include <ti/drivers/PWM.h>


#define FORWARD 20000  //  2ms
#define STOP    18000  //  1.5ms
#define BACKWARD 16000  // 1ms
typedef struct{
    uint32_t duty;
    uint8_t pwm;
    PWM_Handle handle;
}motor_t;

typedef struct{
    motor_t * motor_1;
    motor_t * motor_2;
}motors_t;

void motor_init(motor_t * const motor);
void motor_forward(motor_t * const motor);
void motor_backward(motor_t * const motor);
void motor_stop(motor_t * const motor);
void motor_pwm_start(motor_t * const motor);
void motor_pwm_stop(motor_t * const motor);

void motors_init(motors_t * const left_side, motors_t * const right_side);
void motor_set_duty(motor_t * const motor, uint32_t duty);
void motors_forward(motors_t * const left_side, motors_t * const right_side);
void motors_backward(motors_t * const left_side, motors_t * const right_side);
void motors_left(motors_t * const left_side, motors_t * const right_side);
void motors_right(motors_t * const left_side, motors_t * const right_side);
void motors_stop(motors_t * const left_side, motors_t * const right_side);
void motors_pwm_start(motors_t * const left_side, motors_t * const right_side);
void motors_pwm_stop(motors_t * const left_side, motors_t * const right_side);

#endif

#endif /* INC_MOTORS_H_ */
