//
// Created by Luca Giovannesi.
//

#ifndef GREENHOUSE_DEVICE_H
#define GREENHOUSE_DEVICE_H

#include "stdint.h"
#include "stdlib.h"
#include "xtimer.h"

typedef enum device_type {
    PUMP = 0,
    WATER_LEVEL = 1,
    WATER_LEVEL_POWER = 2,
    SOIL_MOISTURE = 3,
    TEMP_HUM = 4,
    SERVO = 6,
    DEVICE_NUMBER = 7
} device_type_e;

typedef struct device_specs_s {
    device_type_e device_type;
    bool is_analog;
    bool is_sensor;
    char * name;
} device_specs_t;

typedef struct device_s {
    void *device;
    uint32_t scan_interval;
    time_t last_scan;
} device_t;

void device_manager_add(device_type_e device_type, void *device);

void device_manager_set_is_analog(device_type_e device_type, uint8_t is_analog);

void device_manager_set_is_sensor(device_type_e device_type, uint8_t is_sensor);

uint8_t device_manager_is_analog(device_type_e device_type);

uint8_t device_manager_is_sensor(device_type_e device_type);

void *device_manager_get_device(device_type_e device_type);

void device_manager_set_scan_interval(device_type_e device_type, uint32_t scan_interval);

bool device_manager_must_be_scanned(device_type_e device_type);

void device_manager_scan(device_type_e device_type);

char *device_manager_get_device_name(device_type_e device_type);
#endif //GREENHOUSE_DEVICE_H
