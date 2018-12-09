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
// User interface.

#ifndef MOIRE_UI_H_
#define MOIRE_UI_H_

#include "stmlib/stmlib.h"
#include "drivers/leds.h"
#include "drivers/switches.h"
#include "channel.h"

namespace moire
{

class UI
{
public:
  UI() {}
  ~UI() {}

  Leds leds;

  void Init();
  void Poll();
  void DoEvents();
  void SetChannelLeds(size_t index, Channel *channel);
  void SetUIHoldValue(size_t index, int16_t time, uint16_t color);
  void SetSliderHoldValue(size_t index, int16_t time, uint16_t color);
  void ClearHoldValues();
  bool IsLongPress(size_t index);
  inline const Switches &switches() const { return _switches; }

private:
  Switches _switches;

  int press_time_[kNumSwitches];

  DISALLOW_COPY_AND_ASSIGN(UI);
};

} // namespace moire

#endif // MOIRE_UI_H_
