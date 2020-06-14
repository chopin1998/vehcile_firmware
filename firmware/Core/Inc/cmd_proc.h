#ifndef _CMD_PROC_H
#define _CMD_PROC_H

#include "main.h"

void cmd_proc(uint8_t *buf, int len);

#define CMD_RESET    0x0052 //
#define CMD_BOOTLOAD 0x0053 //

#define CMD_M_IDLE   0x4D00
#define CMD_M_DUTY   0x4D10 // [duty][duty],[duty][duty],[duty][duty],[duty][duty]
#define CMD_M_SPD    0x4D20 // [duty][duty],[duty][duty],[duty][duty],[duty][duty]
#define CMD_M_PID    0x4D30 // [float][float][float]

#define CMD_QD       0x4D55 // return 4 x 32bit value

#define CMD_BUZZ     0x5542 // [freq][freq]
#define CMD_LED_SYNC 0x5552 // SYNC
#define CMD_LED_DATA 0x5562 // [start][no_of_node][g][r][b][g][r][b]... (no_of_node <= 10)

#define CMD_ADC      0x5572

#endif