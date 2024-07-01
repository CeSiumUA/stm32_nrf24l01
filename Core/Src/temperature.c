/*
 * temperature.c
 *
 *  Created on: Jun 30, 2024
 *      Author: mtgin
 */
#include "temperature.h"

float temperature_calculate(uint16_t *adc_value)
{
    float v_sense = 0.0f;
    float v_ref = 0.0f;
    float temperature = 0.0f;

    v_ref = (float)((V_REF_INT * 4095.0) / adc_value[0]);
    v_sense = (float)((v_ref * adc_value[1]) / 4095.0);
    temperature = (float)((v_sense - V_AT_25C) * 1000.0f/ AVG_SLPOE) + 25.0f;

    return temperature;
}