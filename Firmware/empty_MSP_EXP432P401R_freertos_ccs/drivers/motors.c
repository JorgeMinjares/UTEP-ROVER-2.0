/*
 * motors.c
 *
 *  Created on: Mar 23, 2023
 *      Author: jorge
 */
#include "inc/motors.h"

#ifdef MOTOR_TYPE_DC
void motors_init(motors_t * motor){
    GPIO_init();    // Initialize all GPIOs
    /* Set Motor 0 Pins */
    GPIO_setConfig(motor->motor_0->a, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(motor->motor_0->b, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    /* Set Motor 1 Pins*/
    GPIO_setConfig(motor->motor_1->a, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(motor->motor_1->b, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    GPIO_setConfig(motor->sleep, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); // Set motors driver to sleep
                                                                       // LOW = SLEEP | HIGH = AWAKE
}

void motor_forward(motor_t * const motor){
    GPIO_write(motor->a, MOTOR_HIGH);
    GPIO_write(motor->b, MOTOR_LOW);
}

void motor_backward(motor_t * const motor){
    GPIO_write(motor->a, MOTOR_LOW);
    GPIO_write(motor->b, MOTOR_HIGH);
}

void motors_left(motors_t * const motors_left, motors_t * const motors_right){
    motor_backward(motors_left->motor_0);
    motor_backward(motors_left->motor_1);

    motor_forward(motors_right->motor_0);
    motor_forward(motors_right->motor_1);
}

void motors_right(motors_t * const motors_left, motors_t * const motors_right){
    motor_forward(motors_left->motor_0);
    motor_forward(motors_left->motor_1);

    motor_backward(motors_right->motor_0);
    motor_backward(motors_right->motor_1);

}

void motors_forward(motors_t * const motors_left, motors_t * const motors_right){
    motor_forward(motors_left->motor_0);
    motor_forward(motors_left->motor_1);

    motor_forward(motors_right->motor_0);
    motor_forward(motors_right->motor_1);

}
void motors_backward(motors_t * const motors_left, motors_t * const motors_right){
    motor_backward(motors_left->motor_0);
    motor_backward(motors_left->motor_1);

    motor_backward(motors_right->motor_0);
    motor_backward(motors_right->motor_1);
}

void motor_stop(motor_t * const motor){
    GPIO_write(motor->a, MOTOR_HIGH);
    GPIO_write(motor->b, MOTOR_HIGH);
}

void motors_stop(motors_t * const motors_left, motors_t * const motors_right){
    motor_stop(motors_left->motor_0);
    motor_stop(motors_left->motor_1);

    motor_stop(motors_right->motor_0);
    motor_stop(motors_right->motor_1);
}

void motors_enable(motors_t * const motors){
    GPIO_write(motors->sleep, MOTOR_HIGH);
}
void motors_disable(motors_t * const motors){
    GPIO_write(motors->sleep, MOTOR_LOW);
}
#else

void motor_init(motor_t * const motor){
    PWM_init();
    PWM_Params params;
    PWM_Params_init(&params);
    params.dutyUnits = PWM_DUTY_COUNTS;
    params.dutyValue = motor->duty;
    params.idleLevel = PWM_IDLE_LOW;
    params.periodUnits = PWM_PERIOD_COUNTS;
    params.periodValue = 240000;
    motor->handle = PWM_open(motor->pwm, &params);
    if(motor->handle == NULL){
        while(1);
    }
}
void motors_init(motors_t * const left_side, motors_t * const right_side){
    motor_init(left_side->motor_1);
    motor_init(left_side->motor_2);

    motor_init(right_side->motor_1);
    motor_init(right_side->motor_2);
}

void motor_pwm_start(motor_t * const motor){
    PWM_start(motor->handle);
}
void motor_pwm_stop(motor_t * const motor){
    PWM_stop(motor->handle);
}
void motors_pwm_start(motors_t * const left_side, motors_t * const right_side){
    motor_pwm_start(left_side->motor_1);
    motor_pwm_start(left_side->motor_2);

    motor_pwm_start(right_side->motor_1);
    motor_pwm_start(right_side->motor_2);
}
void motors_pwm_stop(motors_t * const left_side, motors_t * const right_side){
    motor_pwm_stop(left_side->motor_1);
    motor_pwm_stop(left_side->motor_2);

    motor_pwm_stop(right_side->motor_1);
    motor_pwm_stop(right_side->motor_2);
}
void motor_set_duty(motor_t * const motor, uint32_t duty){
    motor->duty = duty;

    if(PWM_setDuty(motor->handle, motor->duty) == PWM_STATUS_ERROR){
        while(1);
    }
}
void motor_forward(motor_t * const motor){
     motor_set_duty(motor, FORWARD);
}
void motor_backward(motor_t * const motor){
    motor_set_duty(motor, BACKWARD);
}
void motor_stop(motor_t * const motor){
    motor_set_duty(motor, STOP);
}
void motors_forward(motors_t * const left_side, motors_t * const right_side){
    motor_forward(left_side->motor_1);
    motor_forward(left_side->motor_2);

    motor_forward(right_side->motor_1);
    motor_forward(right_side->motor_2);
}
void motors_backward(motors_t * const left_side, motors_t * const right_side){
    motor_backward(left_side->motor_1);
    motor_backward(left_side->motor_2);

    motor_backward(right_side->motor_1);
    motor_backward(right_side->motor_2);
}
void motors_left(motors_t * const left_side, motors_t * const right_side){
    motor_backward(left_side->motor_1);
    motor_backward(left_side->motor_2);

    motor_forward(right_side->motor_1);
    motor_forward(right_side->motor_2);
}
void motors_right(motors_t * const left_side, motors_t * const right_side){
    motor_forward(left_side->motor_1);
    motor_forward(left_side->motor_2);

    motor_backward(right_side->motor_1);
    motor_backward(right_side->motor_2);
}
void motors_stop(motors_t * const left_side, motors_t * const right_side){
    motor_stop(left_side->motor_1);
    motor_stop(left_side->motor_2);

    motor_stop(right_side->motor_1);
    motor_stop(right_side->motor_2);
}
#endif
