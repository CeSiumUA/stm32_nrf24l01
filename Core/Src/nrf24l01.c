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

static void nrf24l01_csn_on(struct nrf24l01_t *dev) {
    HAL_GPIO_WritePin(dev->csn_port, dev->csn_pin, GPIO_PIN_RESET);
}

static void nrf24l01_csn_off(struct nrf24l01_t *dev) {
    HAL_GPIO_WritePin(dev->csn_port, dev->csn_pin, GPIO_PIN_SET);
}

void nrf24l01_ce_on(struct nrf24l01_t *dev) {
    HAL_GPIO_WritePin(dev->ce_port, dev->ce_pin, GPIO_PIN_SET);
}

void nrf24l01_ce_off(struct nrf24l01_t *dev) {
    HAL_GPIO_WritePin(dev->ce_port, dev->ce_pin, GPIO_PIN_RESET);
}

static HAL_StatusTypeDef nrf24l01_read_register(struct nrf24l01_t *dev, uint8_t reg, uint8_t *data, uint8_t len, uint8_t *status_out) {
    HAL_StatusTypeDef res;
    uint8_t read_command = NRF24L01_CMD_R_REGISTER(reg);
    uint8_t status;
    uint8_t *status_ptr;
    status_ptr = &status;

    if(status_out != NULL) {
        status_ptr = status_out;
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

static HAL_StatusTypeDef nrf24l01_write_register(struct nrf24l01_t *dev, uint8_t reg, uint8_t *data, uint8_t len, uint8_t *status_out) {
    HAL_StatusTypeDef res;
    uint8_t write_command = NRF24L01_CMD_W_REGISTER(reg);
    uint8_t status;
    uint8_t *status_ptr;
    status_ptr = &status;

    if(status_out != NULL) {
        status_ptr = status_out;
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

HAL_StatusTypeDef nrf24l01_pwr_down(struct nrf24l01_t *dev) {
    uint8_t config;
    HAL_StatusTypeDef res;

    res = nrf24l01_read_register(dev, NRF24L01_REG_CONFIG, &config, 1, NULL);
    if (res != HAL_OK) {
        return res;
    }

    config &= ~(1 << 1);
    return nrf24l01_write_register(dev, NRF24L01_REG_CONFIG, &config, 1, NULL);
}

HAL_StatusTypeDef nrf24l01_set_ptx_mode(struct nrf24l01_t *dev) {
    uint8_t config;
    HAL_StatusTypeDef res;

    res = nrf24l01_read_register(dev, NRF24L01_REG_CONFIG, &config, 1, NULL);
    if (res != HAL_OK) {
        return res;
    }

    config &= ~(1 << 0);
    return nrf24l01_write_register(dev, NRF24L01_REG_CONFIG, &config, 1, NULL);
}

HAL_StatusTypeDef nrf24l01_set_prx_mode(struct nrf24l01_t *dev) {
    uint8_t config;
    HAL_StatusTypeDef res;

    res = nrf24l01_read_register(dev, NRF24L01_REG_CONFIG, &config, 1, NULL);
    if (res != HAL_OK) {
        return res;
    }

    config |= (1 << 0);
    return nrf24l01_write_register(dev, NRF24L01_REG_CONFIG, &config, 1, NULL);
}

HAL_StatusTypeDef nrf24l01_set_address_width(struct nrf24l01_t *dev, enum nrf24l01_address_width_t width) {
    return nrf24l01_write_register(dev, NRF24L01_REG_SETUP_AW, &width, 1, NULL);
}

HAL_StatusTypeDef nrf24l01_set_auto_retransmit_delay(struct nrf24l01_t *dev, enum nrf24l01_auto_retransmit_delay_t delay) {
    uint8_t setup_retr;
    HAL_StatusTypeDef res;

    res = nrf24l01_read_register(dev, NRF24L01_REG_SETUP_RETR, &setup_retr, 1, NULL);
    if (res != HAL_OK) {
        return res;
    }

    setup_retr &= 0xF;
    setup_retr |= (delay << 4);

    return nrf24l01_write_register(dev, NRF24L01_REG_SETUP_RETR, &setup_retr, 1, NULL);
}

HAL_StatusTypeDef nrf24l01_set_auto_retransmit_count(struct nrf24l01_t *dev, enum nrf24l01_auto_retransmit_count_t count) {
    uint8_t setup_retr;
    HAL_StatusTypeDef res;

    res = nrf24l01_read_register(dev, NRF24L01_REG_SETUP_RETR, &setup_retr, 1, NULL);
    if (res != HAL_OK) {
        return res;
    }

    setup_retr &= 0xF0;
    setup_retr |= count;

    return nrf24l01_write_register(dev, NRF24L01_REG_SETUP_RETR, &setup_retr, 1, NULL);
}

HAL_StatusTypeDef nrf24l01_set_rf_channel(struct nrf24l01_t *dev, int channel) {
    uint8_t ch;
    ch = (uint8_t)(channel - NRF24L01_BASE_FREQUENCY);
    if (ch > 125) {
        return HAL_ERROR;
    }
    return nrf24l01_write_register(dev, NRF24L01_REG_RF_CH, &ch, 1, NULL);
}

HAL_StatusTypeDef nrf24l01_set_air_data_rate(struct nrf24l01_t *dev, enum nrf24l01_air_data_rate_t rate) {
    uint8_t rf_setup;
    HAL_StatusTypeDef res;

    res = nrf24l01_read_register(dev, NRF24L01_REG_RF_SETUP, &rf_setup, 1, NULL);
    if (res != HAL_OK) {
        return res;
    }

    rf_setup &= ~(1 << 3);
    rf_setup |= rate;
    return nrf24l01_write_register(dev, NRF24L01_REG_RF_SETUP, &rf_setup, 1, NULL);
}

HAL_StatusTypeDef nrf24l01_get_status(struct nrf24l01_t *dev, uint8_t *status) {
    return nrf24l01_read_register(dev, NRF24L01_REG_STATUS, status, 1, NULL);
}

HAL_StatusTypeDef nrf24l01_get_status_nop(struct nrf24l01_t *dev, uint8_t *status) {
    uint8_t nop = NRF24L01_CMD_NOP;
    HAL_StatusTypeDef res;

    nrf24l01_csn_on(dev);
    res = HAL_SPI_TransmitReceive(dev->spi, &nop, status, 1, 100);
    nrf24l01_csn_off(dev);
    return res;
}

HAL_StatusTypeDef nrf24l01_set_tx_address(struct nrf24l01_t *dev, uint8_t *address, uint8_t len) {
    return nrf24l01_write_register(dev, NRF24L01_REG_TX_ADDR, address, len, NULL);
}

HAL_StatusTypeDef nrf24l01_set_rx_pipe_data_width(struct nrf24l01_t *dev, uint8_t pipe, uint8_t width) {
    if (pipe > 5 || width > 32) {
        return HAL_ERROR;
    }
    return nrf24l01_write_register(dev, NRF24L01_REG_RX_PW_P0 + pipe, &width, 1, NULL);
}