//
// Created by Luca Giovannesi.
//

#ifndef GREENHOUSE_SERVO_DEVICE_H
#define GREENHOUSE_SERVO_DEVICE_H

#include "../../../RIOT/drivers/include/servo.h"
#include "../../../RIOT/drivers/include/periph/gpio.h"

typedef struct servo_device_s {
    servo_t servo;
    int degree;
    uint8_t manual_override;

}servo_device_t;

void servo_device_init(servo_device_t *servo_device, int pwm, int channel);
void servo_device_set_position(servo_device_t * servo_device, int degree);
int servo_device_get_position(servo_device_t * servo_device);
int servo_device_is_manual_override(servo_device_t * servo_device);
void servo_device_set_manual_override(servo_device_t *servo_device, int degree);
void servo_device_clear_manual_override(servo_device_t *servo_device);
#endif //GREENHOUSE_SERVO_DEVICE_H

