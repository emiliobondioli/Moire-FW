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
  { A_DAC_OUT_GPIO_Port, A_DAC_OUT_Pin },
  { B_DAC_OUT_GPIO_Port, B_DAC_OUT_Pin },
  { C_DAC_OUT_GPIO_Port, C_DAC_OUT_Pin }
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
    channels[i]->Update();
  }
}
