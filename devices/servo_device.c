//
// Created by Luca Giovannesi.
//

#include "servo_device.h"

#define SERVO_MIN 550
#define SERVO_MAX 2500
#define DEGREE_MAX 180
#define DEGREE_TO_US(x) ((x*(SERVO_MAX-SERVO_MIN)/DEGREE_MAX)+SERVO_MIN)

void servo_device_init(servo_device_t *servo_device, int pwm, int channel) {
    servo_t *servo = &(servo_device->servo);
    servo_device->degree = 0;
    servo_device->manual_override = 0;
    servo_init(servo, PWM_DEV(pwm), channel, SERVO_MIN, SERVO_MAX);
    servo_set(servo, DEGREE_TO_US(0));

}

void servo_device_set_position(servo_device_t *servo_device, int degree) {
    servo_t servo = servo_device->servo;
    servo_device->degree = degree;
    servo_set(&servo, DEGREE_TO_US(degree));
}

int servo_device_get_position(servo_device_t *servo_device) {
    return servo_device->degree;
}

int servo_device_is_manual_override(servo_device_t *servo_device) {
    return servo_device->manual_override == 1;
}

void servo_device_set_manual_override(servo_device_t *servo_device, int degree) {
    servo_device_set_position(servo_device, degree);
    servo_device->manual_override = 1;
}

void servo_device_clear_manual_override(servo_device_t *servo_device) {
    servo_device->manual_override = 0;
}
