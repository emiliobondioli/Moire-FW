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

struct ChannelParameters
{
  float primary;
  float secondary;
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
  void SetParameters(float primary, float secondary)
  {
    parameters.primary = primary;
    parameters.secondary = secondary;
  }

private:
  ChannelMode mode = LFO;
  float value = 0;
  void Out();
  DISALLOW_COPY_AND_ASSIGN(Channel);
  ChannelParameters parameters;
};

}  // namespace moire

#endif  // MOIRE_CHANNEL_H_
