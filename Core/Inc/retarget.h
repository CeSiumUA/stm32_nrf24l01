/*
 * retarget.h
 *
 *  Created on: May 9, 2024
 *      Author: mtgin
 */

#ifndef INC_RETARGET_H_
#define INC_RETARGET_H_

#include <stdio.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

void retarget_init(UART_HandleTypeDef *huart);

#endif /* INC_RETARGET_H_ */
