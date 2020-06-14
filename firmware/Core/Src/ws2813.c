#include "ws2813.h"

uint8_t LED_BBUF[LED_BBUF_SIZE]; // raw bin buff
uint8_t LED_DATA[LED_DATA_SIZE]; // grb data array

void delay_us(uint16_t n)
{
	uint32_t time = n * (SystemCoreClock / 1000000);
	while (time--)
	{
	}
}

static uint32_t grb2bin(uint8_t *array, uint32_t no)
{
	uint16_t bytes = 0;
	uint8_t curr_color;

	for (uint8_t i = 0; i < no * 3; i++)
	{
		curr_color = array[i];
		for (uint8_t b = 0; b < 8; b++)
		{
			LED_BBUF[bytes] = ((curr_color << b) & 0x80) ? CODE1 : CODE0;
			bytes++;
		}
	}

	return bytes;
}

static void data2buf(void)
{
	uint16_t bytes = 0;
	uint8_t curr_color;

	for (uint16_t i = 0; i < LED_DATA_SIZE; i++)
	{
		curr_color = LED_DATA[i];
		for (uint8_t b = 0; b < 8; b++)
		{
			LED_BBUF[bytes] = ((curr_color << b) & 0x80) ? CODE1 : CODE0;
			bytes++;
		}
	}
}

void ws2813_init(void)
{
	data2buf();
	HAL_SPI_Transmit_DMA(&hspi2, LED_BBUF, LED_BBUF_SIZE);
	// delay_us(50);
}

void ws2813_sync(int16_t flag)
{
	if (flag < 0)
	{
		HAL_SPI_Transmit_DMA(&hspi2, LED_BBUF, LED_BBUF_SIZE);
	}
	else
	{
		if (flag > LED_NO)
		{
			flag = LED_NO;
		}
		flag = LED_NO * 3 * 8;
		HAL_SPI_Transmit_DMA(&hspi2, LED_BBUF, flag);
	}
	
}

void ws2813_set_data(uint16_t start, uint8_t len, uint8_t *a)
{
	if ((start + len) > LED_NO)
	{
		len = (LED_NO - start) * 3;
	}
	else
	{
		len *= 3;
	}
	

	uint8_t *led_data_head = &LED_DATA[start*3];
	for (uint16_t i=0; i<len; i++)
	{
		*led_data_head = a[i];
		led_data_head++;
	}

	data2buf();
}

void ws2813_tick(void)
{
	return;
}