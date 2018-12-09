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
    {&hdac2, DAC_CHANNEL_1, DAC_ALIGN_12B_R, GPIOA, GPIO_PIN_10, GPIOB, GPIO_PIN_1, 4, 3}};

Channel channel_a;
Channel channel_b;
Channel channel_c;

const size_t kNumChannels = 3;
const uint32_t kEditTimeout = 2000;
bool ui_locks[kNumChannels];
int selected_channel = -1;
int edit_channel = -1;

Channel *channels[] = {
    &channel_a,
    &channel_b,
    &channel_c};

int16_t edit_timeout = -1;

void Moire::Init()
{
  ui.Init();
  mux.Init();
  for (int i = 0; i < kNumChannels; i++)
  {
    channels[i]->Init(channel_defs[i]);
  }
}

inline void UpdateEditUI()
{
  if (edit_timeout >= 0 && edit_channel >= 0)
  {
    ChannelSettings settings = channels[edit_channel]->GetSettings();
    ChannelMode mode = channels[edit_channel]->GetChannelMode();
    ui.SetUIHoldValue(0, 2, (uint16_t)settings.alpha);
    ui.SetUIHoldValue(1, 2, (uint16_t)settings.beta);
    ui.SetUIHoldValue(2, 2, (uint16_t)settings.gamma);
    edit_timeout--;
  }
  else
  {
    edit_channel = -1;
  }
}

void Moire::Update()
{
  if (tick)
  {
    tick = 0;
    ui.Poll();
    UpdateEditUI();
    for (size_t i = 0; i < kNumChannels; i++)
    {
      int primary = AddCV(mux.value_mux(channels[i]->def.primary_mux), mux.value(i));
      int secondary = mux.value_mux(channels[i]->def.secondary_mux);
      channels[i]->SetParameters(primary, secondary);
      if (edit_timeout < 0)
      {
        if (ui.switches().pressed(i))
        {
          if (selected_channel < 0 && !ui_locks[i])
            selected_channel = i;
          if (ui.IsLongPress(i))
          {
            edit_timeout = kEditTimeout;
            edit_channel = i;
            ui_locks[i] = true;
          }
        }
        if (ui.switches().released(i))
        {
          if (selected_channel >= 0 && selected_channel != i)
          {
            SetTriggerSource(i);
          } else if(!ui_locks[i]) {
            SetChannelMode(i);
          }
          if (selected_channel == i)
          {
            selected_channel = -1;
          }
          ui_locks[i] = false;
        }
      }
      else
      {
        ui.SetSliderHoldValue(i, 2, i == edit_channel ? 1 : 0);
        if (ui.switches().released(i))
        {
          if (ui_locks[i])
          {
            ui_locks[i] = false;
          }
          else
          {
            switch (i)
            {
            case 0:
              channels[edit_channel]->NextAlphaSetting();
              break;
            case 1:
              channels[edit_channel]->NextBetaSetting();
              break;
            case 2:
              channels[edit_channel]->NextGammaSetting();
              break;
            }
          }
          edit_timeout = kEditTimeout;
        }
      }
      channels[i]->Update();
      ui.SetChannelLeds(i, channels[i]);
    }
  }
}

void Moire::SetChannelMode(int channel_id)
{
  ChannelMode currentMode = channels[channel_id]->GetChannelMode();
  int newMode = currentMode + 1 < NUM_MODES ? currentMode + 1 : 0;
  channels[channel_id]->SetChannelMode(static_cast<ChannelMode>(newMode));
}

void Moire::SetTriggerSource(int source)
{
  bool toggle_state = channels[selected_channel]->input.SetInputSource(&channels[source]->gate);
  ui.SetUIHoldValue(source, 1000, (uint16_t)toggle_state + 1);
  ui.SetUIHoldValue(selected_channel, 1000, 3);
  ui_locks[selected_channel] = true;
  ui_locks[source] = true;
  selected_channel = -1;
}

int Moire::AddCV(int param, int cv)
{
  cv -= 2048;
  int sum = param + cv;
  CONSTRAIN(sum, 0, UINT12_MAX);
  return sum;
}

