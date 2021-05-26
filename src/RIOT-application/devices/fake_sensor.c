//
// Created by Luca Giovannesi.
//
#include "fake_sensor.h"

#ifdef FAKE_SENSOR

void fake_sensor_init(fake_sensor_t *fakeSensor, int min, int max, time_t intervalMs, unsigned int step) {
    if (fakeSensor) {
        fakeSensor->min = min;
        fakeSensor->max = max;
        fakeSensor->editIntervalMs = intervalMs;
        fakeSensor->step = step;
        fakeSensor->direction = (random_uint32_range(0,1000) > 500 ? 1:-1);
        fakeSensor->lastEditUs = xtimer_now_usec();
        fakeSensor->value = random_uint32_range(min,max);
    }
}

void fake_sensor_update(fake_sensor_t *fakeSensor) {
    if (US2MS(xtimer_now_usec() - fakeSensor->lastEditUs) > fakeSensor->editIntervalMs) {
        fakeSensor->value += fakeSensor->step * fakeSensor->direction;
        fakeSensor->lastEditUs = xtimer_now_usec();

        if (fakeSensor->value < fakeSensor->min || fakeSensor->value > fakeSensor->max)
            fakeSensor->direction *= -1;
    }

}

int fake_sensor_read(fake_sensor_t *fakeSensor) {
    return fakeSensor->value;
}

#endif
