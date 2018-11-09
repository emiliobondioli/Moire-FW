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

#include "channel.h"
#include "stmlib/dsp/dsp.h"
#include "stmlib/dsp/parameter_interpolator.h"
#include "stmlib/dsp/units.h"

#include <cassert>
#include <cmath>
#include <algorithm>

#include "resources.h"

using namespace moire;
using namespace std;

void Channel::Init(ChannelDefinition _def)
{
  def = _def;
}

void Channel::Update()
{
  switch (mode)
  {
  case LFO:
    ProcessLFO();
    break;
  case ENVELOPE:
    value = 4095;
    break;
  case SHAPE_VIEW:
    value = parameters.secondary;
    break;
  }
  Out();
}

void Channel::Out()
{
  uint32_t value_byte = static_cast<uint32_t>(value);
  HAL_DAC_SetValue(def.dac, def.channel, def.alignment, value_byte);
}

void Channel::SetChannelMode(ChannelMode _mode)
{
  mode = _mode;
  value = 0;
}

ChannelMode Channel::GetChannelMode()
{
  return mode;
}

float map(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Channel::ProcessLFO()
{
  float inc = 1 / (MAX_TIME / parameters.primary) / kSampleRate;
  phase += inc;
  float slope = 1 / (4096 / parameters.secondary); 
  float triangle = 0;
  if(phase < slope) {
    triangle = map(phase, 0, slope, 0, 1);
  } else {
    triangle = map(phase, slope, 1, 1, 0);
  }
  value = triangle * 4096;
  if(phase >= 1.0) phase -= 1.0;
}