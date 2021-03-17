#include "dht11.h"

#define USE_DHT
#define DHT_PARAM_TYPE          (DHT11)

void dht11_init(dht11_device_t *device, gpio_t pin) {
    dht_params_t my_params;
    my_params.pin = pin;
    my_params.type = DHT_PARAM_TYPE;
    my_params.in_mode = DHT_PARAM_PULL;

    dht_init(&(device->dht), &my_params);
}

void dht11_update(dht11_device_t *device) {
    int16_t temp,hum;
    dht_read(&(device->dht), &temp, &hum);
    device->last_hum=hum;
    device->last_temp=temp;
}

double dht11_get_temp(dht11_device_t *device) {
    return ((double) device->last_temp) / 10.0f;
}

double dht11_get_hum(dht11_device_t *device) {
    return ((double) device->last_hum) / 10.0f;
}