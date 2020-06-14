#ifndef _SERVO_H
#define _SERVO_H

#define ARM_MATH_CM4
#include "main.h"
#include "tim.h"
#include "arm_math.h"

enum {M1, M2, M3, M4};
enum {DIR_P, DIR_M};

enum {MODE_IDLE, MODE_RAW, MODE_PID};

typedef struct {
    volatile uint16_t mode;

    int16_t cmd_spd;
    int16_t curr_spd;

    uint16_t spd_zero_count;
    uint16_t pid_reset_count;
    volatile uint16_t last_qd;

    arm_pid_instance_f32 pid_ins;
} servo_t;


int16_t servo_get_spd(uint8_t m_id);
uint16_t servo_get_qd(uint8_t m_id);

void servo_init(void);
void servo_tick(void);
void servo_set_duty(uint8_t m_id, int16_t duty);
//
void servo_cmd(int16_t, int16_t, int16_t, int16_t);
void servo_set_pid(float, float, float);

#endif