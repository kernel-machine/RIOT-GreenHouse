//
// Created by Luca Giovannesi.
//

#ifndef GREENHOUSE_SENSOR_SCAN_H
#define GREENHOUSE_SENSOR_SCAN_H

#include "../RIOT/sys/include/xtimer.h"

typedef struct scheduled_function_s{
    time_t next_schedule;
    time_t interval;
    void (*function)(void);
    uint8_t active;
}scheduled_function_t;

void green_house_scheduler_init(void);
void green_house_add_function(uint16_t intervalms, void (*function)(void));
void green_house_scheduler_start(void);

#endif //GREENHOUSE_SENSOR_SCAN_H
