//
// Created by Luca Giovannesi.
//

#ifndef RIOT_GREENHOUSE_EMCUTE_CONNECTION_H
#define RIOT_GREENHOUSE_EMCUTE_CONNECTION_H

#include "stddef.h"
#include "stdint.h"
#include "../devices/devices_manager.h"
#include "../RIOT/sys/include/net/emcute.h"

typedef enum {
    TOPIC_HUMIDITY,
    TOPIC_TEMPERATURE,
    TOPIC_SOIL,
    TOPIC_WATER_LEVEL
} topic_e;

typedef struct topic_device_s {
    topic_e device;
    char *name;
    emcute_topic_t *topic_pointer;
} topic_device_t;

void init_connection(void);
void emcute_publish(topic_e topic, int value);

#endif //RIOT_GREENHOUSE_EMCUTE_CONNECTION_H
