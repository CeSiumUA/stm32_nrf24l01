/*
 * retarget.c
 *
 *  Created on: May 9, 2024
 *      Author: mtgin
 */


#include "retarget.h"

static UART_HandleTypeDef *print_uart;

void retarget_init(UART_HandleTypeDef *huart)
{
    print_uart = huart;
}

int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(print_uart, (uint8_t *)ptr, len, 0xFFFF);
    return len;
}