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

#include "udp.h"

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
    printf("Water level %d\n", device->scaled);
    return 0;
}

int print_soil_moisture(int argc, char **argv) {
    (void) argc;
    (void) argv;
    analog_device_t *device = device_manager_get_device(SOIL_MOISTURE);
    printf("Soil level %d\n", device->scaled);
    return 0;
}

int startUDPServer(int argc, char **argv) {
    (void) argc;
    (void) argv;
    char *server_port = UDP_SERVER_PORT;
    start_server(server_port);
    return 0;
}

int stopUDPServer(int argc, char **argv) {
    (void) argc;
    (void) argv;
    stop_server();
    return 0;
}

int connectToUDP(int argc, char **argv) {
    if (argc < 2)
        printf("Usage - connect ip6_server_address");
    else
        setServerAddress(argv[1]);

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

static const shell_command_t shell_commands[] = {
        {"temp",         "print env. temperature",   print_temperature},
        {"hum",          "print env. humidity",      print_humidity},
        {"toggle_pump",  "Toggle water pump",        pump_toggle},
        {"soil",         "Print soil humidity",      print_soil_moisture},
        {"level",        "Print water level",        print_water_level},
        {"servo",        "Move servo",               set_servo_position},
        {"start_server", "Start the UDP server",     startUDPServer},
        {"stop_server",  "Stop the UDP server",      stopUDPServer},
        {"connect",      "Connect to an UDP server", connectToUDP},

        {NULL,           NULL,                       NULL}
};

void command_wait_for_command(void) {
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
}