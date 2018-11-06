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

#ifndef MOIRE_DRIVERS_POTS_ADC_H_
#define MOIRE_DRIVERS_POTS_ADC_H_

#include "stmlib/stmlib.h"
#include "adc.h"

namespace moire {

const int kNumAdcChannels = 4;
const int kNumMuxPots = 3;
const int kNumMuxSliders = 3;
const int kNumMuxAddresses = kNumMuxPots + kNumMuxSliders;
const int ADC_NUM_SAMPLES = 4;
const int ADC_CONVERTED_DATA_BUFFER_SIZE = 32;
static uint16_t ADC1ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];

class MuxAdc
{
public:
  MuxAdc() { }
  ~MuxAdc() { }
  
  void Init();
  void Poll();

  inline int32_t value(int channel) const {
    return static_cast<int32_t>(ADC1Values[channel]);
  }
  
  inline float float_value(int channel) const {
    return static_cast<float>(value(channel)) / 65536.0f;
  }
  
  inline uint8_t pot_index() const {
    return pot_index_;
  }
  
  inline uint8_t slider_index() const {
    return slider_index_;
  }
  static uint16_t ADC1Values[kNumAdcChannels];
  
 private:
  int mux_address_;
  uint8_t pot_index_;
  uint8_t slider_index_;

  static uint8_t mux_address_to_pot_index_[kNumMuxPots];
  static uint8_t mux_address_to_slider_index_[kNumMuxSliders];
  
  DISALLOW_COPY_AND_ASSIGN(MuxAdc);
};

}  // namespace moire

#endif  // MOIRE_DRIVERS_POTS_ADC_H_