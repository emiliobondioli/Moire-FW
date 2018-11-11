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
// Driver for the six gate/trigger inputs.

#include "drivers/gate_inputs.h"
#include "stm32f3xx_hal.h"
#include "gpio.h"


namespace moire {

using namespace std;
using namespace stmlib;

struct GateInputDefinition {
  GPIO_TypeDef* gpio;
  uint16_t pin;
};

const GateInputDefinition gate_input_definition[] = {
  { GPIOA, GPIO_PIN_15 },
  { GPIOA, GPIO_PIN_0 },
  { GPIOB, GPIO_PIN_1 },
};

void GateInputs::Init() {

}

void GateInputs::Read(const IOBuffer::Slice& slice, size_t size) {
  for (size_t i = 0; i < kNumChannels; ++i) {
    previous_flags_[i] = ExtractGateFlags(
        previous_flags_[i],
        !(gate_input_definition[i].gpio->IDR & gate_input_definition[i].pin));
    slice.block->input[i][slice.frame_index] = previous_flags_[i];
  }
  
  // Extend gate input data to the next samples.
  for (size_t j = 1; j < size; ++j) {
    for (size_t i = 0; i < kNumChannels; ++i) {
      slice.block->input[i][slice.frame_index + j] = \
          previous_flags_[i] & GATE_FLAG_HIGH;
    }
  }
}

}  // namespace moire
