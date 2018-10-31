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

/* static */
uint8_t MuxAdc::mux_address_to_pot_index_[kNumMuxPots] = {
    1,
    2,
    3
};

/* static */
uint8_t MuxAdc::mux_address_to_slider_index_[kNumMuxSliders] = {
    4,
    5,
    6
};

uint16_t MuxAdc::aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];

void MuxAdc::Init() {
    /* ### - 4 - Start conversion in DMA mode ################################# */
    if (HAL_ADC_Start_DMA(&hadc1,
                          (uint32_t *)aADCxConvertedData,
                          ADC_CONVERTED_DATA_BUFFER_SIZE) != HAL_OK)
    {
        Error_Handler();
    }
}

void MuxAdc::DeInit() {

}

void MuxAdc::Poll() {

}

void MuxAdc::Convert() {
  if (conversion_done_) {

  } else {

  }
  conversion_done_ = !conversion_done_;
}

extern "C"
{
    void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
    {
        for (int i = 0; i < kNumAdcChannels; i++)
        {
            int channel_sum = 0;
            channel_sum += aADCxConvertedData[kNumAdcChannels * 2 * i + i];
            channel_sum += aADCxConvertedData[kNumAdcChannels * 2 * i + i];
            channel_sum += aADCxConvertedData[kNumAdcChannels * 2 * i + i];
            channel_sum += aADCxConvertedData[kNumAdcChannels * 2 * i + i];
             values_[i] = ;
        }
    }
    void ADC_IRQHandler()
    {
        HAL_ADC_IRQHandler(&hadc1);
    }
}

}  // namespace moire
