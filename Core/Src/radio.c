/*
 * radio.c
 *
 *  Created on: May 11, 2024
 *      Author: mtgin
 */


#include "radio.h"


void radio_init(SPI_HandleTypeDef *hspi, struct nrf24l01_t *radio_1, struct nrf24l01_t *radio_2) {
    if(radio_1 == NULL) {
        return;
    }
    nrf24l01_init(radio_1, hspi);
    nrf24l01_set_csn(radio_1, RADIO_1_CSN_GPIO_Port, RADIO_1_CSN_Pin);
    nrf24l01_set_ce(radio_1, RADIO_1_CE_GPIO_Port, RADIO_1_CE_Pin);
    nrf24l01_set_irq(radio_1, RADIO_1_IRQ_GPIO_Port, RADIO_1_IRQ_Pin);

    if(radio_2 == NULL) {
        return;
    }
    nrf24l01_init(radio_2, hspi);
    nrf24l01_set_csn(radio_2, RADIO_2_CSN_GPIO_Port, RADIO_2_CSN_Pin);
    nrf24l01_set_ce(radio_2, RADIO_2_CE_GPIO_Port, RADIO_2_CE_Pin);
    nrf24l01_set_irq(radio_2, RADIO_2_IRQ_GPIO_Port, RADIO_2_IRQ_Pin);
}

void radio_set_rx_mode(struct nrf24l01_t *radio) {
    HAL_StatusTypeDef res;

    res = nrf24l01_reset(radio);
    if(res != HAL_OK) {
        printf("Failed to reset\n");
    }

    res = nrf24l01_flush_rx_fifo(radio);
    if(res != HAL_OK) {
        printf("Failed to flush RX FIFO\n");
    }

    res = nrf24l01_set_prx_mode(radio);
    if(res != HAL_OK) {
        printf("Failed to set PRX mode\n");
    }
    HAL_Delay(15);

    res = nrf24l01_pwr_up(radio);
    if(res != HAL_OK) {
        printf("Failed to power up\n");
    }
    HAL_Delay(2);

    res = nrf24l01_set_address_width(radio, NRF24L01_AW_5_BYTES);
    if(res != HAL_OK) {
        printf("Failed to set address width\n");
    }
    res = nrf24l01_set_rf_channel(radio, 2500);
    if(res != HAL_OK) {
        printf("Failed to set RF channel\n");
    }
    res = nrf24l01_set_air_data_rate(radio, NRF24L01_ADR_1_MBPS);
    if(res != HAL_OK) {
        printf("Failed to set air data rate\n");
    }
    res = nrf24l01_set_rx_pipe_data_width(radio, 0, 32);
    if(res != HAL_OK) {
        printf("Failed to set RX pipe data width\n");
    }

    res = nrf24l01_flush_rx_fifo(radio);
    if(res != HAL_OK) {
        printf("Failed to flush RX FIFO\n");
    }

    nrf24l01_ce_on(radio);
}

void radio_set_tx_mode(struct nrf24l01_t *radio) {
    HAL_StatusTypeDef res;

    res = nrf24l01_reset(radio);
    if(res != HAL_OK) {
        printf("Failed to reset\n");
    }

    res = nrf24l01_flush_tx_fifo(radio);
    if(res != HAL_OK) {
        printf("Failed to flush TX FIFO\n");
    }

    res = nrf24l01_set_ptx_mode(radio);
    if(res != HAL_OK) {
        printf("Failed to set PTX mode\n");
    }
    HAL_Delay(15);

    res = nrf24l01_pwr_up(radio);
    if(res != HAL_OK) {
        printf("Failed to power up\n");
    }
    HAL_Delay(2);

    res = nrf24l01_set_address_width(radio, NRF24L01_AW_5_BYTES);
    if(res != HAL_OK) {
        printf("Failed to set address width\n");
    }
    res = nrf24l01_set_rf_channel(radio, 2500);
    if(res != HAL_OK) {
        printf("Failed to set RF channel\n");
    }
    res = nrf24l01_set_air_data_rate(radio, NRF24L01_ADR_1_MBPS);
    if(res != HAL_OK) {
        printf("Failed to set air data rate\n");
    }

    res = nrf24l01_flush_tx_fifo(radio);
    if(res != HAL_OK) {
        printf("Failed to flush TX FIFO\n");
    }

    nrf24l01_ce_on(radio);
}

void radio_send(struct nrf24l01_t *radio, uint8_t *data, uint8_t len) {
    HAL_StatusTypeDef res;

    res = nrf24l01_write_tx_fifo(radio, data, len);
    if(res != HAL_OK) {
        printf("Failed to write TX FIFO\n");
    }
}

void radio_receive(struct nrf24l01_t *radio, uint8_t *data, uint8_t len) {
    HAL_StatusTypeDef res;

    res = nrf24l01_read_rx_fifo(radio, data, len);
    if(res != HAL_OK) {
        printf("Failed to read RX FIFO\n");
    }
}

void radio_print_status(struct nrf24l01_t *radio) {
    HAL_StatusTypeDef res;
    uint8_t status;

    res = nrf24l01_get_status(radio, &status);
    if(res != HAL_OK) {
        printf("Failed to get status\n");
    }

    printf("STATUS: 0x%02X\n", status);

    res = nrf24l01_get_fifo_status(radio, &status);

    if(res != HAL_OK) {
        printf("Failed to get FIFO status\n");
    }

    printf("FIFO STATUS: 0x%02X\n", status);
}

void radio_print_config(struct nrf24l01_t *radio) {
    HAL_StatusTypeDef res;
    uint8_t config;

    res = nrf24l01_read_register(radio, NRF24L01_REG_CONFIG, &config, 1, NULL);
    if(res != HAL_OK) {
        printf("Failed to read CONFIG register\n");
    }

    printf("CONFIG: 0x%02X\n", config);
    HAL_Delay(500);

    res = nrf24l01_read_register(radio, NRF24L01_REG_EN_AA, &config, 1, NULL);
    if(res != HAL_OK) {
        printf("Failed to read EN_AA register\n");
    }

    printf("EN_AA: 0x%02X\n", config);
    HAL_Delay(500);

    res = nrf24l01_read_register(radio, NRF24L01_REG_EN_RXADDR, &config, 1, NULL);
    if(res != HAL_OK) {
        printf("Failed to read EN_RXADDR register\n");
    }

    printf("EN_RXADDR: 0x%02X\n", config);
    HAL_Delay(500);

    res = nrf24l01_read_register(radio, NRF24L01_REG_SETUP_AW, &config, 1, NULL);
    if(res != HAL_OK) {
        printf("Failed to read SETUP_AW register\n");
    }

    printf("SETUP_AW: 0x%02X\n", config);
    HAL_Delay(500);

    res = nrf24l01_read_register(radio, NRF24L01_REG_SETUP_RETR, &config, 1, NULL);
    if(res != HAL_OK) {
        printf("Failed to read SETUP_RETR register\n");
    }

    printf("SETUP_RETR: 0x%02X\n", config);
    HAL_Delay(500);

    res = nrf24l01_read_register(radio, NRF24L01_REG_RF_CH, &config, 1, NULL);
    if(res != HAL_OK) {
        printf("Failed to read RF_CH register\n");
    }

    printf("RF_CH: 0x%02X\n", config);
    HAL_Delay(500);

    res = nrf24l01_read_register(radio, NRF24L01_REG_RF_SETUP, &config, 1, NULL);
    if(res != HAL_OK) {
        printf("Failed to read RF_SETUP register\n");
    }

    printf("RF_SETUP: 0x%02X\n", config);
    HAL_Delay(500);

    res = nrf24l01_read_register(radio, NRF24L01_REG_STATUS, &config, 1, NULL);
    if(res != HAL_OK) {
        printf("Failed to read STATUS register\n");
    }

    printf("STATUS: 0x%02X\n", config);
    HAL_Delay(500);

    res = nrf24l01_read_register(radio, NRF24L01_REG_OBSERVE_TX, &config, 1, NULL);
    if(res != HAL_OK) {
        printf("Failed to read OBSERVE_TX register\n");
    }

    printf("OBSERVE_TX: 0x%02X\n", config);
    HAL_Delay(500);

    res = nrf24l01_read_register(radio, NRF24L01_REG_CD, &config, 1, NULL);
    if(res != HAL_OK) {
        printf("Failed to read CD register\n");
    }

    printf("CD: 0x%02X\n", config);
    HAL_Delay(500);

    // uint8_t addr[5] = {0};
    // res = nrf24l01_get_rx_p0_address(radio, addr);
    // if(res != HAL_OK) {
    //     printf("Failed to get RX P0 address\n");
    // }

    // printf("RX P0 address: ");
    // for(int i = 0; i < 5; i++) {
    //     printf("0x%02X ", addr[i]);
    // }
    // printf("\n");
    // HAL_Delay(3000);

    // res = nrf24l01_read_register(radio, NRF24L01_REG_TX_ADDR, addr, 5, NULL);
    // if(res != HAL_OK) {
    //     printf("Failed to read TX_ADDR register\n");
    // }

    // printf("TX_ADDR: ");
    // for(int i = 0; i < 5; i++) {
    //     printf("0x%02X ", addr[i]);
    // }
    // printf("\n");
    // HAL_Delay(3000);

    // res = nrf24l01_read_register(radio, NRF24L01_REG_RX_PW_P0, &config, 1, NULL);
    // if(res != HAL_OK) {
    //     printf("Failed to read RX_PW_P0 register\n");
    // }

    // printf("RX_PW_P0: 0x%02X\n", config);
    // HAL_Delay(3000);

    // res = nrf24l01_read_register(radio, NRF24L01_REG_FIFO_STATUS, &config, 1, NULL);
    // if(res != HAL_OK) {
    //     printf("Failed to read FIFO_STATUS register\n");
    // }

    // printf("FIFO_STATUS: 0x%02X\n", config);
    // HAL_Delay(3000);

    // res = nrf24l01_read_register(radio, NRF24L01_REG_DYNPD, &config, 1, NULL);
    // if(res != HAL_OK) {
    //     printf("Failed to read DYNPD register\n");
    // }

    // printf("DYNPD: 0x%02X\n", config);
    // HAL_Delay(3000);

    // res = nrf24l01_read_register(radio, NRF24L01_REG_FEATURE, &config, 1, NULL);
    // if(res != HAL_OK) {
    //     printf("Failed to read FEATURE register\n");
    // }

    // printf("FEATURE: 0x%02X\n", config);
    // HAL_Delay(3000);
}

void radio_shut_down(struct nrf24l01_t *radio) {
    HAL_StatusTypeDef res;

    nrf24l01_ce_off(radio);

    res = nrf24l01_pwr_down(radio);
    if(res != HAL_OK) {
        printf("Failed to power down\n");
    }
}
