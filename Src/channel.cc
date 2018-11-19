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
#include <algorithm>

using namespace moire;
using namespace std;

void Channel::Init(ChannelDefinition _def)
{
  def = _def;
  ramp_division_quantizer_.Init();
  input.Init(def.input_gpio, def.input_pin);
}

void Channel::Update()
{
  input.Read(kSampleRate);
  switch (mode)
  {
  case LFO:
    ProcessFreeLFO();
    break;
  case ENVELOPE:
    value = 4095;
    break;
  case SHAPE_VIEW:
    value = parameters.secondary;
    break;
  case TAP_LFO:
    ProcessTapLFO(input.phase_inc);
    break;
  }
  if(phase >= 1.0) ResetPhase();
  ProcessGate();
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
  phase = 0;
}

ChannelMode Channel::GetChannelMode()
{
  return mode;
}

uint16_t Channel::GetValue()
{
  return value;
}

bool Channel::GetGate()
{
  return gate_time > 0;
}

void Channel::ProcessFreeLFO()
{
  phase += 1 / (MAX_TIME / parameters.primary) / kSampleRate;
  const float_t slope = static_cast<float_t>(1 / (4028 / parameters.secondary)); 
  ShapeLFO();
}

Ratio divider_ratios[] = {
  { 0.249999f, 4 },
  { 0.333333f, 3 },
  { 0.499999f, 2 },
  { 1, 1 },
  { 1.999999f, 1 },
  { 2.999999f, 1 },
  { 3.999999f, 1 },
};

void Channel::ProcessTapLFO(float_t _phase_inc)
{
  Ratio r = ramp_division_quantizer_.Lookup(divider_ratios, (parameters.primary / 4028.0) * 1.03f, 7);
  if(r.ratio >= 1 && input.clocked) ResetPhase();
  phase += _phase_inc * r.ratio;
  const float_t slope = static_cast<float_t>(1 / (4028 / parameters.secondary)); 
  ShapeLFO();
}

void Channel::ShapeLFO()
{
  const float_t slope = static_cast<float_t>(1 / (4028 / parameters.secondary)); 
  if(phase < slope) {
    value = map(phase, 0, slope, 0, 4096);
  } else {
    value = map(phase, slope, 1, 4096, 0);
  }
}


void Channel::ResetPhase()
{
  phase = 0.0;
  reset = 1;
}

void Channel::ProcessGate()
{
  if(reset) {
    reset = 0;
    HAL_GPIO_WritePin(def.gate_gpio, def.gate_pin, GPIO_PIN_SET);
    gate_time = 0;
  } 
  if(gate_time >= 0) {
    gate_time++;
    if(gate_time > 35) {
      HAL_GPIO_WritePin(def.gate_gpio, def.gate_pin, GPIO_PIN_RESET);
      gate_time = -1;
    }
  }
}