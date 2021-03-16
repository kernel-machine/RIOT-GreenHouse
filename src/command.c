//
// Created by Luca Giovannesi.
//

#include "command.h"

int print_temperature(int argc, char **argv) {
    (void) argc;
    (void) argv;
    dht11_device_t *dht = device_manager_get_device(TEMP_HUM);
    printf("Temperature %d\n", dht->last_temp);
    return 0;
}

int print_humidity(int argc, char **argv) {
    (void) argc;
    (void) argv;
    dht11_device_t *dht = device_manager_get_device(TEMP_HUM);
    printf("Humidity %d\n", dht->last_hum);
    return 0;
}

int pump_toggle(int argc, char **argv) {
    (void) argc;
    (void) argv;
    digital_out_t *pump = device_manager_get_device(PUMP);
    digital_out_toggle(pump);
    printf("Toogled");
    return 0;
}

int print_water_level(int argc, char **argv) {
    (void) argc;
    (void) argv;
    analog_device_t *device = device_manager_get_device(WATER_LEVEL);
    printf("Water level %ld\n", analog_device_get_data(device));
    return 0;
}

int print_soil_moisture(int argc, char **argv) {
    (void) argc;
    (void) argv;
    analog_device_t *device = device_manager_get_device(SOIL_MOISTURE);
    printf("Soil level %ld\n", analog_device_get_data(device));
    return 0;
}

int set_servo_position(int argc, char **argv) {
    (void) argc;
    (void) argv;
    if (argc < 2) {
        printf("Invalid input\nUse servo [degree]\n");
        return 0;
    }
    servo_device_t *device = device_manager_get_device(SERVO);
    int degree = atoi(argv[1]);
    servo_device_set_position(device, degree);
    printf("Servo setted to %d\n", degree);
    return 0;
}

static const shell_command_t commands[] = {
        {"temp",        "print env. temperature", print_temperature},
        {"hum",         "print env. humidity",    print_humidity},
        {"toggle_pump", "Toggle water pump",      pump_toggle},
        {"soil",        "Print soil humidity",    print_soil_moisture},
        {"level",       "Print water level",      print_water_level},
        {"servo",       "Move servo",             set_servo_position},

        {NULL,          NULL,                     NULL}
};

void command_wait_for_command(void) {
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(commands, line_buf, SHELL_DEFAULT_BUFSIZE);
}