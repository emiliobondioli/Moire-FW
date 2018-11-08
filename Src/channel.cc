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

using namespace moire;

void Channel::Init(ChannelDefinition _def)
{
  def = _def;
}

void Channel::Update()
{
  switch(mode)
  {
    case LFO:
      value += 0.1;
      if (value >= 4096) value = 0;
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

void Channel::ProcessLFO()
{
  
}
