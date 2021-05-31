//
// Created by Luca Giovannesi.
//

#include "analog_device.h"

#define POW2(x) (2<<(x-1))
#define RAW2SCALED(x, min, max) (((x*max)/POW2(12))+min)

void analog_device_init(analog_device_t *device, adc_t adc) {
    device->adc_line = adc;
    device->raw_data = 0;
    device->scaled = 0;
    device->max_value = 100;
    device->min_value = 0;
    adc_init(adc);
}

void analog_device_update(analog_device_t *device) {
    device->raw_data = adc_sample(device->adc_line, ADC_RESOLUTION);
    device->scaled = RAW2SCALED(device->raw_data, device->min_value, device->max_value);
}

int32_t analog_device_get_data(analog_device_t *device) {
    return device->raw_data;
}

void analog_device_set_minmax(analog_device_t *device, int min, int max) {
    device->max_value = max;
    device->min_value = min;
}

uint16_t analog_device_get_mv(analog_device_t * device){
    return RAW2SCALED(device->raw_data, 0, 5000);;
}