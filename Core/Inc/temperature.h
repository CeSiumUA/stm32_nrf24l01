/*
 * temperature.h
 *
 *  Created on: Jun 30, 2024
 *      Author: mtgin
 */

#ifndef INC_TEMPERATURE_H_
#define INC_TEMPERATURE_H_

#include <stdint.h>

#define AVG_SLPOE                           (2.5f)
#define V_AT_25C                            (0.76f)
#define V_REF_INT                           (1.21f)

float temperature_calculate(uint16_t *adc_value);

#endif /* INC_TEMPERATURE_H_ */
