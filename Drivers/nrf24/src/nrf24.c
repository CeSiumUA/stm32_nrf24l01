/*
 * nrf24.c
 *
 *  Created on: Jun 28, 2024
 *      Author: mtgin
 */

#include "nrf24.h"

static void nrf24_csn_on(struct nrf24_t *nrf24) {
    NRF24_HAL_SET_PIN_LOW
}

static void nrf24_csn_off(struct nrf24_t *nrf24) {
    NRF24_HAL_SET_PIN_HIGH(nrf24->csn);
}

void nrf24_ce_on(struct nrf24_t *nrf24) {
    NRF24_HAL_SET_PIN_HIGH(nrf24->ce);
}

void nrf24_ce_off(struct nrf24_t *nrf24) {
    NRF24_HAL_SET_PIN_LOW(nrf24->ce);
}