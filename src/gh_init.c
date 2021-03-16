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
#include "sensor_scan.h"
#include "timer_conversion.h"

#include "../RIOT/sys/include/xtimer.h"
#include "../RIOT/core/include/thread.h"

#include "stdio.h"


dht11_device_t dht;
analog_device_t water_level;
analog_device_t soil_moisture;
digital_out_t pump;
digital_out_t water_level_power;

servo_device_t servo;

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
    device_manager_set_scan_interval(WATER_LEVEL, S2MS(2));
    device_manager_set_scan_interval(SOIL_MOISTURE, S2MS(2));

    start_sensor_scan();

    command_wait_for_command();
}
