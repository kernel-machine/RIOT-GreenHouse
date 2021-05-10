//
// Created by Luca Giovannesi.
//

#ifndef RIOT_GREENHOUSE_UDP_H
#define RIOT_GREENHOUSE_UDP_H
//Server-side
void start_server(char *port_str);
void stop_server(void);

//Client-side
void setServerAddress(char * server_addr);
void send( char *data, unsigned int num, unsigned int delay);

int is_server(void);
int is_client(void);
#endif //RIOT_GREENHOUSE_UDP_H
