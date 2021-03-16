//
// Created by Luca Giovannesi.
//

#include "servo_device.h"

#define SERVO_MIN 550
#define SERVO_MAX 2500
#define DEGREE_MAX 180
#define DEGREE_TO_US(x) ((x*(SERVO_MAX-SERVO_MIN)/DEGREE_MAX)+SERVO_MIN)

void servo_device_init(servo_device_t *servo_device, int pwm, int channel) {
    servo_t * servo = &(servo_device->servo);

    servo_init(servo, PWM_DEV(pwm),channel, SERVO_MIN, SERVO_MAX);
    servo_set(servo,DEGREE_TO_US(0));

}

void servo_device_set_position(servo_device_t * servo_device, int degree){
    servo_t servo = servo_device->servo;
    servo_set(&servo,DEGREE_TO_US(degree));
}