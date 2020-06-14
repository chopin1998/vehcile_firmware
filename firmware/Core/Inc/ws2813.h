
#ifndef __WS2813_H__
#define __WS2813_H__

#include "main.h"
#include "spi.h"

#define LED_NO 42
#define LED_DATA_SIZE LED_NO * 3 // GRB array
#define LED_BBUF_SIZE LED_NO * 3 * 8 // bin array

#define CODE1 0xf0
#define CODE0 0xc0

void delay_us(uint16_t);

void ws2813_tick(void);

void ws2813_init(void);
void ws2813_set_data(uint16_t start, uint8_t len, uint8_t *data);
void ws2813_sync(int16_t);

#endif