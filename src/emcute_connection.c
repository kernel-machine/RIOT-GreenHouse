//
// Created by Luca Giovannesi.
//

#include "emcute_connection.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "../devices/devices_manager.h"
#include "../devices/servo_device.h"
#include "../devices/dht11.h"
#include "../devices/digital_out.h"

#include "jsmn.h"

#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "msg.h"



#define TOPIC_MAXLEN        256
#define MAX_JSON_TOKEN      128

char em_stack[THREAD_STACKSIZE_MAIN];

static emcute_sub_t subscription;
static char topics[TOPIC_MAXLEN];
int can_access = 1;

int id_node = 0;
int isSetted = 0;
char nodeName[64];

emcute_topic_t emcute_topic;

jsmn_parser p;
jsmntok_t t[MAX_JSON_TOKEN];


static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}

void emcute_publish(char *str) {
    if (isSetted) {
        unsigned flags = EMCUTE_QOS_0;
        int res = emcute_pub(&emcute_topic, str, strlen(str), flags);
        if (res != EMCUTE_OK) {
            printf("Error while publishing %d\n",res);
        }
    }
}


int get_node_id(void) {
    return id_node;
}

static void *emcute_thread(void *arg) {
    (void) arg;
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, nodeName);
    return NULL;   // should never be reached
}

void on_pub_my(const emcute_topic_t *topic, void *data, size_t len) {
    if (!can_access) {
        printf("ACCESS DENIED!\n");
        return;
    }

    char *topic_name = MQTT_CMD_TOPIC;
    if (strcmp(topic->name, topic_name) != 0)
        return;

    can_access = 0;

    char *in = (char *) data;
    if (len == 0 || strlen(in) < 10) {
        can_access = 1;
        return;
    }

    printf("### got publication for topic '%s' [%i] ###\n",
           topic->name, (int) topic->id);

    for (size_t i = 0; i < len; i++) {
        printf("%c", in[i]);
    }
    puts("\n");


    memset(&p, 0, sizeof(jsmn_parser));
    memset(&t, 0, sizeof(jsmntok_t) * MAX_JSON_TOKEN);

    jsmn_init(&p);
    int r = jsmn_parse(&p, in, len, t, MAX_JSON_TOKEN);

    if (r < 0) {
        printf("Failed to parse JSON: %d\n", r);
        can_access = 1;
        return;

    }

    if (r < 1 || t[0].type != JSMN_OBJECT) {
        printf("Object expected\n");
        can_access = 1;
        return;
    }

    /* Loop over all keys of the root object */
    char name[10];
    char value[10];
    for (int i = 1; i < r; i++) {
        if (jsoneq(in, &t[i], "name") == 0) {
            /* We may use strndup() to fetch string value */
            sprintf(name, "%.*s", t[i + 1].end - t[i + 1].start,
                    in + t[i + 1].start);
            //printf("Name: %s\n", name);
            i++;
        }
        else if (jsoneq(in, &t[i], "value") == 0) {
            /* We may additionally check if the value is either "true" or "false" */
            sprintf(value, "%.*s", t[i + 1].end - t[i + 1].start,
                    in + t[i + 1].start);
            //printf("value: %s\n", value);
            i++;
        }
        else {
            printf("Unexpected key: %.*s\n", t[i].end - t[i].start,
                   in + t[i].start);
        }
    }

    if (strlen(name) > 0 && strlen(value) > 0) {
        int int_value = atoi(value);

        if (strcmp(name, "pump") == 0) {
            digital_out_t *pump = device_manager_get_device(PUMP);
            if (int_value)
                digital_out_enable(pump);
            else
                digital_out_disable(pump);

        }
        else if (strcmp(name, "servo") == 0) {

            servo_device_t *device = device_manager_get_device(SERVO);
            servo_device_set_manual_override(device, int_value);
            if (int_value == -1) {
                //servo_device_set_position(device, degree);
                servo_device_clear_manual_override(device);
            }
        }
    }

    can_access = 1;
}

int set_connection(char *server_addr, int node_id) {
    id_node = node_id;

    memset(nodeName,0,64);
    sprintf(nodeName, "node%d", node_id);

    memset(&subscription, 0, sizeof(emcute_sub_t));

    thread_create(em_stack, sizeof(em_stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  emcute_thread,
                  NULL, "emcute thread");


    sock_udp_ep_t gw = {.family = AF_INET6, .port = SERVER_PORT};

    char *topic = NULL;
    char *message = NULL;
    size_t len = 0;

    //parse address
    if (ipv6_addr_from_str((ipv6_addr_t * ) & gw.addr.ipv6, server_addr) == NULL) {
        printf("error parsing IPv6 address\n");
        return 0;
    }
    else
        printf("Parsed Pv6 address\n");

    if (emcute_con(&gw, true, topic, message, len, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%i\n", server_addr,
               (int) gw.port);
        return 0;
    }

    printf("Successfully connected to gateway at [%s]:%i\n",
           server_addr, (int) gw.port);

    //setup subscription to topic
    unsigned flags = EMCUTE_QOS_0;
    subscription.cb = on_pub_my;
    memset(topics, 0, TOPIC_MAXLEN);
    subscription.topic.name = MQTT_CMD_TOPIC;

    if (emcute_sub(&subscription, flags) != EMCUTE_OK) {
        printf("error: unable to subscribe to %s\n", MQTT_CMD_TOPIC);
        return 0;
    }
    else
        printf("Now subscribed to %s\n", MQTT_CMD_TOPIC);

    //Register topic to send
    emcute_topic.name = MQTT_TOPIC;
    emcute_reg(&emcute_topic);
    isSetted = 1;

    return 1;
}