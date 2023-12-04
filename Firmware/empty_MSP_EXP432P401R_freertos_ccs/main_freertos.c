/*
 * Copyright (c) 2016-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== main_freertos.c ========
 */
#include <stdint.h>

#ifdef __ICCARM__
#include <DLib_Threads.h>
#endif

/* POSIX Header files */
#include <pthread.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>

/* Driver configuration */
#include <ti/drivers/Board.h>


/*
 * void *motorMovementThread(void *arg0)
 * */
//#define DISPLAY_THREAD
#define MOTOR_MOVEMENT_THREAD
#define BLUETOOTH_THREAD
#define GPS_THREAD
#define ARM_THREAD
////#define SENSOR_THREAD
//#define ROBOTIC_ARM_THREAD


#ifdef MOTOR_MOVEMENT_THREAD
extern void *motorMovementThread(void *arg0);
#endif

#ifdef BLUETOOTH_THREAD
extern void *bluetoothThread(void *arg0);
#endif

#ifdef GPS_THREAD
extern void *gpsThread(void *arg0);
#endif

#ifdef SENSOR_THREAD
extern void *sensorThread(void *arg0);
#endif

#ifdef ARM_THREAD
extern void *armThread(void *arg0);
#endif

#ifdef ROBOTIC_ARM_THREADS
extern void *armThreadRight(void *arg0);
extern void *armThreadLeft(void *arg0);
#endif

#ifdef DISPLAY_THREAD
extern void *displayThread(void *arg0);
#endif
//extern void hardware_init();

/* Stack size in bytes */
#define THREADSTACKSIZE  2048 //1024

/*
 *  ======== main ========
 */
int main(void){

    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;

    /* initialize the system locks */
#ifdef __ICCARM__
    __iar_Initlocks();
#endif

    /* Call driver init functions */
    Board_init();
//    hardware_init(); /* Initialize Hardware */
    /* Initialize the attributes structure with default values */
    pthread_attr_init(&attrs);

    /* Set priority, detach state, and stack size attributes */
    priParam.sched_priority = 1;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0) {
        /* failed to set attributes */
        while (1) {}
    }

#ifdef BLUETOOTH_THREAD
    pthread_t thread1;
    priParam.sched_priority = 6;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    retc = pthread_create(&thread1, &attrs, bluetoothThread, NULL);
    if(retc != 0){
        /* pthread_create() faild */
        while(1){}
    }
#endif

#ifdef MOTOR_MOVEMENT_THREAD
    pthread_t thread2;
    priParam.sched_priority = 5;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    retc = pthread_create(&thread2, &attrs, motorMovementThread, NULL);
    if(retc != 0){
        /* pthread_create() faild */
        while(1){}
    }
#endif


#ifdef GPS_THREAD
    pthread_t thread3;
    priParam.sched_priority = 3;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    retc = pthread_create(&thread3, &attrs, gpsThread, NULL);
    if(retc != 0){
        /* pthread_create() faild */
        while(1){}
    }
#endif

#ifdef SENSOR_THREAD
    pthread_t thread4;
    retc = pthread_create(&thread4, &attrs, sensorThread, NULL);
    if(retc != 0){
        /* pthread_create() faild */
        while(1){}
    }
#endif

#ifdef ARM_THREAD
    pthread_t thread8;
    priParam.sched_priority = 3;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    retc = pthread_create(&thread8, &attrs, armThread, NULL);
    if(retc != 0){
       /* pthread_create() faild */
       while(1){}
    }
#endif

#ifdef ROBOTIC_ARM_THREADS
    pthread_t thread5;
    priParam.sched_priority = 4;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    retc = pthread_create(&thread5, &attrs, armThreadRight, NULL);
    if(retc != 0){
        /* pthread_create() faild */
        while(1){}
    }

    pthread_t thread6;
    retc = pthread_create(&thread6, &attrs, armThreadLeft, NULL);
    if(retc != 0){
        /* pthread_create() faild */
        while(1){}
    }
#endif

#ifdef DISPLAY_THREAD
    pthread_t thread7;
    priParam.sched_priority = 8;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    retc = pthread_create(&thread7, &attrs, displayThread, NULL);
    if(retc != 0){
        /* pthread_create() faild */
        while(1){}
    }
#endif
    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    return (0);
}

//*****************************************************************************
//
//! \brief Application defined malloc failed hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationMallocFailedHook()
{
    /* Handle Memory Allocation Errors */
    while(1)
    {
    }
}

//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    //Handle FreeRTOS Stack Overflow
    while(1)
    {
    }
}
