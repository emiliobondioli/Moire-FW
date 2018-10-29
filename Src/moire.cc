#include "ui.h"
#include "channel.h"
#include "stmlib/system/system_clock.h"
#include "stm32f3xx_hal.h"
#include "moire.h"

using namespace std;
using namespace stmlib;
using namespace moire;

UI ui;

const int kNumChannels = 3;

const ChannelDefinition channel_defs[] = {
    {&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R},
    {&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R},
    {&hdac2, DAC_CHANNEL_1, DAC_ALIGN_12B_R}
};

Channel channel_a;
Channel channel_b;
Channel channel_c;

Channel* channels[] = {
  &channel_a,
  &channel_b,
  &channel_c
};

void Moire::Init() {
  ui.Init();
  for(int i = 0; i < kNumChannels; i++)
  {
    channels[i]->Init(channel_defs[i]);
  }
}

void Moire::Update() {
  ui.Poll();
  for(int i = 0; i < kNumChannels; i++)
  {
    if (ui.switches().pressed(i))
    {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
      if(channels[i]->GetChannelMode() == LFO) channels[i]->SetChannelMode(ENVELOPE);
      else channels[i]->SetChannelMode(LFO);
    }
    else
    {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
    }
    channels[i]->Update();
  }

}
