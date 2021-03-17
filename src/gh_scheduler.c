//
// Created by Luca Giovannesi.
//

#include "gh_scheduler.h"
#include "thread.h"
#include "stdint.h"

#include "../devices/devices_manager.h"
#include "../devices/analog_device.h"
#include "../devices/dht11.h"

#include "../RIOT/sys/include/xtimer.h"
#include "logic_condition.h"


char stack[THREAD_STACKSIZE_MAIN];

int analog_device_last = -1;
uint32_t last_analog_time_scan = 0;


void *sensor_scanning_loop(void *arg) {
    (void) arg;
    while (1) {
        for (uint8_t i = 0; i < DEVICE_NUMBER; i++) {
            if (device_manager_must_be_scanned(i)) {
                device_manager_scan(i);
            }
        }
        logic_condition_update();
        xtimer_msleep(200);
    }
    return NULL;

}

void green_house_scheduler_start(void) {
    thread_create(stack, sizeof(stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  sensor_scanning_loop,
                  NULL, "sensor thread");
}