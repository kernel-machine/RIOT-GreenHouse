//
// Created by Luca Giovannesi.
//

#include "sensor_scan.h"
#include "thread.h"
#include "stdint.h"

#include "../devices/devices_manager.h"
#include "../devices/analog_device.h"
#include "../devices/dht11.h"

#include "../RIOT/sys/include/xtimer.h"


char stack[THREAD_STACKSIZE_MAIN];

int analog_device_last = -1;
uint32_t last_analog_time_scan = 0;

/*
void sample_analog_devices(device_type_e current_device) {
    const uint8_t elapsedEnoughTime = xtimer_now_usec() - last_analog_time_scan > ANALOG_DEVICE_SCAN_INTERVAL;
    const uint8_t differentFromPrevious = current_device != analog_device_last;

    if (elapsedEnoughTime && differentFromPrevious) {
        analog_device_t *analog_dev = device_manager_get_device(current_device);
        analog_device_update(analog_dev);

        last_analog_time_scan = xtimer_now_usec();
        analog_device_last = current_device;
    }

}

void sample_digital_devices(device_type_e dev) {
    void *dev_pointer = device_manager_get_device(dev);
    switch (dev) {
        case TEMP_HUM:
            dht11_update(dev_pointer);
            break;
        default:
            break;
    }
}
*/
void *sensor_scanning_loop(void *arg) {
    (void) arg;
    while (1) {
        for (uint8_t i = 0; i < DEVICE_NUMBER; i++) {
            if (device_manager_is_sensor(i) && device_manager_must_be_scanned(i)) {
                /*
                if (device_manager_is_analog(i)) {
                    sample_analog_devices(i);
                } else
                    sample_digital_devices(i);
                    */
                device_manager_scan(i);
            }
        }
        xtimer_msleep(200);
    }
    return NULL;

}

void start_sensor_scan(void) {
    thread_create(stack, sizeof(stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  sensor_scanning_loop,
                  NULL, "sensor thread");
}