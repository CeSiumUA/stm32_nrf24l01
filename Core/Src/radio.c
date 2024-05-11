/*
 * radio.c
 *
 *  Created on: May 11, 2024
 *      Author: mtgin
 */


#include "radio.h"


void radio_init(SPI_HandleTypeDef *hspi, struct nrf24l01_t *radio_1, struct nrf24l01_t *radio_2) {
    nrf24l01_init(radio_1, hspi);
    nrf24l01_set_csn(radio_1, RADIO_1_CSN_GPIO_Port, RADIO_1_CSN_Pin);
    nrf24l01_set_ce(radio_1, RADIO_1_CE_GPIO_Port, RADIO_1_CE_Pin);
    nrf24l01_set_irq(radio_1, RADIO_1_IRQ_GPIO_Port, RADIO_1_IRQ_Pin);

    nrf24l01_init(radio_2, hspi);
    nrf24l01_set_csn(radio_2, RADIO_2_CSN_GPIO_Port, RADIO_2_CSN_Pin);
    nrf24l01_set_ce(radio_2, RADIO_2_CE_GPIO_Port, RADIO_2_CE_Pin);
    nrf24l01_set_irq(radio_2, RADIO_2_IRQ_GPIO_Port, RADIO_2_IRQ_Pin);
}

void radio_set_rx_mode(struct nrf24l01_t *radio) {
    nrf24l01_set_prx_mode(radio);
    HAL_Delay(15);

    nrf24l01_pwr_up(radio);
    HAL_Delay(2);

    nrf24l01_set_address_width(radio, NRF24L01_AW_5_BYTES);
    nrf24l01_set_rf_channel(radio, 2500);
    nrf24l01_set_air_data_rate(radio, NRF24L01_ADR_2_MBPS);
    nrf24l01_set_rx_pipe_data_width(radio, 0, 32);

    nrf24l01_ce_on(radio);
}

void radio_set_tx_mode(struct nrf24l01_t *radio) {
    nrf24l01_set_ptx_mode(radio);
    HAL_Delay(15);

    nrf24l01_pwr_up(radio);
    HAL_Delay(2);

    nrf24l01_set_address_width(radio, NRF24L01_AW_5_BYTES);
    nrf24l01_set_rf_channel(radio, 2500);
    nrf24l01_set_air_data_rate(radio, NRF24L01_ADR_2_MBPS);

    nrf24l01_ce_on(radio);
}

void radio_shut_down(struct nrf24l01_t *radio) {
    nrf24l01_ce_off(radio);
    nrf24l01_pwr_down(radio);
}