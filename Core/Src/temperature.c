/*
 * temperature.c
 *
 *  Created on: Jun 30, 2024
 *      Author: mtgin
 */
#include "temperature.h"

float temperature_calculate(uint16_t *adc_value)
{
    float temperature = 0.0f;
    float vdda, vref = 0.0f;
    float vsense = 0.0f;
    uint32_t temp_sum = 0;
    uint32_t vref_sum = 0;
    uint16_t temp_avg, vref_avg = 0;
    for(int i = 0; i < (TEMPERATURE_ADC_BUFFER_SIZE / 4); i++)
    {
        uint16_t tmp_val = adc_value[i * 2];
        uint16_t vref_val = adc_value[i * 2 + 1];
        temp_sum += (uint32_t)tmp_val;
        vref_sum += (uint32_t)vref_val;
    }

    temp_avg = temp_sum / (TEMPERATURE_ADC_BUFFER_SIZE / 4);
    vref_avg = vref_sum / (TEMPERATURE_ADC_BUFFER_SIZE / 4);

    vdda = (float)TEMPERATURE_TS_CAL_VREF * (float)(*TEMPERATURE_V_REF_INT_CAL) / (float)vref_avg / 1000.0f;

    // vref = (float)vdda / 4095 * vref_avg;

    vref = (float)((TEMPERATURE_V_REF_INT * 4095.0f)/vref_avg);
    vsense = (float)((float)temp_avg * vref / 4095.0f);

    temperature = (float)((vsense - TEMPERATURE_V_AT_25C) * 1000.0f) / TEMPERATURE_AVG_SLPOE + 25.0f;

    return temperature;
}