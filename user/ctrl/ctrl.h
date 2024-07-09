#ifndef __CTRL_H__
#define __CTRL_H__

#include "main.h"
#include "string.h"
#include "stdio.h"
#include "tim.h"
#include "dht11.h"
#include "task.h"
#include "dht11.h"
#include "pid.h"
#include "stm32f4xx_hal_gpio.h"
#include "menu.h"
#include "usart.h"
extern uint8_t rxBuffer[4];
#define RX_CMD_LEN 4
typedef struct GPIO_Scan
{
	uint8_t	pa0;
	uint8_t pa1;
	uint8_t pa2;
	uint8_t	pa3;
	uint8_t pa4;
	uint8_t pa5;
	uint8_t	pa6;
	uint8_t pb5;
	uint8_t pb6;
}Scan;
#endif
