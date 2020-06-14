#include "servo.h"
#include "usbd_cdc_if.h"

servo_t servo_m1, servo_m2, servo_m3, servo_m4;
servo_t *servos[4];

inline uint16_t servo_get_qd(uint8_t m_id)
{
    switch (m_id)
    {
    case M1:
        return __HAL_TIM_GET_COUNTER(&htim1);

        break;
    case M2:
        return __HAL_TIM_GET_COUNTER(&htim3);

        break;
    case M3:
        return __HAL_TIM_GET_COUNTER(&htim4);

        break;
    case M4:
        return __HAL_TIM_GET_COUNTER(&htim8);

        break;
    default:
        break;
    }
}

static inline uint16_t _get_qd_dir(uint8_t m_id)
{
    switch (m_id)
    {
    case M1:
        return htim1.Instance->CR1 & TIM_CR1_DIR;

        break;
    case M2:
        return htim3.Instance->CR1 & TIM_CR1_DIR;

        break;
    case M3:
        return htim4.Instance->CR1 & TIM_CR1_DIR;

        break;
    case M4:
        return htim8.Instance->CR1 & TIM_CR1_DIR;

        break;
    default:
        break;
    }
}

int16_t servo_get_spd(uint8_t m_id)
{
    return servos[m_id]->curr_spd;
}

void servo_spd_update(uint8_t m_id)
{
    uint16_t curr_qd;
    curr_qd = servo_get_qd(m_id);

    servo_t *s = servos[m_id];

    if ((curr_qd - s->last_qd) == 0)
    {
        s->spd_zero_count++;
        if (s->spd_zero_count > 12)
        {
            s->spd_zero_count = 0;
            s->curr_spd = 0;
        }
        else
        {
            s->curr_spd >>= 1;
        }
    }
    else
    {
        s->spd_zero_count = 0;

        s->curr_spd = curr_qd - s->last_qd;
        s->last_qd = curr_qd;
    }
}

void servo_set_duty(uint8_t m_id, int16_t duty)
{
    switch (m_id)
    {
    case M1:
        if (duty >= 0)
        {
            HAL_GPIO_WritePin(M1_DIR_GPIO_Port, M1_DIR_Pin, 0);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, duty);
        }
        else
        {
            HAL_GPIO_WritePin(M1_DIR_GPIO_Port, M1_DIR_Pin, 1);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, -duty);
        }

        break;
    case M2:
        if (duty >= 0)
        {
            HAL_GPIO_WritePin(M2_DIR_GPIO_Port, M2_DIR_Pin, 0);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, duty);
        }
        else
        {
            HAL_GPIO_WritePin(M2_DIR_GPIO_Port, M2_DIR_Pin, 1);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, -duty);
        }

        break;
    case M3:
        if (duty >= 0)
        {
            HAL_GPIO_WritePin(M3_DIR_GPIO_Port, M3_DIR_Pin, 0);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, duty);
        }
        else
        {
            HAL_GPIO_WritePin(M3_DIR_GPIO_Port, M3_DIR_Pin, 1);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, -duty);
        }

        break;
    case M4:
        if (duty >= 0)
        {
            HAL_GPIO_WritePin(M4_DIR_GPIO_Port, M4_DIR_Pin, 0);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, duty);
        }
        else
        {
            HAL_GPIO_WritePin(M4_DIR_GPIO_Port, M4_DIR_Pin, 1);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, -duty);
        }

        break;

    default:
        break;
    }
}

void servo_init(void)
{
    servos[M1] = &servo_m1;
    servos[M2] = &servo_m2;
    servos[M3] = &servo_m3;
    servos[M4] = &servo_m4;

    for (uint8_t i = 0; i < 4; i++)
    {
        servos[i]->mode = MODE_IDLE;
        servos[i]->pid_ins.Kd = 15;
        servos[i]->pid_ins.Ki = 5;
        servos[i]->pid_ins.Kd = 2;
        servos[i]->last_qd = 0;

        arm_pid_init_f32(&(servos[i]->pid_ins), 1);
    }
}

/*
 * the function called by systick @ 1Khz
 * 
 * do real func ~100Hz(10ms)
 */
void servo_tick(void)
{
    static uint8_t _servo_tick_delay = 0;

    _servo_tick_delay++;
    if (_servo_tick_delay <= 9)
    {
        return;
    }
    _servo_tick_delay = 0;
    // HAL_GPIO_TogglePin(LED_DIAG_GPIO_Port, LED_DIAG_Pin);

    for (uint8_t m_id=0; m_id<4; m_id++)
    {
        servo_spd_update(m_id);
        if (servos[m_id]->mode == MODE_PID)
        {
            servo_pid_update(m_id);
        }
    }
}

void servo_pid_update(uint8_t m_id)
{
    // uint8_t buf[64];
    // uint16_t l;

    servo_t *s = servos[m_id];
    int16_t pid_error, duty;
    
    pid_error = s->cmd_spd - s->curr_spd;
    duty = arm_pid_f32(&(s->pid_ins), pid_error);

    // if (m_id == M1)
    // {
    //     l = snprintf(buf, 64, "err: %d, duty: %d\n", pid_error, duty);
    //     CDC_Transmit_FS(buf, l);
    // }

#define PWM_LIMIT 768
    if (duty < -PWM_LIMIT)
    {
        if (s->pid_reset_count > 32)
        {
            s->pid_reset_count = 0;
            arm_pid_reset_f32(&s->pid_ins);
            duty = -PWM_LIMIT;
        }
        else
        {
            s->pid_reset_count++;
        }
    }
    else if (duty > PWM_LIMIT)
    {
        if (s->pid_reset_count > 32)
        {
            arm_pid_reset_f32(&s->pid_ins);
            duty = PWM_LIMIT;
        }
        else
        {
            s->pid_reset_count++;
        }
    }
    else
    {
        s->pid_reset_count >>= 1;
    }

    servo_set_duty(m_id, duty);
}

void servo_cmd(int16_t cmd1, int16_t cmd2, int16_t cmd3, int16_t cmd4)
{
    int16_t cmds[] = {cmd1, cmd2, cmd3, cmd4};

    for (uint32_t i=0; i<4; i++)
    {
        if (cmds[i])
        {
            servos[i]->cmd_spd = cmds[i];
            servos[i]->mode = MODE_PID;
        }
        else
        {
            servos[i]->mode = MODE_IDLE;
            servo_set_duty(i, 0);
        }
    }   
}

void servo_set_pid(float kp, float ki, float kd)
{
    servo_t *s;
    for (uint32_t m_id=0; m_id<4; m_id++)
    {
        s = servos[m_id];
        s->pid_ins.Kp = kp;
        s->pid_ins.Ki = ki;
        s->pid_ins.Kd = kd;

        arm_pid_init_f32(&s->pid_ins, 1);
    }
}
