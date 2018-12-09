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

const int32_t kLongPressDuration = 2000;
const size_t kNumUILeds = 3;

namespace moire
{
struct LedHoldValue
{
  int16_t time;
  uint16_t color;
};

LedHoldValue hold_ui_values[kNumUILeds] = {
    {-1, 0},
    {-1, 0},
    {-1, 0}
};

LedHoldValue hold_slider_values[kNumUILeds] = {
    {-1, 0},
    {-1, 0},
    {-1, 0}
};

uint16_t out_pwm[3];
void UI::Init()
{
  _switches.Init();
  system_clock.Init();
  leds.Init();
  fill(&press_time_[0], &press_time_[kNumSwitches], 0);
}

void UI::SetChannelLeds(size_t index, Channel *channel)
{
  if(hold_ui_values[index].time < 0) {
    leds.SetUILed(index, (uint16_t)channel->GetChannelMode());
  } else {
    leds.SetUILed(index, hold_ui_values[index].color);
    hold_ui_values[index].time--;
  }
  if(hold_slider_values[index].time < 0) {
    leds.SetSliderLed(index, channel->GetGate());
  } else {
    leds.SetSliderLed(index, hold_slider_values[index].color);
    hold_slider_values[index].time--;
  } 
}

void UI::SetUIHoldValue(size_t index, int16_t _time, uint16_t _color) 
{
  hold_ui_values[index].time = _time;
  hold_ui_values[index].color = _color;
}

void UI::SetSliderHoldValue(size_t index, int16_t _time, uint16_t _color) 
{
  hold_slider_values[index].time = _time;
  hold_slider_values[index].color = _color;
}

void UI::ClearHoldValues() 
{
  for(int i = 0; i < kNumUILeds; i++) {
    hold_ui_values[i].time = -1;
  }
}

bool UI::IsLongPress(size_t index) {
  return press_time_[index] < 0;
}

void UI::Poll()
{
  system_clock.Tick();
  _switches.Debounce();

  for (int i = 0; i < kNumSwitches; ++i)
  {
    if (_switches.pressed(i))
    {
      if (press_time_[i] != -1)
      {
        ++press_time_[i];
      }
      // long press
      if (press_time_[i] > kLongPressDuration)
      {

        press_time_[i] = -1;
      }
    }
    else
    {
      // short press
      if (press_time_[i] > 0)
      {
      }
      press_time_[i] = 0;
    }
  }

  leds.Write();
}
} // namespace moire
