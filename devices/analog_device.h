//
// Created by Luca Giovannesi.
//

#ifndef ANALOG_DEVICE_H
#define ANALOG_DEVICE_H

#include "analog_util.h"
#include "periph/adc.h"

#define ADC_RESOLUTION ADC_RES_12BIT

typedef struct analog_device_s {
    adc_t adc_line;
    int32_t raw_data;
    int max_value;
    int min_value;
    int scaled;
} analog_device_t;

void analog_device_init(analog_device_t *device, adc_t adc);

void analog_device_update(analog_device_t *device);

void analog_device_set_minmax(analog_device_t *device, int min, int max);

int32_t analog_device_get_data(analog_device_t *device);

#endif //ANALOG_DEVICE_H
