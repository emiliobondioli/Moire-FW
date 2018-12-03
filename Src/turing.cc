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

#include "turing.h"
#include "stmlib/utils/random.h"

#include "stmlib/dsp/units.h"

#include <cassert>
#include <algorithm>

using namespace moire;
using namespace std;

const uint16_t gate_threshold = UINT16_MAX / 2;

void TuringMachine::Init()
{
  Random::Seed(rand());
  for(int i = 0; i < kMaxTuringLength; i++) {
    Randomize(i);
  }
}

void TuringMachine::Randomize(int index) {
  cv_values[index] = Random::GetSample();
  uint16_t r = Random::GetSample();
  gate_values[index] = r > gate_threshold;
}

void TuringMachine::Next() {
  uint16_t c =static_cast<float_t>(UINT16_MAX / (Random::GetSample() + 1)) * 100;
  if( c < chance) {
    Randomize(current_step);
  }
  current_step += direction;
  if(current_step >= length) current_step = 0;
}

uint16_t TuringMachine::GetCV() {
  return cv_values[current_step];
}

bool TuringMachine::GetGate() {
  return gate_values[current_step];
}

size_t TuringMachine::GetLength() {
  return length;
}

size_t TuringMachine::SetLength(size_t _length) {
  length = _length;
}

size_t TuringMachine::GetChance() {
  return chance;
}

size_t TuringMachine::SetChance(size_t _chance) {
  chance = _chance;
}
