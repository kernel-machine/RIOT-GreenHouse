//
// Created by Luca Giovannesi.
//

#ifndef GREENHOUSE_DIGITAL_OUT_H
#define GREENHOUSE_DIGITAL_OUT_H

#include "../RIOT/drivers/include/periph/gpio.h"
#include "../RIOT/sys/include/xtimer.h"
#include "../RIOT/core/include/thread.h"

typedef struct digital_out_s {
    gpio_t pin;
    bool state;
    int enable_hysteresis;
    uint32_t last_activation;
    int is_timed_active;
}digital_out_t;

void digital_out_init(digital_out_t * device, gpio_t pin);
void digital_out_enable(digital_out_t * device);
void digital_out_disable(digital_out_t * device);
void digital_out_toggle(digital_out_t * device);
bool digital_out_is_enabled(digital_out_t * device);
void digital_out_enable_for_x_ms(digital_out_t *device,int ms);
void digital_out_update(digital_out_t *device);
int digital_out_get_current_state(digital_out_t *device);
#endif //GREENHOUSE_DIGITAL_OUT_H
