/*
 * temperature.h
 *
 *  Created on: Jun 30, 2024
 *      Author: mtgin
 */

#ifndef INC_TEMPERATURE_H_
#define INC_TEMPERATURE_H_

#include <stdint.h>

// Some defines are added for possible better temperature calculation algorithm

#define TEMPERATURE_AVG_SLPOE                           (2.5f)
#define TEMPERATURE_V_AT_25C                            (0.76f)
#define TEMPERATURE_V_REF_INT                           (1.21f)
#define TEMPERATURE_TS_CAL1_ADDR                        ((uint16_t *)(0x1FFF7A2CU)
#define TEMPERATURE_TS_CAL2_ADDR                        ((uint16_t *)(0x1FFF7A2EU)
#define TEMPERATURE_TS_CAL1_TEMP                        (30.0f)
#define TEMPERATURE_TS_CAL2_TEMP                        (110.0f)
#define TEMPERATURE_TS_CAL_VREF                         (3300UL)
#define TEMPERATURE_TS_CAL_VREFANALOG                   (3300UL)
#define TEMPERATURE_V_REF_INT_CAL                       ((uint16_t *)0x1FFF7A2AU)
#define TEMPERATURE_ADC_BUFFER_SIZE                     (2000)

float temperature_calculate(uint16_t *adc_value);

#endif /* INC_TEMPERATURE_H_ */
