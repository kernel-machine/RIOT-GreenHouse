//
// Created by Luca Giovannesi.
//

#include "digital_out.h"
#include "../main/convertion.h"

#define HYSTERESIS_THRESHOLD 1000

void digital_out_init(digital_out_t *device, gpio_t pin) {
    device->pin = pin;
    device->enable_hysteresis = 0;
    device->last_activation = 0;
    device->is_timed_active = 0;
#ifndef FAKE_SENSOR
    gpio_init(pin, GPIO_OUT);
#endif
    digital_out_disable(device);
    xtimer_init();
}

static int can_be_toggled(digital_out_t *device) {
    return !device->enable_hysteresis || US2MS(xtimer_now_usec()) - device->last_activation > HYSTERESIS_THRESHOLD;
}

void digital_out_enable(digital_out_t *device) {
    if (can_be_toggled(device) && !device->is_timed_active) {
#ifndef FAKE_SENSOR
        gpio_set(device->pin);
#endif
        device->state = 1;
    }
}

void digital_out_disable(digital_out_t *device) {
    if (can_be_toggled(device) && !device->is_timed_active) {
#ifndef FAKE_SENSOR
        gpio_clear(device->pin);
#endif
        device->state = 0;
    }
}

void digital_out_toggle(digital_out_t *device) {
    if (!can_be_toggled(device) && device->is_timed_active)
        return;
    if (device->state)
        digital_out_disable(device);
    else
        digital_out_enable(device);
}

bool digital_out_is_enabled(digital_out_t *device) {
    return device->state;
}

void digital_out_update(digital_out_t *device) {
    if(!device->is_timed_active)
        return;

    if (US2MS(xtimer_now_usec()) > device->last_activation) {
        device->is_timed_active = 0;
#ifndef FAKE_SENSOR
        gpio_clear(device->pin);
#endif
        device->state = 0;
    } else {
#ifndef FAKE_SENSOR
        gpio_set(device->pin);
#endif
        device->state = 1;
    }
}

void digital_out_enable_for_x_ms(digital_out_t *device, int ms) {
    device->is_timed_active = 1;
    device->last_activation = US2MS(xtimer_now_usec()) + ms;
}

int digital_out_get_current_state(digital_out_t *device) {
    return device->state;
}

