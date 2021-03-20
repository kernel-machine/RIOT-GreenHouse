//
// Created by Luca Giovannesi.
//

#include "emcute_connection.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "../devices/devices_manager.h"

#ifndef EMCUTE_ID
#define EMCUTE_ID           ("gertrud")
#endif
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)

#define NUMOFSUBS           (1U)
#define TOPIC_MAXLEN        (64U)

#define COUNT(x)            sizeof(x)/sizeof(x[0])

static char stack[THREAD_STACKSIZE_DEFAULT];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

topic_device_t topic_devices[] = {
        {TOPIC_HUMIDITY,    "HUMI", NULL},
        {TOPIC_TEMPERATURE, "TEMP", NULL},
        {TOPIC_SOIL,        "SOIL", NULL},
        {TOPIC_WATER_LEVEL, "WATL", NULL}
};

emcute_topic_t emcute_topics[COUNT(topic_devices)];

static void emcute_register_sensors_topics(void) {
    for (unsigned int i = 0; i < COUNT(topic_devices); i++) {
        emcute_topics[i].name = topic_devices[i].name;
        topic_devices[i].topic_pointer = emcute_topics + i;
        emcute_reg(emcute_topics + i);
    }
}

void emcute_publish(topic_e topic, int value) {

    for (unsigned int i = 0; i < COUNT(topic_devices); i++) {
        if (topic_devices[i].device == topic) {
            char out[32];
            sprintf(out, "%d\n", value);
            unsigned flags = EMCUTE_QOS_0;
            emcute_pub(topic_devices[i].topic_pointer, out, strlen(out), flags);
            return;
        }
    }

}

static void *emcute_thread(void *arg) {
    (void) arg;
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, EMCUTE_ID);
    return NULL;    /* should never be reached */
}

void init_connection(void) {
    /* initialize our subscription buffers */
    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));

    /* start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    // connect to MQTT-SN broker
    printf("Connecting to MQTT-SN broker %s port %d.\n",
           SERVER_ADDR, SERVER_PORT);

    sock_udp_ep_t gw = {.family = AF_INET6, .port = SERVER_PORT};
    char *topic = MQTT_TOPIC;
    char *message = "connected";
    size_t len = strlen(message);

    /* parse address */
    if (ipv6_addr_from_str((ipv6_addr_t * ) & gw.addr.ipv6, SERVER_ADDR) == NULL) {
        printf("error parsing IPv6 address\n");
        return;
    }

    if (emcute_con(&gw, true, topic, message, len, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%i\n", SERVER_ADDR,
               (int) gw.port);
        return;
    }

    printf("Successfully connected to gateway at [%s]:%i\n",
           SERVER_ADDR, (int) gw.port);

    (void) topics;

    emcute_register_sensors_topics();
}


