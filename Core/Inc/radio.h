/*
 * radio.h
 *
 *  Created on: Jun 28, 2024
 *      Author: mtgin
 */

#ifndef INC_RADIO_H_
#define INC_RADIO_H_

#include "nrf24.h"
#include "retarget.h"

void radio_raise_irq_flag(void);
void radio_process_irq(struct nrf24_t *radio);
void radio_shut_down(struct nrf24_t *radio);

#endif /* INC_RADIO_H_ */
