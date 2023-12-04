/*
 *  ======== senior.c ========
 */

/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <semphr.h>
#include <task.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/ADC.h>
#include <ti/display/Display.h>
//#include <ti/drivers/TIMER.h>

/* Driver configuration */
#include "ti_drivers_config.h"
#include "inc/motors.h"
#include "inc/servo.h"
#include "inc/sensor.h"
#include "inc/hc_05.h"
#include "inc/gps.h"
#include "inc/stepper.h"

/* Joystick Messages */
#define JOYSTICK_UP_MSG     "j-up\r\n"
#define JOYSTICK_DOWN_MSG   "j-down\r\n"
#define JOYSTICK_LEFT_MSG   "j-left\r\n"
#define JOYSTICK_RIGHT_MSG  "j-right\r\n"
#define JOYSTICK_BUTTON_MSG "j-button\r\n"
/* Button Messages */
#define BUTTON_UP_MSG       "b-up\r\n"
#define BUTTON_DOWN_MSG     "b-down\r\n"
#define BUTTON_LEFT_MSG     "b-left\r\n"
#define BUTTON_RIGHT_MSG    "b-right\r\n"

typedef enum{
    NO = -1, UP, DOWN, RIGHT, LEFT, CENTER_BUTTON, BACK_RIGHT, BACK_LEFT,
}button_t;

typedef enum {
    JOYSTICK,
    BUTTON,
}device_t;

typedef struct{
    device_t device;
    button_t button;
}event_t;

event_t joystick = {.device = JOYSTICK, .button = NO};
event_t buttons = {.device = BUTTON, .button = NO};

bool isSubstring(char *str, char *substr);

/* Global objects */

servo_t servo_0 = {.duty = SERVO_AVERAGE, .pwm = SERVO_0, .handle = NULL};
servo_t servo_1 = {.duty = SERVO_AVERAGE, .pwm = SERVO_1, .handle = NULL};
servo_t servo_2 = {.duty = SERVO_AVERAGE, .pwm = SERVO_2, .handle = NULL};
servo_t servo_3 = {.duty = SERVO_AVERAGE, .pwm = SERVO_3, .handle = NULL};

arm_t robotic_arm = {.joint_0 = &servo_0, .joint_1 = &servo_1, .joint_2 = &servo_2, .joint_3 = &servo_3};


channels_t channels = {.a = MOTOR_1_A, .b = MOTOR_1_B, .c = MOTOR_2_A, .d = MOTOR_2_B};
stepper_t robotic_base = {.channel = &channels, .state = STATE_NONE, .sleep = NSLEEP};

bluetooth_t hc_05 = {.uart = BLUETOOTH,.handle = NULL, .buffer = "NONE"};
bluetooth_t gui_bluetooth = {.uart = Base_BLUETOOTH, .handle = NULL, .buffer = "NONE"};

sensor_t humidity_sensor = {.sensor = HUMIDITY_SENSOR, .adc_avg = 0, .adc_value = 0, .adc_handle = NULL};

motor_t motor_0 = {.duty = STOP, .pwm = PWM_MOTOR_0, .handle = NULL};
motor_t motor_1 = {.duty = STOP, .pwm = PWM_MOTOR_1, .handle = NULL};
motor_t motor_2 = {.duty = STOP, .pwm = PWM_MOTOR_2, .handle = NULL};
motor_t motor_3 = {.duty = STOP, .pwm = PWM_MOTOR_3, .handle = NULL};

motors_t right_side = {.motor_1 = &motor_0, .motor_2 = &motor_1};
motors_t left_side = {.motor_1 = &motor_2, .motor_2 = &motor_3};
gps_t gps = {.uart = GPS,.handle = NULL, .buffer = "NONE"};

//void hardware_init(void);


/*\brief Bluetooth Thread
 *
 * Detailed description starts here
 * @param arg           Bluetooth Arguement
 * @return None
 * */
void *bluetoothThread(void *arg0){

        Display_init();
        Display_Params displayParams;
        Display_Handle displayHandle;
        Display_Params_init(&displayParams);
        displayParams.lineClearMode = DISPLAY_CLEAR_BOTH;

        displayHandle = Display_open(Display_Type_UART,&displayParams);

        if(displayHandle == NULL){
            Display_printf(displayHandle, 0,0, "Display has not been successfully setup!\n");
            while(1);
        }
        else{
            Display_printf(displayHandle, 0,0, "Display has been successfully setup!\n");
        }

        hc_05_init(&hc_05);
        hc_05_init(&gui_bluetooth);

        while(1){

            hc_05_read(&hc_05);
            /* Decrypt messaged received */

            /* Lock Mutuex */
            Display_printf(displayHandle, 0,0, "%s", hc_05.buffer);

            if(isSubstring(gui_bluetooth.buffer,"j-up\r\n")){
                Display_printf(displayHandle, 0,0, "J-FORWARD\n");
                joystick.button = UP;
                memset(hc_05.buffer, 0 , sizeof(hc_05.buffer));
            }
            else if(isSubstring(hc_05.buffer,"j-down\r\n")){
                Display_printf(displayHandle, 0,0, "J-DOWNWARD\n");
                joystick.button = DOWN;
                memset(hc_05.buffer, 0 , sizeof(hc_05.buffer));
            }
            else if(isSubstring(hc_05.buffer,"j-left")){
                Display_printf(displayHandle, 0,0, "J-LEFT\n");
                joystick.button = LEFT;
                memset(hc_05.buffer, 0 , sizeof(hc_05.buffer));
            }
            else if(isSubstring(hc_05.buffer,"j-right")){
                Display_printf(displayHandle, 0,0, "J-RIGHT\n");
                joystick.button = RIGHT;
                memset(hc_05.buffer, 0 , sizeof(hc_05.buffer));
            }
            else if(isSubstring(hc_05.buffer,"j-button")){
                Display_printf(displayHandle, 0,0, "J-BUTTON\n");
                joystick.button = CENTER_BUTTON;
                memset(hc_05.buffer, 0 , sizeof(hc_05.buffer));
            }
            else if(isSubstring(hc_05.buffer,"b-up")){
                Display_printf(displayHandle, 0,0, "B-UP\n");
                buttons.button = UP;
                memset(hc_05.buffer, 0 , sizeof(hc_05.buffer));
            }
            else if(isSubstring(hc_05.buffer,"b-down")){
                Display_printf(displayHandle, 0,0, "B-DOWN\n");
                buttons.button = DOWN;
                memset(hc_05.buffer, 0 , sizeof(hc_05.buffer));
            }
            else if(isSubstring(hc_05.buffer,"b-right")){
                Display_printf(displayHandle, 0,0, "B-RIGHT\n");
                buttons.button = RIGHT;
                memset(hc_05.buffer, 0 , sizeof(hc_05.buffer));
            }
            else if(isSubstring(hc_05.buffer,"b-left")){
                Display_printf(displayHandle, 0,0, "B-LEFT\n");
                buttons.button = LEFT;
                memset(hc_05.buffer, 0 , sizeof(hc_05.buffer));
            }
            else if(isSubstring(hc_05.buffer, "b-back-right")){
                Display_printf(displayHandle, 0,0, "B-BACK-RIGHT\n");
                buttons.button = BACK_RIGHT;
                memset(hc_05.buffer, 0 , sizeof(hc_05.buffer));
            }
            else if(isSubstring(hc_05.buffer, "b-back-left")){
                Display_printf(displayHandle, 0,0, "B-BACK-LEFT\n");
                buttons.button = BACK_LEFT;
                memset(hc_05.buffer, 0 , sizeof(hc_05.buffer));
            }
            else{
                joystick.button = NO;
                buttons.button = NO;
            }
            /* Mutuex unlock */
        }
}

/*\brief Motor Movement Thread
 *
 * Detailed description starts here
 * @param arg           Motor Movement Arguement
 * @return None
 * */
void *motorMovementThread(void *arg0){
//    hardware_init();
    motors_init(&left_side, &right_side);
    motors_pwm_start(&left_side, &right_side);
    int count = 0;
    while(1){
        /* If j-up */
        if(joystick.button == NO){
            motors_stop(&left_side, &right_side);
        }
        else if(buttons.button == NO){
            stepper_stop(&robotic_base);
        }
        else if(joystick.button == UP){
            motors_forward(&left_side, &right_side);
        }
        /* If j-down */
        else if(joystick.button == DOWN){
            motors_backward(&left_side, &right_side);
        }
        /* If j-right */
        else if(joystick.button == RIGHT){
            motors_right(&left_side, &right_side);
        }
        /* If j-left */
        else if(joystick.button == LEFT){
            motors_left(&left_side, &right_side);
        }
        /* Stepper Motor */
        else if(buttons.button == RIGHT){
            count++;
            if(count >= 50){
                __delay_cycles(100);
            }
            else{
                /* Move base motor forward */
                stepper_right(&robotic_base);
            }
        }
        /* Stepper Motor */
        else if(buttons.button == LEFT){
            count--;
            if(count <= -50){
                __delay_cycles(100);
            }
            else{
                /* Move base motor backward */
                stepper_left(&robotic_base);
            }
        }
        else{
            __delay_cycles(100);
        }
    }
}
/*\brief GPS Thread
 *
 * Detailed description starts here
 * @param arg           GPS Arguement
 * @return None
 * */
void *gpsThread(void *arg0){
    gps_init(&gps);
    while(1){
        gps_read(&gps);
        if(isSubstring(gps.buffer, "$GPRMC")){
            tokenize_gps(&gps);
            //Send string to GUI
            hc_05_write(&gui_bluetooth, gps.buffer);
        }
        __delay_cycles(100);
    }
}
/*\brief Sensor Thread
 *
 * Detailed description starts here
 * @param arg           Sensor Arguement
 * @return None
 */
void *sensorThread(void *arg0){

    while(1){
        //CONVERT REGISTER LEVEL CODE

        //SEND TO GUI
    }
}
/*\brief Arm Thread
 *
 * Detailed description starts here
 * @param arg           Arm Arguement
 * @return None
 * */
void *armThreadRight(void *arg0){
    arm_set_duty_all(&robotic_arm, SERVO_AVERAGE);
    arm_start(&robotic_arm);
    int count_bottom = SERVO_AVERAGE;
    int count_upper = SERVO_AVERAGE;
    while(1){
        if(buttons.button == NO){
            arm_stop(&robotic_arm);
        }
        else if(buttons.button == UP){
            if((count_bottom += 1000) != SERVO_MAX){
                arm_set_duty(&robotic_arm, count_bottom, 1);
            }
        }
        else if(buttons.button == DOWN){
            if((count_bottom -= 1000) != SERVO_MIN){
                arm_set_duty(&robotic_arm, count_bottom, 1);
            }
        }
        else if(buttons.button == BACK_RIGHT){
            if((count_upper += 1000) != SERVO_MAX){
                arm_set_duty(&robotic_arm, count_upper, 3);
            }
        }
        else if(buttons.button == BACK_LEFT){
            if((count_upper -= 1000) != SERVO_MIN){
                arm_set_duty(&robotic_arm, count_upper, 3);
            }
        }
        else{
            __delay_cycles(100);
        }
    }
}

void *armThreadLeft(void *arg0){

    int count_bottom = SERVO_AVERAGE;
    int count_upper = SERVO_AVERAGE;
    while(1){
            if(buttons.button == NO){
                arm_stop(&robotic_arm);
            }
            else if(buttons.button == UP){
                if((count_bottom -= 1000) != SERVO_MIN){
                    arm_set_duty(&robotic_arm, count_bottom, 2);
            }
            else if(buttons.button == DOWN){
                if((count_bottom += 1000) != SERVO_MAX)
                    arm_set_duty(&robotic_arm, count_bottom, 2);
            }
            else if(buttons.button == BACK_RIGHT){
                if((count_upper -= 1000) != SERVO_MIN){
                    arm_set_duty(&robotic_arm, count_upper, 4);
                }
            }
            else if(buttons.button == BACK_LEFT){
                if((count_upper += 1000) != SERVO_MAX){
                    arm_set_duty(&robotic_arm, count_upper, 4);
                }
            }
            else{
                __delay_cycles(100);
            }
        }
    }
}


bool isSubstring(char *str, char *substr){
    int i;
    int len = strlen(str);
    int len2 = strlen(substr);
    for(i = 0; i < len; i++){
        if(str[i] == substr[0]){
            int j;
            int temp = i;
            for(j = 0; j < len2; j++){
                if(substr[j] == str[temp++])
                    continue;
                else{
                    break;
                }
            }
            if(temp - i == strlen(substr)){
                return true;
            }
        }
    }
    return false;
}
