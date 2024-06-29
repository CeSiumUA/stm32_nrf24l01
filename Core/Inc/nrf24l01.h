/*
 * nrf24l01.h
 *
 *  Created on: May 9, 2024
 *      Author: mtgin
 */

#ifndef INC_NRF24L01_H_
#define INC_NRF24L01_H_

#include <stdint.h>
#ifdef __KERNEL__
#else
#include "stm32f4xx_hal.h"
#endif

#include <string.h>

#ifdef __KERNEL__
#else
typedef SPI_HandleTypeDef nrf24l01_spi_t;
typedef GPIO_TypeDef nrf24l01_gpio_t;
#endif

/*Commands*/
#define NRF24L01_CMD_R_REGISTER(reg)                (0x00 | (reg & 0x1F))
#define NRF24L01_CMD_W_REGISTER(reg)                (0x20 | (reg & 0x1F))
#define NRF24L01_CMD_R_RX_PAYLOAD                   (0x61)
#define NRF24L01_CMD_W_TX_PAYLOAD                   (0xA0)
#define NRF24L01_CMD_FLUSH_TX                       (0xE1)
#define NRF24L01_CMD_FLUSH_RX                       (0xE2)
#define NRF24L01_CMD_REUSE_TX_PL                    (0xE3)
#define NRF24L01_CMD_ACTIVATE                       (0x50)
#define NRF24L01_CMD_R_RX_PL_WID                    (0x60)
#define NRF24L01_CMD_W_ACK_PAYLOAD(pipe)            (0xA8 | (pipe & 0x07))
#define NRF24L01_CMD_W_TX_PAYLOAD_NOACK             (0xB0)
#define NRF24L01_CMD_NOP                            (0xFF)

/*Registers*/
#define NRF24L01_REG_CONFIG                         (0x00)
#define NRF24L01_REG_CONFIG_RESET_VALUE             (0b00001000)

#define NRF2RL01_REG_CONFIG_MASK_PRIM_RX            (1 << 0)
#define NRF2RL01_REG_CONFIG_MASK_PWR_UP             (1 << 1)
#define NRF2RL01_REG_CONFIG_MASK_CRCO               (1 << 2)
#define NRF2RL01_REG_CONFIG_MASK_EN_CRC             (1 << 3)
#define NRF2RL01_REG_CONFIG_MASK_MASK_MAX_RT        (1 << 4)
#define NRF2RL01_REG_CONFIG_MASK_MASK_TX_DS         (1 << 5)
#define NRF2RL01_REG_CONFIG_MASK_MASK_RX_DR         (1 << 6)

#define NRF24L01_REG_EN_AA                          (0x01)
#define NRF24L01_REG_EN_AA_RESET_VALUE              (0b00111111)

#define NRF24L01_REG_EN_AA_MASK_ENAA_P0             (1 << 0)
#define NRF24L01_REG_EN_AA_MASK_ENAA_P1             (1 << 1)
#define NRF24L01_REG_EN_AA_MASK_ENAA_P2             (1 << 2)
#define NRF24L01_REG_EN_AA_MASK_ENAA_P3             (1 << 3)
#define NRF24L01_REG_EN_AA_MASK_ENAA_P4             (1 << 4)
#define NRF24L01_REG_EN_AA_MASK_ENAA_P5             (1 << 5)

#define NRF24L01_REG_EN_RXADDR                      (0x02)
#define NRF24L01_REG_EN_RXADDR_RESET_VALUE          (0b00000011)

#define NRF24L01_REG_EN_RXADDR_MASK_ERX_P0          (1 << 0)
#define NRF24L01_REG_EN_RXADDR_MASK_ERX_P1          (1 << 1)
#define NRF24L01_REG_EN_RXADDR_MASK_ERX_P2          (1 << 2)
#define NRF24L01_REG_EN_RXADDR_MASK_ERX_P3          (1 << 3)
#define NRF24L01_REG_EN_RXADDR_MASK_ERX_P4          (1 << 4)
#define NRF24L01_REG_EN_RXADDR_MASK_ERX_P5          (1 << 5)

#define NRF24L01_REG_SETUP_AW                       (0x03)
#define NRF24L01_REG_SETUP_AW_RESET_VALUE           (0b00000011)

#define NRF24L01_REG_SETUP_AW_MASK_3_BYTES          (0x01)
#define NRF24L01_REG_SETUP_AW_MASK_4_BYTES          (0x02)
#define NRF24L01_REG_SETUP_AW_MASK_5_BYTES          (0x03)

#define NRF24L01_REG_SETUP_RETR                     (0x04)
#define NRF24L01_REG_SETUP_RETR_RESET_VALUE         (0b00000011)

#define NRF24L01_REG_RF_CH                          (0x05)
#define NRF24L01_REG_RF_CH_RESET_VALUE              (0b00000010)

#define NRF24L01_REG_RF_SETUP                       (0x06)
#define NRF24L01_REG_RF_SETUP_RESET_VALUE           (0b00001111)

#define NRF24L01_REG_STATUS                         (0x07)
#define NRF24L01_REG_STATUS_RESET_VALUE             (0b01111110)

#define NRF24L01_REG_OBSERVE_TX                     (0x08)
#define NRF24L01_REG_OBSERVE_TX_RESET_VALUE         (0b00000000)

#define NRF24L01_REG_CD                             (0x09)
#define NRF24L01_REG_RX_ADDR_P0                     (0x0A)
#define NRF24L01_REG_RX_ADDR_P1                     (0x0B)
#define NRF24L01_REG_RX_ADDR_P2                     (0x0C)
#define NRF24L01_REG_RX_ADDR_P3                     (0x0D)
#define NRF24L01_REG_RX_ADDR_P4                     (0x0E)
#define NRF24L01_REG_RX_ADDR_P5                     (0x0F)
#define NRF24L01_REG_TX_ADDR                        (0x10)
#define NRF24L01_REG_RX_PW_P0                       (0x11)
#define NRF24L01_REG_RX_PW_P1                       (0x12)
#define NRF24L01_REG_RX_PW_P2                       (0x13)
#define NRF24L01_REG_RX_PW_P3                       (0x14)
#define NRF24L01_REG_RX_PW_P4                       (0x15)
#define NRF24L01_REG_RX_PW_P5                       (0x16)
#define NRF24L01_REG_RX_PW_RESET_VALUE              (0b00000000)

#define NRF24L01_REG_FIFO_STATUS                    (0x17)
#define NRF24L01_REG_DYNPD                          (0x1C)
#define NRF24L01_REG_DYNPD_RESET_VALUE              (0b00000000)

#define NRF24L01_REG_FEATURE                        (0x1D)
#define NRF24L01_REG_FEATURE_RESET_VALUE            (0b00000000)

#define NRF24L01_BASE_FREQUENCY                     (2400)

struct nrf24l01_t {
    nrf24l01_spi_t *spi;
    nrf24l01_gpio_t *csn_port;
    uint16_t csn_pin;
    nrf24l01_gpio_t *ce_port;
    uint16_t ce_pin;
    nrf24l01_gpio_t *irq_port;
    uint16_t irq_pin;
};

enum nrf24l01_address_width_t {
    NRF24L01_AW_ILLEGAL = 0,
    NRF24L01_AW_3_BYTES,
    NRF24L01_AW_4_BYTES,
    NRF24L01_AW_5_BYTES
};

enum nrf24l01_auto_retransmit_delay_t {
    NRF24L01_ARD_250_US = 0,
    NRF24L01_ARD_500_US,
    NRF24L01_ARD_750_US,
    NRF24L01_ARD_1000_US,
    NRF24L01_ARD_1250_US,
    NRF24L01_ARD_1500_US,
    NRF24L01_ARD_1750_US,
    NRF24L01_ARD_2000_US,
    NRF24L01_ARD_2250_US,
    NRF24L01_ARD_2500_US,
    NRF24L01_ARD_2750_US,
    NRF24L01_ARD_3000_US,
    NRF24L01_ARD_3250_US,
    NRF24L01_ARD_3500_US,
    NRF24L01_ARD_3750_US,
    NRF24L01_ARD_4000_US
};

enum nrf24l01_auto_retransmit_count_t {
    NRF24L01_ARC_DISABLED = 0,
    NRF24L01_ARC_1,
    NRF24L01_ARC_2,
    NRF24L01_ARC_3,
    NRF24L01_ARC_4,
    NRF24L01_ARC_5,
    NRF24L01_ARC_6,
    NRF24L01_ARC_7,
    NRF24L01_ARC_8,
    NRF24L01_ARC_9,
    NRF24L01_ARC_10,
    NRF24L01_ARC_11,
    NRF24L01_ARC_12,
    NRF24L01_ARC_13,
    NRF24L01_ARC_14,
    NRF24L01_ARC_15
};

enum nrf24l01_air_data_rate_t {
    NRF24L01_ADR_1_MBPS = 0,
    NRF24L01_ADR_2_MBPS

};

void nrf24l01_init(struct nrf24l01_t *dev, SPI_HandleTypeDef *spi);
void nrf24l01_set_csn(struct nrf24l01_t *dev, GPIO_TypeDef *port, uint16_t pin);
void nrf24l01_set_ce(struct nrf24l01_t *dev, GPIO_TypeDef *port, uint16_t pin);
void nrf24l01_set_irq(struct nrf24l01_t *dev, GPIO_TypeDef *port, uint16_t pin);
void nrf24l01_ce_on(struct nrf24l01_t *dev);
void nrf24l01_ce_off(struct nrf24l01_t *dev);
void nrf24l01_csn_on(struct nrf24l01_t *dev);
void nrf24l01_csn_off(struct nrf24l01_t *dev);
HAL_StatusTypeDef nrf24l01_pwr_up(struct nrf24l01_t *dev);
HAL_StatusTypeDef nrf24l01_pwr_down(struct nrf24l01_t *dev);
HAL_StatusTypeDef nrf24l01_set_ptx_mode(struct nrf24l01_t *dev);
HAL_StatusTypeDef nrf24l01_set_prx_mode(struct nrf24l01_t *dev);
HAL_StatusTypeDef nrf24l01_set_address_width(struct nrf24l01_t *dev, enum nrf24l01_address_width_t width);
HAL_StatusTypeDef nrf24l01_set_auto_retransmit_delay(struct nrf24l01_t *dev, enum nrf24l01_auto_retransmit_delay_t delay);
HAL_StatusTypeDef nrf24l01_set_auto_retransmit_count(struct nrf24l01_t *dev, enum nrf24l01_auto_retransmit_count_t count);
HAL_StatusTypeDef nrf24l01_set_rf_channel(struct nrf24l01_t *dev, int channel);
HAL_StatusTypeDef nrf24l01_set_air_data_rate(struct nrf24l01_t *dev, enum nrf24l01_air_data_rate_t rate);
HAL_StatusTypeDef nrf24l01_get_status(struct nrf24l01_t *dev, uint8_t *status);
HAL_StatusTypeDef nrf24l01_get_status_nop(struct nrf24l01_t *dev, uint8_t *status);
HAL_StatusTypeDef nrf24l01_set_tx_address(struct nrf24l01_t *dev, uint8_t *address, uint8_t len);
HAL_StatusTypeDef nrf24l01_set_rx_pipe_data_width(struct nrf24l01_t *dev, uint8_t pipe, uint8_t width);
HAL_StatusTypeDef nrf24l01_read_rx_fifo(struct nrf24l01_t *dev, uint8_t *data, uint8_t len);
HAL_StatusTypeDef nrf24l01_write_tx_fifo(struct nrf24l01_t *dev, uint8_t *data, uint8_t len);
HAL_StatusTypeDef nrf24l01_flush_tx_fifo(struct nrf24l01_t *dev);
HAL_StatusTypeDef nrf24l01_flush_rx_fifo(struct nrf24l01_t *dev);
HAL_StatusTypeDef nrf24l01_get_fifo_status(struct nrf24l01_t *dev, uint8_t *status);
HAL_StatusTypeDef nrf24l01_reset(struct nrf24l01_t *dev);
HAL_StatusTypeDef nrf24l01_read_register(struct nrf24l01_t *dev, uint8_t reg, uint8_t *data, uint8_t len, uint8_t *status_out);
HAL_StatusTypeDef nrf24l01_write_register(struct nrf24l01_t *dev, uint8_t reg, uint8_t *data, uint8_t len, uint8_t *status_out);

#endif /* INC_NRF24L01_H_ */
