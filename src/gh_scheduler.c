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
#include "convertion.h"

#define MAX_FUNCTION 4

char stack[THREAD_STACKSIZE_MAIN];

int analog_device_last = -1;
uint32_t last_analog_time_scan = 0;

scheduled_function_t functions[MAX_FUNCTION];

static void real_time_operation(void) {
    //Real time
    for (uint8_t i = 0; i < DEVICE_NUMBER; i++) {
        if (device_manager_must_be_scanned(i)) {
            device_manager_scan(i);
        }
    }
    logic_condition_update();
}

void *green_house_core(void *arg) {
    (void) arg;

    while (1) {
        for (int i = 0; i < MAX_FUNCTION; i++) {
            if (functions[i].active && US2MS(xtimer_now_usec()) > functions[i].next_schedule) {
                functions[i].function();
                functions[i].next_schedule = US2MS(xtimer_now_usec()) + functions[i].interval;
            }
        }
        real_time_operation();
        xtimer_msleep(15);
    }
    return NULL;
}

void green_house_scheduler_init(void) {
    xtimer_init();
    for (int i = 0; i < MAX_FUNCTION; i++) {
        functions[i].active = 0;
    }
}

void green_house_add_function(uint16_t interval, void (*function)(void)) {
    for (int i = 0; i < MAX_FUNCTION; i++) {
        if (functions[i].active == 0) {
            functions[i].interval = interval;
            functions[i].function = function;
            functions[i].next_schedule = US2MS(xtimer_now_usec()) + interval;
            functions[i].active = 1;
            break;
        }
    }
}

void green_house_scheduler_start(void) {
    thread_create(stack, sizeof(stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  green_house_core,
                  NULL, "sensor thread");
}