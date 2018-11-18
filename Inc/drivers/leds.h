// Copyright 2016 Olivier Gillet.
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
// Drivers for the column of LEDs.

#ifndef MOIRE_DRIVERS_LEDS_H_
#define MOIRE_DRIVERS_LEDS_H_

#include "stmlib/stmlib.h"

namespace moire {

const int kNumLEDs = 9;

struct RGLedDefinition {
  uint16_t address_r;
  uint16_t address_g;
};

struct SliderLedDefinition {
  uint16_t address_r;
  uint16_t address_g;
};

enum LedColor {
  LED_COLOR_OFF = 0,
  LED_COLOR_RED = 0xff0000,
  LED_COLOR_GREEN = 0x00ff00,
  LED_COLOR_YELLOW = 0xffff00,
};

class Leds {
 public:
  Leds() { }
  ~Leds() { }
  
  void Init();
  void Write();
  void Clear();
  void SetOutputLed(size_t index, uint16_t value);
  void SetSliderLed(size_t index, uint16_t value);
  void SetUILed(size_t index, uint16_t value);
  
  void set(int index, uint32_t color) {
    colors_[index] = color;
  }

  void mask(int index, uint32_t color) {
    colors_[index] |= color;
  }
  
 private:
  uint32_t colors_[kNumLEDs];
  RGLedDefinition ui_leds[kNumLEDs];
  RGLedDefinition output_leds[kNumLEDs];
  SliderLedDefinition slider_leds[kNumLEDs];
  DISALLOW_COPY_AND_ASSIGN(Leds);
};

}  // namespace moire

#endif  // MOIRE_DRIVERS_LEDS_H_
