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
    nrf24l01_init(&radio_1, &hspi2);
    nrf24l01_set_csn(&radio_1, GPIOB, GPIO_PIN_12);
    nrf24l01_set_ce(&radio_1, GPIOB, GPIO_PIN_13);
    nrf24l01_set_irq(&radio_1, GPIOB, GPIO_PIN_14);

    nrf24l01_init(&radio_2, &hspi2);
    nrf24l01_set_csn(&radio_2, GPIOB, GPIO_PIN_15);
    nrf24l01_set_ce(&radio_2, GPIOB, GPIO_PIN_0);
    nrf24l01_set_irq(&radio_2, GPIOB, GPIO_PIN_1);
}