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
uint8_t PotsAdc::mux_address_to_pot_index_[kNumMuxAddresses] = {
  0xff,
  2,
  4,
  1,
  3,
  0,
  0xff,
  5,
};

/* static */
uint8_t PotsAdc::mux_address_to_slider_index_[kNumMuxAddresses] = {
  3,
  2,
  1,
  4,
  0,
  0xff,
  5,
  0xff,
};

void PotsAdc::Init() {

}

void PotsAdc::DeInit() {

}

void PotsAdc::Convert() {
  if (conversion_done_) {

  } else {

  }
  conversion_done_ = !conversion_done_;
}

}  // namespace moire
