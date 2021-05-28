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

#include "fmt.h"

#include "periph/rtc.h"

#include "net/loramac.h"
#include "semtech_loramac.h"
#include "sx127x.h"
#include "sx127x_netdev.h"
#include "sx127x_params.h"

#include "jsmn.h"

/* Messages are sent every 20s to respect the duty cycle on each channel */
#define PERIOD              (20U)

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
//static kernel_pid_t sender_pid;

int id_node = 0;
int isSetted = 0;

static semtech_loramac_t loramac;
static sx127x_t sx127x;

static uint8_t deveui[LORAMAC_DEVEUI_LEN];
static uint8_t appeui[LORAMAC_APPEUI_LEN];
static uint8_t appkey[LORAMAC_APPKEY_LEN];

int get_node_id(void) {
    return id_node;
}

static void _send_message(char * message)
{
    printf("Sending: %s\n", message);
    /* Try to send the message */
    uint8_t ret = semtech_loramac_send(&loramac,
                                       (uint8_t *)message, strlen(message));
    if (ret != SEMTECH_LORAMAC_TX_DONE)  {
        printf("Cannot send message '%s', ret code: %d\n", message, ret);
        return;
    }
}

void emcute_publish(char *str) {
    if (isSetted) {
        _send_message(str);
    }
}

int set_connection(int node_id) {
    id_node = node_id;
    isSetted = 1;


    fmt_hex_bytes(deveui, CONFIG_LORAMAC_DEV_EUI_DEFAULT);
    fmt_hex_bytes(appeui, CONFIG_LORAMAC_APP_EUI_DEFAULT);
    fmt_hex_bytes(appkey, CONFIG_LORAMAC_APP_KEY_DEFAULT);

    sx127x_setup(&sx127x, &sx127x_params[0], 0);
    loramac.netdev = (netdev_t * ) & sx127x;
    loramac.netdev->driver = &sx127x_driver;

    /* Initialize the loramac stack */
    semtech_loramac_init(&loramac);
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);

    // Use a fast datarate, e.g. BW125/SF7 in EU868
    semtech_loramac_set_dr(&loramac, LORAMAC_DR_5);

    // Start the Over-The-Air Activation (OTAA) procedure to retrieve the
    // generated device address and to get the network and application session
    // keys.
    puts("Starting join procedure");
    if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
        puts("Join procedure failed");
        return 1;
    }
    puts("Join procedure succeeded");

    return 1;
}