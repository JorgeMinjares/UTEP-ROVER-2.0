/*
 * stepper.c
 *
 *  Created on: Sep 27, 2023
 *      Author: jorge
 */
#include <ti/drivers/GPIO.h>
#include "inc/stepper.h"
#include <ti/drivers/Timer.h>

void stepper_channel_init(channels_t * channel){
    GPIO_setConfig(channel->a, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(channel->b, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(channel->c, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(channel->d, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
}
void stepper_motor_init(stepper_t * const stepper){
    GPIO_init();
    stepper_channel_init(stepper->channel);
    GPIO_setConfig(stepper->sleep, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
}
void stepper_stop(stepper_t * const stepper){
    GPIO_write(stepper->channel->a, STEP_LOW);
    GPIO_write(stepper->channel->b, STEP_LOW);
    GPIO_write(stepper->channel->c, STEP_LOW);
    GPIO_write(stepper->channel->d, STEP_LOW);
}

void stepper_right(stepper_t * const stepper){
    GPIO_write(stepper->channel->a, STEP_HIGH);
    __delay_cycles(100000);
    GPIO_write(stepper->channel->a, STEP_LOW);
    __delay_cycles(100000);
    GPIO_write(stepper->channel->c, STEP_HIGH);
    __delay_cycles(100000);
    GPIO_write(stepper->channel->c, STEP_LOW);
    __delay_cycles(100000);
    GPIO_write(stepper->channel->b, STEP_HIGH);
    __delay_cycles(100000);
    GPIO_write(stepper->channel->b, STEP_LOW);
    __delay_cycles(100000);
    GPIO_write(stepper->channel->d, STEP_HIGH);
    __delay_cycles(100000);
    GPIO_write(stepper->channel->d, STEP_LOW);
    __delay_cycles(100000);
}

void stepper_left(stepper_t * const stepper){
    GPIO_write(stepper->channel->d, STEP_HIGH);
    __delay_cycles(100000);
    GPIO_write(stepper->channel->d, STEP_LOW);
    __delay_cycles(100000);
    GPIO_write(stepper->channel->b, STEP_HIGH);
    __delay_cycles(100000);
    GPIO_write(stepper->channel->b, STEP_LOW);
    __delay_cycles(100000);
    GPIO_write(stepper->channel->c, STEP_HIGH);
    __delay_cycles(100000);
    GPIO_write(stepper->channel->c, STEP_LOW);
    __delay_cycles(100000);
    GPIO_write(stepper->channel->a, STEP_HIGH);
    __delay_cycles(100000);
    GPIO_write(stepper->channel->a, STEP_LOW);
    __delay_cycles(100000);
}
