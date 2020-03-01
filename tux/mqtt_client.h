//
// Created by alter on 31.01.2020.
//

#ifndef DEMO_TUX_MQTT_CLIENT_H_
#define DEMO_TUX_MQTT_CLIENT_H_

#include "ip_config.h"
#include <stdint.h>

#ifdef MQTT_client

extern void mqtt_connect(
	void (*callback)(unsigned int, unsigned int, unsigned int),
	uint8_t *dstip,
	uint8_t *dstmac);

//
// returns 1 if we have an answer from an DNS server and an IP
extern uint8_t mqtt_connected(void);

#endif

#endif //DEMO_TUX_IP_ARP_UDP_TCP_C_MQTT_CLIENT_H_
