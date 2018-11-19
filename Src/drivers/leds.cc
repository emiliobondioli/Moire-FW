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

#include "drivers/leds.h"

#include <algorithm>

#include "stm32f3xx_hal.h"
#include "spi.h"
#include "gpio.h"
#include "stmlib/system/system_clock.h"


namespace moire {

using namespace std;

void Leds::Init() {
  HAL_SPI_MspInit(&hspi1);
  __HAL_SPI_ENABLE(&hspi1);
  Clear();
}

RGLedDefinition ui_leds[] = {
  {5, 4, 0},
  {7, 6, 0},
  {2, 3, 0}
};

RGLedDefinition output_leds[] = {
  {9, 8, 0},
  {10, 11, 0},
  {12, 13, 0}
};

SliderLedDefinition slider_leds[] = {
  {14, 0},
  {15, 0},
  {1, 0}
};

void Leds::Clear() {

}

void Leds::SetOutputLed(size_t index, uint16_t value) {
  output_leds[index].color = value;
}

void Leds::SetSliderLed(size_t index, uint16_t value) {
  slider_leds[index].color = value;
}

void Leds::SetUILed(size_t index, uint16_t value) {
  ui_leds[index].color = value;
}

void Leds::Write() {
  uint16_t leds_data = 0b00000000000000;
  for(int i = 0; i < 3; i++) {
    leds_data = GetRGLedData(ui_leds[i], leds_data);
    leds_data = GetRGLedData(output_leds[i], leds_data);
    leds_data = GetSliderLedData(slider_leds[i], leds_data);
  }
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
  //HAL_SPI_Transmit(&hspi1, (uint8_t *)leds_data, 1, HAL_MAX_DELAY); 
  SPI1->DR = leds_data;
}
}  // namespace moire


/*
  // Clear();
   set_time++;
  if(set_time > 500) {
    if(leds_data == 0b00000000000000){
      leds_data = 0b00000000000000;
    }
    else leds_data = 0b00000000000000;
    set_time = 0; 
    // SET
    //leds_data |= 0x1 << current_set + 1;
    // TOGGLE
    //leds_data ^= 1UL << current_set + 1;
    current_set++;
  }
  if(current_set > 15) current_set = 0;
*/