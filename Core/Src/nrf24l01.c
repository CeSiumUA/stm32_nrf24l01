/*
 * nrf24l01.c
 *
 *  Created on: May 9, 2024
 *      Author: mtgin
 */


#include "nrf24l01.h"

void nrf24l01_init(struct nrf24l01_t *dev, SPI_HandleTypeDef *spi) {
    dev->spi = spi;
}

HAL_StatusTypeDef nrf24l01_read_register(struct nrf24l01_t *dev, uint8_t reg, uint8_t *data, uint8_t len) {
    HAL_StatusTypeDef res;
    uint8_t read_command = NRF24L01_CMD_R_REGISTER(reg);
    uint8_t status;
    HAL_GPIO_WritePin(dev->csn_port, dev->csn_pin, GPIO_PIN_RESET);
    res = HAL_SPI_TransmitReceive(dev->spi, &read_command, &status, 1, 100);
    if (res != HAL_OK) {
        HAL_GPIO_WritePin(dev->csn_port, dev->csn_pin, GPIO_PIN_SET);
        return res;
    }
    res = HAL_SPI_Receive(dev->spi, data, len, 100);
    HAL_GPIO_WritePin(dev->csn_port, dev->csn_pin, GPIO_PIN_SET);
    return res;
}