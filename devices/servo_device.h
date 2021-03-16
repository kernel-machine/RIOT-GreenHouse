//
// Created by Luca Giovannesi.
//

#ifndef GREENHOUSE_SERVO_DEVICE_H
#define GREENHOUSE_SERVO_DEVICE_H

#include "../RIOT/drivers/include/servo.h"
#include "../RIOT/drivers/include/periph/gpio.h"

typedef struct servo_device_s {
    servo_t servo;

}servo_device_t;

void servo_device_init(servo_device_t *servo_device, int pwm, int channel);
void servo_device_set_position(servo_device_t * servo_device, int degree);
#endif //GREENHOUSE_SERVO_DEVICE_H

