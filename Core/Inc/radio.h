/*
 * radio.h
 *
 *  Created on: May 11, 2024
 *      Author: mtgin
 */

#ifndef INC_RADIO_H_
#define INC_RADIO_H_

#include "nrf24l01.h"
#include "main.h"

void radio_init(SPI_HandleTypeDef *hspi, struct nrf24l01_t *radio_1, struct nrf24l01_t *radio_2);
void radio_set_rx_mode(struct nrf24l01_t *radio);
void radio_set_tx_mode(struct nrf24l01_t *radio);
void radio_send(struct nrf24l01_t *radio, uint8_t *data, uint8_t len);
void radio_receive(struct nrf24l01_t *radio, uint8_t *data, uint8_t len);
void radio_shut_down(struct nrf24l01_t *radio);
void radio_print_status(struct nrf24l01_t *radio);
void radio_print_config(struct nrf24l01_t *radio);

#endif /* INC_RADIO_H_ */
