/*
 * radio.h
 *
 *  Created on: Jun 28, 2024
 *      Author: mtgin
 */

#ifndef INC_RADIO_H_
#define INC_RADIO_H_

#include "nrf24.h"
#include "retarget.h"

struct radio_t
{
    struct nrf24_t *nrf_radio;
    enum nrf24_address_width_t address_width;
    enum nrf24_air_data_rate_t data_rate;
    uint8_t data_width;
    uint8_t channel;
    bool is_in_rx_mode;
};

enum radio_operation_result_t
{
    RADIO_OK = 0,
    RADIO_ERROR,
    RADIO_RETRY,
};

void radio_raise_irq_flag(void);
enum radio_operation_result_t radio_process_irq(struct radio_t *radio);
enum radio_operation_result_t radio_shut_down(struct radio_t *radio);
enum radio_operation_result_t radio_receive(struct radio_t *radio, uint8_t *data);
enum radio_operation_result_t radio_init(struct radio_t *radio);
enum radio_operation_result_t radio_send(struct radio_t *radio, uint8_t *data);

#endif /* INC_RADIO_H_ */
