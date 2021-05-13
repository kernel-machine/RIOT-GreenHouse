#ifndef RIOT_GREENHOUSE_FAKE_SENSOR_H
#define RIOT_GREENHOUSE_FAKE_SENSOR_H

#include "../RIOT/sys/include/xtimer.h"
#include "random.h"
#include "../src/convertion.h"

typedef struct fake_sensor_s {
    int direction;
    int max;
    int min;
    unsigned int step;
    time_t lastEditUs;
    time_t editIntervalMs;
    int value;
} fake_sensor_t;

void fake_sensor_init(fake_sensor_t *fakeSensor, int min, int max, time_t intervalMs, unsigned int step);

void fake_sensor_update(fake_sensor_t *fakeSensor);

int fake_sensor_read(fake_sensor_t *fakeSensor);

#endif //RIOT_GREENHOUSE_FAKE_SENSOR_H
