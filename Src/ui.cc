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
// User interface

#include "ui.h"
#include <algorithm>
#include "stmlib/system/system_clock.h"

using namespace std;
using namespace stmlib;

const int32_t kLongPressDuration = 1000;

namespace moire {
uint16_t out_pwm[3];
void UI::Init() {
  switches_.Init();
  system_clock.Init();
  leds.Init();
  fill(&press_time_[0], &press_time_[kNumSwitches], 0);
}

void UI::SetChannelLeds(uint16_t index, Channel* channel) {
    leds.SetUILed(index, (uint16_t) channel->GetChannelMode());
    uint16_t out_value = channel->GetValue();
    out_pwm[index]+=2.5;
    if(out_pwm[index] > map(out_value, 0, UINT12_MAX, 50, 0)) {
      out_pwm[index] = 0;
      leds.SetOutputLed(index, 3);
    } else {
      leds.SetOutputLed(index, 0);
    }
    
    /* else {
      leds.SetOutputLed(index, 0);
       if(out_value > 2048) leds.SetOutputLed(index, 2);
      else leds.SetOutputLed(index, 1); 
    }*/
    leds.SetSliderLed(index, channel->GetGate());
  }

void UI::Poll() {
  system_clock.Tick();
  switches_.Debounce();

  for (int i = 0; i < kNumSwitches; ++i) {
    if (switches_.pressed(i)) {
      if (press_time_[i] != -1) {
        ++press_time_[i];
      }
      // long press
      if (press_time_[i] > kLongPressDuration) {

          press_time_[i] = -1;
      }
    } else {
      // short press
      if (press_time_[i] > 0) {

      }
      press_time_[i] = 0;
    }
  }

  leds.Write();
}
}  // namespace moire
