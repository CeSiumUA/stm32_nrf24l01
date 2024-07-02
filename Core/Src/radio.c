/*
 * radio.c
 *
 *  Created on: Jun 28, 2024
 *      Author: mtgin
 */

#include "radio.h"

static bool radio_data_ready = false;
static bool tx_ack_received = false;
static bool tx_max_retries_reached = false;
static int radio_available_data_pipe = 0;

enum radio_operation_result_t radio_process_irq(struct radio_t *radio)
{
    uint8_t status;
    nrf24_hal_status_t hal_status;
    uint8_t pipe;

    hal_status = nrf24_get_status(radio->nrf_radio, &status);

    if(hal_status != HAL_OK)
    {
        printf("Error getting status: %d\n", hal_status);
        return RADIO_ERROR;
    }

    if(status & NRF24_REG_STATUS_MASK_TX_DS)
    {
        printf("Data sent, ACK received!\n");
        tx_ack_received = true;
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
        tx_max_retries_reached = true;
        status |= NRF24_REG_STATUS_MASK_MAX_RT;
    }

    if((status & NRF24_REG_STATUS_MASK_RX_P_NO)){
        pipe = (status & NRF24_REG_STATUS_MASK_RX_P_NO) >> 1;
        if(pipe < 6){
            radio_available_data_pipe = pipe;
        }
    }

    hal_status = nrf24_set_status(radio->nrf_radio, &status);
    if(hal_status != HAL_OK)
    {
        printf("Error clearing status: %d\n", hal_status);
        return RADIO_ERROR;
    }

    return RADIO_OK;
}

enum radio_operation_result_t radio_shut_down(struct radio_t *radio)
{
    nrf24_hal_status_t hal_status;

    nrf24_ce_off(radio->nrf_radio);

    hal_status = nrf24_power_down(radio->nrf_radio);
    if(hal_status != HAL_OK)
    {
        printf("Error shutting down radio: %d\n", hal_status);
        return RADIO_ERROR;
    }

    return RADIO_OK;
}

enum radio_operation_result_t radio_init(struct radio_t *radio){
    HAL_StatusTypeDef res;

    res = nrf24_soft_reset(radio->nrf_radio);
    if(res != HAL_OK)
    {
        printf("Error resetting radio: %d\n", res);
        return RADIO_ERROR;
    }

    res = nrf24_flush_rx_fifo(radio->nrf_radio);
    if(res != HAL_OK)
    {
        printf("Error flushing RX FIFO: %d\n", res);
        return RADIO_ERROR;
    }

    res = nrf24_flush_tx_fifo(radio->nrf_radio);
    if(res != HAL_OK)
    {
        printf("Error flushing TX FIFO: %d\n", res);
        return RADIO_ERROR;
    }

    res = nrf24_set_address_width(radio->nrf_radio, radio->address_width);
    if(res != HAL_OK)
    {
        printf("Error setting address width: %d\n", res);
        return RADIO_ERROR;
    }

    res = nrf24_set_radio_data_rate(radio->nrf_radio, radio->data_rate);
    if(res != HAL_OK)
    {
        printf("Error setting air data rate: %d\n", res);
        return RADIO_ERROR;
    }

    res = nrf24_set_radio_channel(radio->nrf_radio, radio->channel);
    if(res != HAL_OK)
    {
        printf("Error setting channel: %d\n", res);
        return RADIO_ERROR;
    }

    res = nrf24_set_prx_mode(radio->nrf_radio);
    if(res != HAL_OK)
    {
        printf("Error setting PRX mode: %d\n", res);
        return RADIO_ERROR;
    }

    HAL_Delay(5);

    for(int i = 0; i < 6; i++)
    {
        res = nrf24_set_en_rx_pipe(radio->nrf_radio, i, true);
        if(res != HAL_OK)
        {
            printf("Error enabling RX pipe %d: %d\n", i, res);
            return RADIO_ERROR;
        }

        res = nrf24_set_rx_payload_width(radio->nrf_radio, i, radio->data_width);
        if(res != HAL_OK)
        {
            printf("Error setting RX payload width for pipe %d: %d\n", i, res);
            return RADIO_ERROR;
        }
    }

    res = nrf24_power_up(radio->nrf_radio);
    if(res != HAL_OK)
    {
        printf("Error powering up radio: %d\n", res);
        return RADIO_ERROR;
    }

    HAL_Delay(5);

    nrf24_ce_on(radio->nrf_radio);

    return RADIO_OK;
}

enum radio_operation_result_t radio_receive(struct radio_t *radio, uint8_t *data)
{
    nrf24_hal_status_t hal_status;
    HAL_StatusTypeDef res;

    if(!radio_data_ready || !radio->is_in_rx_mode)
    {
        return RADIO_RETRY;
    }

    res = nrf24_read_rx_fifo(radio->nrf_radio, data, radio->data_width);
    if(res != HAL_OK)
    {
        printf("Error reading RX payload: %d\n", res);
        return RADIO_ERROR;
    }

    radio_data_ready = false;

    hal_status = nrf24_flush_rx_fifo(radio->nrf_radio);
    if(hal_status != HAL_OK)
    {
        printf("Error flushing RX FIFO: %d\n", hal_status);
        return RADIO_ERROR;
    }

    return RADIO_OK;
}

enum radio_operation_result_t radio_send(struct radio_t *radio, uint8_t *data)
{
    nrf24_hal_status_t res;
    uint32_t start_time;
    enum radio_operation_result_t radio_res = RADIO_OK;

    if(radio_data_ready || radio->is_in_rx_mode)
    {
        return RADIO_RETRY;
    }

    nrf24_ce_off(radio->nrf_radio);

    tx_max_retries_reached = false;
    tx_ack_received = false;

    res = nrf24_flush_tx_fifo(radio->nrf_radio);
    if(res != HAL_OK)
    {
        printf("Error flushing TX FIFO: %d\n", res);
        return RADIO_ERROR;
    }

    res = nrf24_set_ptx_mode(radio->nrf_radio);
    if(res != HAL_OK)
    {
        printf("Error setting PTX mode: %d\n", res);
        return RADIO_ERROR;
    }

    res = nrf24_write_tx_fifo(radio->nrf_radio, data, radio->data_width);
    if(res != HAL_OK)
    {
        printf("Error writing TX payload: %d\n", res);
        return RADIO_ERROR;
    }

    nrf24_ce_on(radio->nrf_radio);

    start_time = HAL_GetTick();

    while(!tx_ack_received)
    {
        if(tx_max_retries_reached)
        {
            printf("Max retries reached, no ACK received!\n");
            radio_res = RADIO_ERROR;
            goto radio_send_revert_to_rx;
        }

        if((HAL_GetTick() - start_time) % 5000 == 0)
        {
            printf("No ACK received in 5 seconds, retrying...\n");
        }
    }

    printf("Data sent, ACK received!\n");

radio_send_revert_to_rx:
    nrf24_ce_off(radio->nrf_radio);

    res = nrf24_set_prx_mode(radio->nrf_radio);
    if(res != HAL_OK)
    {
        printf("Error setting PRX mode: %d\n", res);
        return RADIO_ERROR;
    }

    nrf24_ce_on(radio->nrf_radio);

    return radio_res;
}