/*
 * dht11.h
 *
 *  Created on: Dec 1, 2021
 *      Author: John
 */

#ifndef PUBLIC_DHT11_H_
#define PUBLIC_DHT11_H_

#include "stm32f407xx.h"

// Public APIs

void dht11_get_data( GPIO_Handle_t *dht_signal_pin);




#endif /* PUBLIC_DHT11_H_ */
