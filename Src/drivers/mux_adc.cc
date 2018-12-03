// Copyright 2017 Olivier Gillet.
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// Drivers for the 12-bit SAR ADC scanning pots and sliders.

#include "drivers/mux_adc.h"
#include "stm32f3xx_hal.h"

namespace moire {

uint16_t mux_address = 0;
const MuxAddressOutputDefinition muxAddressOutputs[] = {
    {GPIOA, GPIO_PIN_7},
    {GPIOA, GPIO_PIN_8},
    {GPIOA, GPIO_PIN_9}
};

/* static */
uint16_t MuxAdc::ADC1Values[kNumAdcChannels];
uint16_t MuxAdc::MuxValues[kNumMuxAddresses];
static uint16_t channel_offset = ADC_CONVERTED_DATA_BUFFER_SIZE / kNumAdcChannels;

void MuxAdc::Init() {
    /* ### - 4 - Start conversion in DMA mode ################################# */
    if (HAL_ADC_Start_DMA(&hadc1,
                          (uint32_t*) ADC1ConvertedData,
                          ADC_CONVERTED_DATA_BUFFER_SIZE) != HAL_OK)
    {
        Error_Handler();
    }
}

uint16_t mux_readings = 0;
uint16_t mux_addresses[] = {0,1,2,3,4,5,6,7};

extern "C"
{
    void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
    {
        int adc_value = 0;
        for (int i = 0; i < kNumAdcChannels; i++)
        {
            int channel_sum = 0;
            for (int j = 0; j < ADC_NUM_SAMPLES; j++) {
                channel_sum += ADC1ConvertedData[channel_offset * j + i * channel_offset / kNumAdcChannels];
            }
            adc_value = channel_sum / ADC_NUM_SAMPLES;
            MuxAdc::ADC1Values[i] = adc_value;
        }
        if(mux_readings >= ADC_NUM_SAMPLES-1) MuxAdc::MuxValues[mux_address] = adc_value;
        mux_readings++;
        if(mux_readings >= ADC_NUM_SAMPLES) {
            mux_address++;
            if(mux_address > kNumMuxAddresses) mux_address = 0;
            for(int i = 0; i < 3; i++)
            {
                bool bit = (mux_addresses[mux_address] >> i) & 1U;
                HAL_GPIO_WritePin(muxAddressOutputs[i].gpio, muxAddressOutputs[i].pin, (GPIO_PinState)bit);
            }
            mux_readings = 0;
        }
    }
    void ADC_IRQHandler()
    {
        HAL_ADC_IRQHandler(&hadc1);
    }
}

}  // namespace moire
