#include "cmd_proc.h"
#include "usbd_cdc_if.h"
#include "adc.h"
#include "tim.h"
#include "ws2813.h"
#include "servo.h"

void cmd_bootload(void)
{
    void (*SysMemBootJump)(void);

    volatile uint32_t addr = 0x1FFFD800;

    HAL_RCC_DeInit();
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;

    __disable_irq();

    __HAL_SYSCFG_REMAPMEMORY_FLASH();

    SysMemBootJump = (void (*)(void))(*((uint32_t *)(addr + 4)));
    __set_MSP(*(uint32_t *)addr);
    SysMemBootJump();
}

void cmd_reset(void)
{
    HAL_NVIC_SystemReset();
}

void cmd_proc(uint8_t *buf, int len)
{
    uint16_t cmd = *(uint16_t *)buf;

    int16_t *data;
    float *data_f;
    int32_t qd;
    
    int8_t tmp[64];
    int32_t l = 0;
    float32_t test = PI;
    
    data = (int16_t *)buf + 1;
    data_f = (float *)buf + 1;
    switch (cmd)
    {
        case CMD_BOOTLOAD:
            cmd_bootload();
        
        break;
        case CMD_RESET:
            cmd_reset();
        
        break;
        case CMD_M_SPD:
            servo_cmd(data[0], data[1], data[2], data[3]);
            l = snprintf(tmp, 64, "%d,%d,%d,%d\n", data[0], data[1], data[2], data[3]);
            CDC_Transmit_FS(tmp, l);

        break;
        case CMD_QD:
            l = snprintf(tmp, 64, "(%d)%d,(%d)%d,(%d)%d,(%d)%d",
                         servo_get_spd(M1),
                         servo_get_qd(M1),
                         servo_get_spd(M2),
                         servo_get_qd(M2),
                         servo_get_spd(M3),
                         servo_get_qd(M3),
                         servo_get_spd(M4),
                         servo_get_qd(M4));
            CDC_Transmit_FS(tmp, l);

        break;
        case CMD_M_DUTY:

            servo_set_duty(M1, data[0]);
            servo_set_duty(M2, data[1]);
            servo_set_duty(M3, data[2]);
            servo_set_duty(M4, data[3]);

            l = snprintf(tmp, 32, "%hd,%hd,%hd,%hd", data[0], data[1], data[2], data[3]);
            CDC_Transmit_FS(tmp, l);
        
        break;
        case CMD_BUZZ:
            htim15.Instance->ARR = data[0];
            htim15.Instance->CCR2 = data[0] >> 2;

        break;
        case CMD_LED_SYNC:
            ws2813_sync(-1);

        break;
        case CMD_LED_DATA:
            ws2813_set_data(buf[2], buf[3], &(buf[4]));
            ws2813_sync(-1);

            l = snprintf(tmp, 32, "%d,%d\n", buf[2], &buf[3]);
            CDC_Transmit_FS(tmp, l);
        break;
        case CMD_M_PID:
            l = snprintf(tmp, 32, "%d,%d,%d\n", (int)data_f[0], (int)data_f[1], (int)data_f[2]);
            CDC_Transmit_FS(tmp, l);
            servo_set_pid(data_f[0], data_f[1], data_f[2]);

        break;
        case CMD_ADC:
            HAL_ADC_Start(&hadc2);
            HAL_ADC_Start(&hadc4);
            HAL_ADC_PollForConversion(&hadc2, 100);
            HAL_ADC_PollForConversion(&hadc4, 100);
            l = snprintf(tmp, 32, "%d, %d\n", HAL_ADC_GetValue(&hadc2), HAL_ADC_GetValue(&hadc4));
            CDC_Transmit_FS(tmp, l);

        break;
        default:
            CDC_Transmit_FS(buf, len);
    }
}