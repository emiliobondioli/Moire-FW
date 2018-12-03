#include "ui.h"
#include "drivers/mux_adc.h"
#include "drivers/gate_input.h"
#include "channel.h"
#include "stmlib/system/system_clock.h"
#include "stm32f3xx_hal.h"
#include "moire.h"
#include "gpio.h"

using namespace std;
using namespace stmlib;
using namespace moire;

UI ui;
MuxAdc mux;

const ChannelDefinition channel_defs[] = {
    {&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, GPIOA, GPIO_PIN_12, GPIOA, GPIO_PIN_15, 1, 0},
    {&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, GPIOA, GPIO_PIN_11, GPIOB, GPIO_PIN_0, 2, 5},
    {&hdac2, DAC_CHANNEL_1, DAC_ALIGN_12B_R, GPIOA, GPIO_PIN_10, GPIOB, GPIO_PIN_1, 4, 3}
};

Channel channel_a;
Channel channel_b;
Channel channel_c;

const size_t kNumChannels = 3;

Channel *channels[] = {
    &channel_a,
    &channel_b,
    &channel_c};

void Moire::Init()
{
  ui.Init();
  mux.Init();
  for (int i = 0; i < kNumChannels; i++)
  {
    channels[i]->Init(channel_defs[i]);
  }
}

void Moire::Update()
{
  if (tick)
  {
    tick = 0;
    ui.Poll();
    for (size_t i = 0; i < kNumChannels; i++)
    {
      int primary = AddCV(mux.value_mux(channels[i]->def.primary_mux), mux.value(i));
      int secondary = mux.value_mux(channels[i]->def.secondary_mux);
      channels[i]->SetParameters(primary, secondary);
      if (ui.switches().released(i))
      {
        ChannelMode currentMode = channels[i]->GetChannelMode();
        int newMode = currentMode + 1 < NUM_MODES ? currentMode + 1 : 0;
        channels[i]->SetChannelMode(static_cast<ChannelMode>(newMode));
      }
      channels[i]->Update();
      ui.SetChannelLeds(i, channels[i]);
    }
  }
}

int Moire::AddCV(int param, int cv) {
  cv -= 2048;
  int sum = param + cv;
  CONSTRAIN(sum, 0, UINT12_MAX);
  return sum;
}
