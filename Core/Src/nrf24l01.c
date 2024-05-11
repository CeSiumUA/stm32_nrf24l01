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

void nrf24l01_set_csn(struct nrf24l01_t *dev, GPIO_TypeDef *port, uint16_t pin) {
    dev->csn_port = port;
    dev->csn_pin = pin;
}

void nrf24l01_set_ce(struct nrf24l01_t *dev, GPIO_TypeDef *port, uint16_t pin) {
    dev->ce_port = port;
    dev->ce_pin = pin;
}

void nrf24l01_set_irq(struct nrf24l01_t *dev, GPIO_TypeDef *port, uint16_t pin) {
    dev->irq_port = port;
    dev->irq_pin = pin;
}

void nrf24l01_csn_on(struct nrf24l01_t *dev) {
    HAL_GPIO_WritePin(dev->csn_port, dev->csn_pin, GPIO_PIN_RESET);
}

void nrf24l01_csn_off(struct nrf24l01_t *dev) {
    HAL_GPIO_WritePin(dev->csn_port, dev->csn_pin, GPIO_PIN_SET);
}

void nrf24l01_ce_on(struct nrf24l01_t *dev) {
    HAL_GPIO_WritePin(dev->ce_port, dev->ce_pin, GPIO_PIN_SET);
}

void nrf24l01_ce_off(struct nrf24l01_t *dev) {
    HAL_GPIO_WritePin(dev->ce_port, dev->ce_pin, GPIO_PIN_RESET);
}

HAL_StatusTypeDef nrf24l01_read_register(struct nrf24l01_t *dev, uint8_t reg, uint8_t *data, uint8_t len, uint8_t *status_out) {
    HAL_StatusTypeDef res;
    uint8_t read_command = NRF24L01_CMD_R_REGISTER(reg);
    uint8_t status;
    uint8_t *status_ptr;
    status_ptr = &status;

    if(status_out != NULL) {
        status_ptr = &status_out;
    }

    nrf24l01_csn_on(dev);
    res = HAL_SPI_TransmitReceive(dev->spi, &read_command, status_ptr, 1, 100);
    if (res != HAL_OK) {
        goto read_register_exit;
    }
    res = HAL_SPI_Receive(dev->spi, data, len, 100);
read_register_exit:
    nrf24l01_csn_off(dev);
    return res;
}

HAL_StatusTypeDef nrf24l01_write_register(struct nrf24l01_t *dev, uint8_t reg, uint8_t *data, uint8_t len, uint8_t *status_out) {
    HAL_StatusTypeDef res;
    uint8_t write_command = NRF24L01_CMD_W_REGISTER(reg);
    uint8_t status;
    uint8_t *status_ptr;
    status_ptr = &status;

    if(status_out != NULL) {
        status_ptr = &status_out;
    }

    nrf24l01_csn_on(dev);
    res = HAL_SPI_TransmitReceive(dev->spi, &write_command, status_ptr, 1, 100);
    if (res != HAL_OK) {
        goto write_register_exit;
    }
    res = HAL_SPI_Transmit(dev->spi, data, len, 100);
write_register_exit:
    nrf24l01_csn_off(dev);
    return res;
}

HAL_StatusTypeDef nrf24l01_pwr_up(struct nrf24l01_t *dev) {
    uint8_t config;
    HAL_StatusTypeDef res;

    res = nrf24l01_read_register(dev, NRF24L01_REG_CONFIG, &config, 1, NULL);
    if (res != HAL_OK) {
        return res;
    }

    config |= (1 << 1);
    return nrf24l01_write_register(dev, NRF24L01_REG_CONFIG, &config, 1, NULL);
}