//
// Created by Luca Giovannesi.
//

#ifndef GREENHOUSE_DIGITAL_OUT_H
#define GREENHOUSE_DIGITAL_OUT_H

#include "../RIOT/drivers/include/periph/gpio.h"

typedef struct digital_out_s {
    gpio_t pin;
    bool state;
}digital_out_t;

void digital_out_init(digital_out_t * device, gpio_t pin);
void digital_out_enable(digital_out_t * device);
void digital_out_disable(digital_out_t * device);
void digital_out_toggle(digital_out_t * device);
bool digital_out_is_enabled(digital_out_t * device);
#endif //GREENHOUSE_DIGITAL_OUT_H
