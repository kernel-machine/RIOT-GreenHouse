//
// Created by Luca Giovannesi.
//

#include "analog_device.h"

void analog_device_init(analog_device_t * device, adc_t adc) {
    device->adc_line = adc;
    device->raw_data = 0;
    adc_init(adc);
}

void analog_device_update(analog_device_t * device) {
    device->raw_data = adc_sample(device->adc_line, ADC_RESOLUTION);
}

int32_t analog_device_get_data(analog_device_t * device) {
    return device->raw_data;
}
