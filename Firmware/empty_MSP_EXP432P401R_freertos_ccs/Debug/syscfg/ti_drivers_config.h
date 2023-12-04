/*
 *  ======== ti_drivers_config.h ========
 *  Configured TI-Drivers module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSP_EXP432P401R
 *  by the SysConfig tool.
 */
#ifndef ti_drivers_config_h
#define ti_drivers_config_h

#define CONFIG_SYSCONFIG_PREVIEW

#define CONFIG_MSP_EXP432P401R

#ifndef DeviceFamily_MSP432P401x
#define DeviceFamily_MSP432P401x
#endif

#include <ti/devices/DeviceFamily.h>

#include <stdint.h>

/* support C++ sources */
#ifdef __cplusplus
extern "C" {
#endif


/*
 *  ======== ADC ========
 */

/* P5.0 */
#define HUMIDITY_SENSOR             0


/*
 *  ======== GPIO ========
 */

/* P4.0 */
#define MOTOR_1_A                   0
/* P4.1 */
#define MOTOR_1_B                   1
/* P4.2 */
#define MOTOR_2_A                   2
/* P4.3 */
#define MOTOR_2_B                   3
/* P8.5 */
#define NSLEEP                      4

/* LEDs are active high */
#define CONFIG_GPIO_LED_ON  (1)
#define CONFIG_GPIO_LED_OFF (0)

#define CONFIG_LED_ON  (CONFIG_GPIO_LED_ON)
#define CONFIG_LED_OFF (CONFIG_GPIO_LED_OFF)


/*
 *  ======== PWM ========
 */

/* P2.4 */
#define SERVO_0                     0
/* P2.5 */
#define SERVO_1                     1
/* P3.0 */
#define PWM_MOTOR_0                 2
/* P3.5 */
#define PWM_MOTOR_1                 3
/* P3.6 */
#define PWM_MOTOR_2                 4
/* P3.7 */
#define PWM_MOTOR_3                 5
/* P2.6 */
#define SERVO_2                     6
/* P2.7 */
#define SERVO_3                     7


/*
 *  ======== UART ========
 */

/*
 *  TX: P3.3
 *  RX: P3.2
 */
#define BLUETOOTH                   0
/*
 *  TX: P7.1
 *  RX: P7.0
 */
#define GPS                         1
/*
 *  TX: P9.7
 *  RX: P9.6
 */
#define Base_BLUETOOTH              2
/*
 *  TX: P1.3
 *  RX: P1.2
 *  XDS110 UART
 */
#define CONFIG_UART_0               3


/*
 *  ======== Board_init ========
 *  Perform all required TI-Drivers initialization
 *
 *  This function should be called once at a point before any use of
 *  TI-Drivers.
 */
extern void Board_init(void);

/*
 *  ======== Board_initGeneral ========
 *  (deprecated)
 *
 *  Board_initGeneral() is defined purely for backward compatibility.
 *
 *  All new code should use Board_init() to do any required TI-Drivers
 *  initialization _and_ use <Driver>_init() for only where specific drivers
 *  are explicitly referenced by the application.  <Driver>_init() functions
 *  are idempotent.
 */
#define Board_initGeneral Board_init

#ifdef __cplusplus
}
#endif

#endif /* include guard */
