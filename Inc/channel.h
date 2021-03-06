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
#include "stmlib/dsp/hysteresis_quantizer.h"
#include "dac.h"
#include "tim.h"
#include "turing.h"
#include "stmlib/utils/gate_flags.h"
#include "drivers/gate_input.h"

#include <cmath>


using namespace stmlib;

namespace moire {

const float_t kSampleRate = 31250.0f;
const size_t kMaxTouringLength = 16;
const size_t kNumSettings = 3;
const uint16_t MAX_PARAM_VALUE = 4096;

struct ChannelParameters
{
  float_t primary;
  float_t secondary;
};

struct ChannelSettings
{
  size_t alpha;
  size_t beta;
  size_t gamma;
};

enum ChannelMode
{
    LFO,
    ENVELOPE,
    TURING,
    TAP_LFO,
    NUM_MODES
};

typedef struct ChannelDefinition {
  DAC_HandleTypeDef* dac;
  uint32_t channel;
  uint32_t alignment;
  GPIO_TypeDef* gate_gpio;
  uint16_t gate_pin;
  GPIO_TypeDef* input_gpio;
  uint16_t input_pin;
  uint16_t primary_mux;
  uint16_t secondary_mux;
};

struct Ratio {
  float ratio;  // Precomputed p/q
  int q;
};

static inline float_t map(float_t x, float_t in_min, float_t in_max, float_t out_min, float_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static inline float_t map_log(float_t x, float_t in_max, float_t out_max)
{
  return expf(x / (in_max / logf(out_max + 1))) - 1;
}

class Channel {
 public:
  Channel() { }
  ~Channel() { }
  
  ChannelDefinition def;
  void Init(ChannelDefinition _def);
  void Update();
  void SetChannelMode(ChannelMode _mode);
  ChannelMode GetChannelMode();
  uint16_t GetValue();
  bool GetGate();
  bool gate;
  void SetParameters(float_t primary, float_t secondary)
  {
    parameters.primary = primary;
    parameters.secondary = secondary;
  }
  void NextAlphaSetting() {
    settings[(size_t)mode].alpha = settings[(size_t)mode].alpha < kNumSettings ? settings[(size_t)mode].alpha + 1 : 0;
  }
  void NextBetaSetting() {
    settings[(size_t)mode].beta = settings[(size_t)mode].beta < kNumSettings ? settings[(size_t)mode].beta + 1 : 0;
  }
  void NextGammaSetting() {
    settings[(size_t)mode].gamma = settings[(size_t)mode].gamma < kNumSettings ? settings[(size_t)mode].gamma + 1 : 0;
  }
  ChannelSettings GetSettings() {
    return settings[(size_t)mode];
  }
  GateInput input;

private:
  ChannelMode mode = LFO;
  float_t value = 0;
  int32_t gate_time = 0;
  bool reset = true;
  void Out();
  void GateOut();
  void ProcessFreeLFO();
  void ProcessTapLFO(float_t _phase_inc);
  void ShapeLFO();
  void ProcessGate();
  void ResetPhase();
  void ProcessTuring();
  DISALLOW_COPY_AND_ASSIGN(Channel);
  ChannelParameters parameters;
  ChannelSettings settings[NUM_MODES];
  TuringMachine tm;
  stmlib::HysteresisQuantizer ramp_division_quantizer_;
  float_t phase = 0;
  const float_t MAX_TIME = 16;
};

}  // namespace moire

#endif  // MOIRE_CHANNEL_H_
