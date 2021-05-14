//
// Created by Luca Giovannesi.
//

#ifndef RIOT_GREENHOUSE_EMCUTE_CONNECTION_H
#define RIOT_GREENHOUSE_EMCUTE_CONNECTION_H

#include "stddef.h"
#include "stdint.h"
#include "../devices/devices_manager.h"
#include "../RIOT/sys/include/net/emcute.h"

int set_connection(char *server_addr, int node_id);
void emcute_publish(char *str);
int get_node_id(void);
int is_connected(void);
#endif //RIOT_GREENHOUSE_EMCUTE_CONNECTION_H
