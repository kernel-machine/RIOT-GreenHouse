//
// Created by Luca Giovannesi.
//

#ifndef RIOT_GREENHOUSE_EMCUTE_CONNECTION_H
#define RIOT_GREENHOUSE_EMCUTE_CONNECTION_H

#include "stddef.h"
#include "stdint.h"
#include "../devices/devices_manager.h"
#include "../RIOT/sys/include/net/emcute.h"

void init_connection(void);

void emcute_publish(char *str);

#endif //RIOT_GREENHOUSE_EMCUTE_CONNECTION_H
