//
// Created by Luca Giovannesi.
//

#include "digital_out.h"

void digital_out_init(digital_out_t *device, gpio_t pin) {
    device->pin = pin;
    gpio_init(pin, GPIO_OUT);
    digital_out_disable(device);
}

void digital_out_enable(digital_out_t *device) {
    gpio_set(device->pin);
    device->state = 1;
}

void digital_out_disable(digital_out_t *device) {
    gpio_clear(device->pin);
    device->state = 0;
}

void digital_out_toggle(digital_out_t *device) {
    if (device->state)
        digital_out_disable(device);
    else
        digital_out_enable(device);
}

bool digital_out_is_enabled(digital_out_t *device) {
    return device->state;
}
