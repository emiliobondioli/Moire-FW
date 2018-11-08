#include "ui.h"
#include "drivers/mux_adc.h"
#include "channel.h"
#include "stmlib/system/system_clock.h"
#include "stm32f3xx_hal.h"
#include "moire.h"

using namespace std;
using namespace stmlib;
using namespace moire;

UI ui;
MuxAdc mux;

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
  mux.Init();
  for(int i = 0; i < kNumChannels; i++)
  {
    channels[i]->Init(channel_defs[i]);
  }
}

void Moire::Update() {
  ui.Poll();
  for(int i = 0; i < kNumChannels; i++)
  {
    int val = mux.value(i);
    channels[i]->SetParameters(val, val);
    if (ui.switches().released(i))
    {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
      ChannelMode currentMode = channels[i]->GetChannelMode();
      int newMode = currentMode + 1 < NUM_MODES ? currentMode + 1 : 0;
      channels[i]->SetChannelMode(static_cast<ChannelMode>(newMode));
    }
    else
    {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
    }
    channels[i]->Update();
  }

}
