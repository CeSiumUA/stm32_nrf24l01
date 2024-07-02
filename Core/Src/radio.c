/*
 * radio.c
 *
 *  Created on: Jun 28, 2024
 *      Author: mtgin
 */

#include "radio.h"

static bool radio_irq;
static bool radio_data_ready = false;
static int radio_available_data_pipe = 0;

void radio_raise_irq_flag(void)
{
    radio_irq = true;
}

void radio_process_irq(struct nrf24_t *radio)
{
    uint8_t status;
    nrf24_hal_status_t hal_status;
    uint8_t pipe;
    
    if(!radio_irq)
    {
        return;
    }

    hal_status = nrf24_get_status(radio, &status);

    if(hal_status != HAL_OK)
    {
        printf("Error getting status: %d\n", hal_status);
        return;
    }

    if(status & NRF24_REG_STATUS_MASK_TX_DS)
    {
        printf("Data sent, ACK received!\n");
        status |= NRF24_REG_STATUS_MASK_TX_DS;
    }

    if(status & NRF24_REG_STATUS_MASK_RX_DR)
    {
        printf("Data received!\n");
        radio_data_ready = true;
        status |= NRF24_REG_STATUS_MASK_RX_DR;
    }

    if(status & NRF24_REG_STATUS_MASK_MAX_RT)
    {
        printf("Max retries reached!\n");
        status |= NRF24_REG_STATUS_MASK_MAX_RT;
    }

    if((status & NRF24_REG_STATUS_MASK_RX_P_NO)){
        pipe = (status & NRF24_REG_STATUS_MASK_RX_P_NO) >> 1;
        if(pipe < 6){
            radio_available_data_pipe = pipe;
        }
    }

    hal_status = nrf24_set_status(radio, &status);
    if(hal_status != HAL_OK)
    {
        printf("Error clearing status: %d\n", hal_status);
        return;
    }

    radio_irq = false;
}

void radio_shut_down(struct nrf24_t *radio)
{
    nrf24_hal_status_t hal_status;

    nrf24_ce_off(radio);

    hal_status = nrf24_power_down(radio);
    if(hal_status != HAL_OK)
    {
        printf("Error shutting down radio: %d\n", hal_status);
        return;
    }
}