//
// Created by luca on 15/03/21.
//

#include "gh_init.h"

#include "../devices/dht11.h"
#include "../devices/analog_device.h"
#include "../devices/digital_out.h"
#include "../devices/devices_manager.h"
#include "../devices/servo_device.h"

#include "command.h"
#include "gh_scheduler.h"
#include "convertion.h"
#include "logic_condition.h"

#include "../RIOT/sys/include/xtimer.h"
#include "../RIOT/core/include/thread.h"

#include "stdio.h"
#include "stdlib.h"


dht11_device_t dht;
analog_device_t water_level;
analog_device_t soil_moisture;
digital_out_t pump;
digital_out_t water_level_power;

servo_device_t servo;

void toggle_pump(int *a) {
    if (a == NULL)
        return;
    else if (*a == 0)
        digital_out_disable(&pump);
    else if (*a == 1)
        digital_out_enable(&pump);
    else {
        int ms = ML2MS(*a);
        printf("ENABLE_PUMP_FOR %d\n", ms);
        digital_out_enable_for_x_ms(&pump, ms);
    }
}

void toggle_roof(int *a) {
    if (a == NULL)
        return;
    else if (*a == 0)
        servo_device_set_position(&servo, 0);
    else if (*a == 1)
        servo_device_set_position(&servo, 180);

    //printf("SERVO %d\n", *a);

}


void gh_init(void) {
    xtimer_init();

    dht11_init(&dht, GPIO_PIN(PORT_A, 10));

    analog_device_init(&water_level, ADC_LINE(1));
    analog_device_init(&soil_moisture, ADC_LINE(2));

    digital_out_init(&pump, GPIO_PIN(PORT_A, 8));
    digital_out_init(&water_level_power, GPIO_PIN(PORT_B, 4));

    servo_device_init(&servo, 0, 1);

    device_manager_add(TEMP_HUM, &dht);
    device_manager_add(WATER_LEVEL, &water_level);
    device_manager_add(WATER_LEVEL_POWER, &water_level_power);
    device_manager_add(SOIL_MOISTURE, &soil_moisture);
    device_manager_add(PUMP, &pump);
    device_manager_add(SERVO, &servo);

    device_manager_set_scan_interval(TEMP_HUM, S2MS(2));
    device_manager_set_scan_interval(WATER_LEVEL, S2MS(1));
    device_manager_set_scan_interval(SOIL_MOISTURE, S2MS(2));
    device_manager_set_scan_interval(PUMP, 200);

    int water_level_threshold = 15;
    int soil_moisture_threshold = 40;
    int hum_threshold = 350;
    int ms = 1000;
    int enable = 1;
    int disable = 0;

    const int *water_level_pointer = (const int *) &(water_level.scaled);
    const int *soil_moisture_pointer = (const int *) &(soil_moisture.scaled);
    const int *green_house_hum_ptr = &(dht.last_hum);

    logic_condition_init();

    const logic_condition_t *lc_enable_pump1 =
            logic_condition_add(water_level_pointer, GREATER, &water_level_threshold, NULL, NULL, NULL);

    const logic_condition_t *lc_enable_pump2 = logic_condition_add(soil_moisture_pointer, LESS,
                                                                   &soil_moisture_threshold,
                                                                   toggle_pump, &ms,
                                                                   lc_enable_pump1);

    logic_condition_set_interval(lc_enable_pump2,S2MS(60));

    logic_condition_add(green_house_hum_ptr, GREATER, &hum_threshold, toggle_roof, &enable, NULL);
    logic_condition_add(green_house_hum_ptr, LESS, &hum_threshold, toggle_roof, &disable, NULL);

    green_house_scheduler_start();

    command_wait_for_command();
}
