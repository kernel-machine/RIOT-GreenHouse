#ifndef RIOT_DHT11_H
#define RIOT_DHT11_H

#include "../RIOT/drivers/include/dht.h"
#include "../RIOT/drivers/dht/include/dht_params.h"
#include "../RIOT/drivers/include/periph/gpio.h"

typedef struct dht11_device_s{
    dht_t dht;
    int16_t last_temp;
    int16_t last_hum;
}dht11_device_t;

void dht11_init(dht11_device_t * device, gpio_t pin);
void dht11_update(dht11_device_t * device);
float dht11_get_temp(dht11_device_t * device);
float dht11_get_hum (dht11_device_t * device);

#endif //RIOT_DHT11_H
