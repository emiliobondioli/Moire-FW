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

#ifndef MOIRE_TURING_H_
#define MOIRE_TURING_H_

#include "stmlib/stmlib.h"

#include <cmath>

using namespace stmlib;

namespace moire
{
const size_t kMaxTuringLength = 16;

class TuringMachine
{
public:
  TuringMachine() {}
  ~TuringMachine() {}

  void Init();
  void Randomize(int index);
  uint16_t GetCV();
  bool GetGate();
  size_t GetLength();
  size_t SetLength(size_t _length);
  size_t GetChance();
  size_t SetChance(size_t _chance);
  void Next();

private:
  uint16_t cv_values[kMaxTuringLength];
  bool gate_values[kMaxTuringLength];
  size_t length = kMaxTuringLength;
  size_t chance = 100;
  size_t current_step = 0;
  size_t direction = 1;
};

} // namespace moire

#endif // MOIRE_TURING_H_
