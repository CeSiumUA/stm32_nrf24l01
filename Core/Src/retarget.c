/*
 * retarget.c
 *
 *  Created on: May 9, 2024
 *      Author: mtgin
 */


#include "retarget.h"

static UART_HandleTypeDef *print_uart;
bool locked = false;

void retarget_init(UART_HandleTypeDef *huart)
{
    while(locked);
    locked = true;
    print_uart = huart;
    locked = false;
}

int _write(int file, char *ptr, int len)
{
    while(locked);
    locked = true;
    HAL_UART_Transmit(print_uart, (uint8_t *)ptr, len, 0xFFFF);
    locked = false;
    return len;
}