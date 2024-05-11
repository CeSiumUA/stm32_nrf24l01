/*
 * radio.c
 *
 *  Created on: May 11, 2024
 *      Author: mtgin
 */


#include "radio.h"

struct nrf24l01_t radio_1;
struct nrf24l01_t radio_2;

void radio_init(SPI_HandleTypeDef *hspi) {
    nrf24l01_init(&radio_1, hspi);
    nrf24l01_set_csn(&radio_1, RADIO_1_CSN_GPIO_Port, RADIO_1_CSN_Pin);
    nrf24l01_set_ce(&radio_1, RADIO_1_CE_GPIO_Port, RADIO_1_CE_Pin);
    nrf24l01_set_irq(&radio_1, RADIO_1_IRQ_GPIO_Port, RADIO_1_IRQ_Pin);

    nrf24l01_init(&radio_2, hspi);
    nrf24l01_set_csn(&radio_2, RADIO_2_CSN_GPIO_Port, RADIO_2_CSN_Pin);
    nrf24l01_set_ce(&radio_2, RADIO_2_CE_GPIO_Port, RADIO_2_CE_Pin);
    nrf24l01_set_irq(&radio_2, RADIO_2_IRQ_GPIO_Port, RADIO_2_IRQ_Pin);
}