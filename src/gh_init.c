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
#include "emcute_connection.h"
#include "random.h"

#include "xtimer.h"
#include "thread.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define DHT_PIN                 GPIO_PIN(PORT_A, 10)    //D2
#define WATER_LEVEL_ADC         ADC_LINE(0)             //A0
#define SOIL_MOISTURE_ADC       ADC_LINE(1)             //A1
#define RELAY_PIN               GPIO_PIN(PORT_A, 8)     //D7
#define WATER_LEVEL_POWER_PIN   GPIO_PIN(PORT_B, 4)     //D5

#define SERVO_CHANNEL           1
#define SERVO_PWM               0

#define MQTT_PUBLISH_RATE       60  //One message every 60 seconds

dht11_device_t dht;
analog_device_t water_level;
analog_device_t soil_moisture;
digital_out_t pump;
digital_out_t water_level_power;

servo_device_t servo;

#define BUFFER_SIZE 1024
char str[BUFFER_SIZE];

void toggle_pump(int *a) {
    if (a == NULL)
        return;
    else if (*a == 0)
        digital_out_disable(&pump);
    else if (*a == 1)
        digital_out_enable(&pump);
    else {
        digital_out_enable_for_x_ms(&pump, *a);
    }
}

void toggle_roof(int *a) {
    if (a == NULL)
        return;
    if (servo_device_is_manual_override(&servo))
        return;

    else if (*a == 0)
        servo_device_set_position(&servo, 0);
    else if (*a == 1)
        servo_device_set_position(&servo, 180);
}

static void scan_device_and_update_lc(void) {
    //Real time
    for (uint8_t i = 0; i < DEVICE_NUMBER; i++) {
        if (device_manager_must_be_scanned(i)) {
            device_manager_scan(i);
        }
    }
    logic_condition_update();
}

void publish_topic(void) {

    const char out_size = 16;
    char out[out_size];

    memset(str, 0, BUFFER_SIZE);
    strcat(str, "{");

    memset(out, 0, out_size);
    strcat(str, "\"message_id\":");
    sprintf(out, "%d\n", (int) US2S(xtimer_now_usec()));
    strcat(str, out);
    strcat(str, ",");

    for (int i = 0; i < DEVICE_NUMBER; i++) {
        void *device = device_manager_get_device((device_type_e) i);


        switch (i) {
            case PUMP:
                memset(out, 0, out_size);
                strcat(str, "\"pump\":");
                sprintf(out, "%d\n", digital_out_get_current_state(device));
                strcat(str, out);
                strcat(str, ",");
                break;
            case WATER_LEVEL:
                memset(out, 0, out_size);
                strcat(str, "\"water_level\":");
                sprintf(out, "%d\n", ((analog_device_t *) device)->scaled);
                strcat(str, out);
                strcat(str, ",");
                break;
            case SOIL_MOISTURE:
                memset(out, 0, out_size);
                strcat(str, "\"soil\":");
                sprintf(out, "%d\n", ((analog_device_t *) device)->scaled);
                strcat(str, out);
                strcat(str, ",");
                break;
            case TEMP_HUM:
                memset(out, 0, out_size);
                strcat(str, "\"temp\":");
                sprintf(out, "%d\n", (int) dht11_get_temp(device));
                strcat(str, out);
                strcat(str, ",");

                memset(out, 0, out_size);
                strcat(str, "\"hum\":");
                sprintf(out, "%d\n", (int) dht11_get_hum(device));
                strcat(str, out);
                strcat(str, ",");
                break;
            case SERVO:
                memset(out, 0, out_size);
                strcat(str, "\"servo\":");
                sprintf(out, "%d\n", (int) servo_device_get_position(device));
                strcat(str, out);
                strcat(str, ",");
                break;
            default:
                break;

        }


    }

    memset(out, 0, out_size);
    strcat(str, "\"nodeId\":");
    sprintf(out, "%d\n", (int) get_node_id());
    strcat(str, out);

    strcat(str, "}");
    emcute_publish(str);

}


void gh_init(void) {
    xtimer_init();
    random_init(RANDOM_SEED_DEFAULT);

    dht11_init(&dht, DHT_PIN);

    analog_device_init(&water_level, WATER_LEVEL_ADC);
    analog_device_init(&soil_moisture, SOIL_MOISTURE_ADC);

    digital_out_init(&pump, RELAY_PIN);
    digital_out_init(&water_level_power, WATER_LEVEL_POWER_PIN);

    servo_device_init(&servo, SERVO_PWM, SERVO_CHANNEL);

    device_manager_add(TEMP_HUM, &dht);
    device_manager_add(WATER_LEVEL, &water_level);
    device_manager_add(WATER_LEVEL_POWER, &water_level_power);
    device_manager_add(SOIL_MOISTURE, &soil_moisture);
    device_manager_add(PUMP, &pump);
    device_manager_add(SERVO, &servo);

    device_manager_set_scan_interval(TEMP_HUM, S2MS(DHT_INTERVAL));
    device_manager_set_scan_interval(WATER_LEVEL, S2MS(WATER_LEVEL_INTERVAL));
    device_manager_set_scan_interval(SOIL_MOISTURE, S2MS(SOIL_INTERVAL));
    device_manager_set_scan_interval(PUMP, 200);    //A bit of hysteresis between activations

    //Logic condition definitions
    int water_level_threshold = MIN_WATER_LEVEL;
    int soil_moisture_threshold = MIN_SOIL;
    int hum_threshold = MAX_HUM * 10;
    int ms = S2MS(ML2S(WATER_TO_PUMP));
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

    //Check interval for this logic condition, 0 by default
    logic_condition_set_interval(lc_enable_pump2, S2MS(PUMP_INTERVAL));

    logic_condition_add(green_house_hum_ptr, GREATER, &hum_threshold, toggle_roof, &enable, NULL);
    logic_condition_add(green_house_hum_ptr, LESS, &hum_threshold, toggle_roof, &disable, NULL);
    //End logic condition

    green_house_scheduler_init();

    green_house_add_function(S2MS(MQTT_PUBLISH_RATE), publish_topic);

    //Scan device is done as soon as possible, but the device manager reads from sensor only if needed.
    green_house_add_function(0, scan_device_and_update_lc);

    //Starts to scan sensor and logic conditions
    green_house_scheduler_start();

    command_wait_for_command();
}
