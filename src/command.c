//
// Created by Luca Giovannesi.
//
#include "command.h"

#include "../devices/digital_out.h"
#include "../devices/dht11.h"
#include "../devices/analog_device.h"
#include "../devices/devices_manager.h"
#include "../devices/servo_device.h"

#include "../RIOT/sys/include/shell.h"

#include <stdio.h>

#include "shell.h"
#include "msg.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

extern int start_udp_server(int argc, char **argv);

extern int connect_udp_server(int argc, char **argv);

extern int stop_udp_server(int argc, char **argv);

int print_temperature(int argc, char **argv) {
    (void) argc;
    (void) argv;
    dht11_device_t *dht = device_manager_get_device(TEMP_HUM);
    printf("Temperature %d °C\n", (int) dht11_get_temp(dht));
    return 0;
}

int print_humidity(int argc, char **argv) {
    (void) argc;
    (void) argv;
    dht11_device_t *dht = device_manager_get_device(TEMP_HUM);
    printf("Humidity %d %%\n", (int) dht11_get_hum(dht));
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
    printf("Water level %d %%\n", device->scaled);
    return 0;
}

int print_soil_moisture(int argc, char **argv) {
    (void) argc;
    (void) argv;
    analog_device_t *device = device_manager_get_device(SOIL_MOISTURE);
    printf("Soil level %d %%\n", device->scaled);
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
    servo_device_set_manual_override(device, degree);
    if (degree == -1) {
        //servo_device_set_position(device, degree);
        servo_device_clear_manual_override(device);
    }
    printf("Servo setted to %d\n", degree);
    return 0;
}

#include "emcute_connection.h"

int connect_mqtt(int argc, char **argv) {
    (void) argc;
    (void) argv;
    if (argc > 2) {
        set_connection(argv[1],atoi(argv[2]));
    }
    else{
        printf("Usage: %s <broker_addr> <node_id>\n",argv[0]);
    }
    return 0;
}

static const shell_command_t shell_commands[] = {
        {"temp",        "print env. temperature",    print_temperature},
        {"hum",         "print env. humidity",       print_humidity},
        {"toggle_pump", "Toggle water pump",         pump_toggle},
        {"soil",        "Print soil humidity",       print_soil_moisture},
        {"level",       "Print water level",         print_water_level},
        {"servo",       "Move servo",                set_servo_position},

        //{"start_server","Start the UDP server",                       start_udp_server},
        {"connect",     "Connect to the UDP server", connect_mqtt},
        //{"stop_server", "Stop the UDP server",                        stop_udp_server},

        {NULL,          NULL,                        NULL}
};


void command_wait_for_command(void) {
    /* we need a message queue for the thread running the shell in order to
     * receive potentially fast incoming networking packets */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    puts("RIOT network stack example application");

    /* start shell */
    puts("All up, running the shell now");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    /* should be never reached */
}