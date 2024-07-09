#ifndef __DHT11_H__
#define __DHT11_H__
#include "main.h"
#include "tim.h"
void DHT11_GetValue(void);
void delay(uint32_t time);
extern float Temperature;
extern float Humidity;
#endif
