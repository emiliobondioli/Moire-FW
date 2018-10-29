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
// Channel class

#ifndef MOIRE_CHANNEL_H_
#define MOIRE_CHANNEL_H_

#include "stmlib/stmlib.h"
#include "dac.h"
#include "tim.h"

using namespace stmlib;

namespace moire {

const uint16_t sine_wave_array[32] = {2047, 1648, 1264, 910, 600,  345,   
                156, 39,  0,  39,  156,  345,  
                600, 910, 1264, 1648, 2048, 2447,  
                2831, 3185, 3495, 3750, 3939, 4056,  
                4095, 4056, 3939, 3750, 3495, 3185,  
                2831, 2447};  

enum ChannelMode
{
    LFO,
    ENVELOPE,
    CLOCK_DIVIDER
};

typedef struct ChannelDefinition {
  DAC_HandleTypeDef* dac;
  uint32_t channel;
  uint32_t alignment;
};

class Channel {
 public:
  Channel() { }
  ~Channel() { }
  
  ChannelDefinition def;
  void Init(ChannelDefinition _def);
  void Update();
  void SetChannelMode(ChannelMode _mode);
  ChannelMode GetChannelMode();
  
  
 private:
  ChannelMode mode = LFO;
  float value = 0;
  void Out();
  DISALLOW_COPY_AND_ASSIGN(Channel);
};

}  // namespace moire

#endif  // MOIRE_CHANNEL_H_
