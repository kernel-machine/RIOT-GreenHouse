//
// Created by Luca Giovannesi.
//

#ifndef GREENHOUSE_COMMAND_H
#define GREENHOUSE_COMMAND_H

#include "../devices/digital_out.h"
#include "../devices/dht11.h"
#include "../devices/analog_device.h"
#include "../devices/devices_manager.h"
#include "../devices/servo_device.h"

#include "../RIOT/sys/include/shell.h"
#include "string.h"
#include "stdlib.h"

void command_wait_for_command(void);
#endif //GREENHOUSE_COMMAND_H
