//
// Created by Luca Giovannesi.
//

#include "devices_manager.h"

#include "../src/emcute_connection.h"
#include "../src/convertion.h"

#include "analog_device.h"
#include "dht11.h"
#include "digital_out.h"

#include "string.h"


device_t devices[DEVICE_NUMBER];

const device_specs_t deviceSpecs[] = {
        //{DEVICE, isAnalog, isSensor, name}
        {PUMP,          0, 0, "pump"},
        {WATER_LEVEL,   1, 1, "water_level"},
        {SOIL_MOISTURE, 1, 1, "soil_moisture"},
        {TEMP_HUM,      0, 1, "temp_humidity"},
        {SERVO,         0, 0, "servo"}
};

char *device_manager_get_device_name(device_type_e device_type) {
    for (int i = 0; i < DEVICE_NUMBER; i++) {
        if (deviceSpecs[i].device_type == device_type)
            return deviceSpecs[i].name;
    }
    return "not_found";
}

void device_manager_add(device_type_e device_type, void *device) {
    devices[device_type].device = device;
    devices[device_type].scan_interval = 0;
    devices[device_type].last_scan = 0;
    xtimer_init();
}

static const device_specs_t *get_device_spec(device_type_e device_type) {
    for (uint8_t i = 0; i < DEVICE_NUMBER; i++) {
        if (deviceSpecs[i].device_type == device_type)
            return deviceSpecs + i;
    }
    return NULL;
}


uint8_t device_manager_is_analog(device_type_e device_type) {
    const device_specs_t *spec = get_device_spec(device_type);
    if (spec == NULL)
        return 0;
    return spec->is_analog;
}

void *device_manager_get_device(device_type_e device_type) {
    return devices[device_type].device;
}

uint8_t device_manager_is_sensor(device_type_e device_type) {
    const device_specs_t *spec = get_device_spec(device_type);
    if (spec == NULL)
        return 0;
    return spec->is_sensor;
}

void device_manager_set_scan_interval(device_type_e device_type, uint32_t scan_interval) {
    devices[device_type].scan_interval = scan_interval;
}

void device_manager_scan(device_type_e device_type) {
    void *device = device_manager_get_device(device_type);

    switch (device_type) {
        case WATER_LEVEL:
            digital_out_enable(device);
            xtimer_msleep(1);
            analog_device_update(device);
            digital_out_disable(device);

            emcute_publish(TOPIC_WATER_LEVEL, ((analog_device_t *) device)->scaled);

            break;
        case SOIL_MOISTURE:
            analog_device_update(device);

            emcute_publish(TOPIC_SOIL, ((analog_device_t *) device)->scaled);

            break;
        case TEMP_HUM:
            dht11_update(device);
            emcute_publish(TOPIC_TEMPERATURE, ((dht11_device_t *) device)->last_temp);
            emcute_publish(TOPIC_HUMIDITY, ((dht11_device_t *) device)->last_hum);
            break;
        case PUMP:
            digital_out_update(device);
            break;
        default:
            break;
    }


    if (device_manager_is_sensor(device_type)) {
        devices[device_type].last_scan = US2MS(xtimer_now_usec());
    }
}

bool device_manager_must_be_scanned(device_type_e device_type) {
    return US2MS(xtimer_now_usec()) - devices[device_type].last_scan > devices[device_type].scan_interval;
}
