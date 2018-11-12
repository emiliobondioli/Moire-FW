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
#include "stmlib/utils/gate_flags.h"
#include <cmath>


using namespace stmlib;

namespace moire {

const float_t kSampleRate = 31250.0f;

struct ChannelParameters
{
  float_t primary;
  float_t secondary;
};

enum ChannelMode
{
    LFO,
    ENVELOPE,
    SHAPE_VIEW,
    NUM_MODES
};

typedef struct ChannelDefinition {
  DAC_HandleTypeDef* dac;
  uint32_t channel;
  uint32_t alignment;
  GPIO_TypeDef* gate_gpio;
  uint16_t gate_pin;
  uint16_t primary_mux;
  uint16_t secondary_mux;
};

class Channel {
 public:
  Channel() { }
  ~Channel() { }
  
  ChannelDefinition def;
  void Init(ChannelDefinition _def);
  void Update(GateFlags* gate_flags, size_t size);
  void SetChannelMode(ChannelMode _mode);
  ChannelMode GetChannelMode();
  void SetParameters(float_t primary, float_t secondary)
  {
    parameters.primary = primary;
    parameters.secondary = secondary;
  }

private:
  ChannelMode mode = LFO;
  float_t value = 0;
  int32_t gate_time = 0;
  bool rising = true;
  void Out();
  void ProcessLFO();
  void ProcessGate();
  DISALLOW_COPY_AND_ASSIGN(Channel);
  ChannelParameters parameters;
  float_t phase = 0;
  const float_t MAX_TIME = 16;
  const float_t phase_inc = (1 / MAX_TIME) / kSampleRate;
  inline float_t map(float_t x, float_t in_min, float_t in_max, float_t out_min, float_t out_max, float_t shape)
  {
    return powf((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min, 1);
  }
  inline float_t mapLog(float_t x, float_t in_min, float_t in_max, float_t out_min, float_t out_max) {
    return out_min + log(x / in_min) / log(in_max/in_min) * (out_max-out_min);
  }
};

}  // namespace moire

#endif  // MOIRE_CHANNEL_H_
