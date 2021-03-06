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

#include "drivers/gate_input.h"
#include "stm32f3xx_hal.h"

namespace moire {

using namespace std;
using namespace stmlib;

void GateInput::Init(GPIO_TypeDef* input_gpio, uint16_t input_pin) {
  def.gpio = input_gpio;
  def.pin = input_pin;
}

void GateInput::Read(const float_t sample_rate) {
  bool state = clock_source == NULL ? ReadExternal() : ReadInternal();
  if(state != current_state) {
    if(!current_state) {
      if(pulses > 0) {
        pulses = 0;
        phase_inc =  static_cast<float_t>(1.0 / timer);
        clocked = true;
        period = timer;
      }
      timer = 0;
      pulses++;
    }
    current_state = state;
  } else {
    clocked = false;
  }
  timer++;
  if(timer > period) {
    phase_inc =  static_cast<float_t>(1.0 / timer);
  }
}

bool GateInput::SetInputSource(bool* source) {
  bool toggle = clock_source != source;
  if(toggle) {
    clock_source = source;
  } else {
    clock_source = NULL;
  }
  return toggle;
}

bool GateInput::ReadExternal() {
  return (bool) HAL_GPIO_ReadPin(def.gpio, def.pin);
}

bool GateInput::ReadInternal() {
  return *clock_source;
}

}  // namespace moire
