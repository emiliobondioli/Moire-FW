#include "ui.h"
#include "drivers/mux_adc.h"
#include "drivers/gate_inputs.h"
#include "channel.h"
#include "stmlib/system/system_clock.h"
#include "stm32f3xx_hal.h"
#include "moire.h"
#include "gpio.h"
#include "io_buffer.h"

using namespace std;
using namespace stmlib;
using namespace moire;

UI ui;
MuxAdc mux;
IOBuffer io_buffer;
GateInputs gate_inputs;
GateFlags no_gate[kBlockSize];
size_t Moire::current_channel = 0;

const ChannelDefinition channel_defs[] = {
    {&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, GPIOA, GPIO_PIN_10, 0, 0},
    {&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, GPIOA, GPIO_PIN_11, 1, 4},
    {&hdac2, DAC_CHANNEL_1, DAC_ALIGN_12B_R, GPIOA, GPIO_PIN_12, 2, 5}
};

Channel channel_a;
Channel channel_b;
Channel channel_c;

Channel *channels[] = {
    &channel_a,
    &channel_b,
    &channel_c};

IOBuffer::Slice FillBuffer(size_t size) {
  IOBuffer::Slice s = io_buffer.NextSlice(size);
  gate_inputs.Read(s, size);
  return s;
}

void Moire::Init()
{
  ui.Init();
  mux.Init();
  gate_inputs.Init();
  io_buffer.Init();
  for (int i = 0; i < kNumChannels; i++)
  {
    channels[i]->Init(channel_defs[i]);
  }
  std::fill(&no_gate[0], &no_gate[kBlockSize], GATE_FLAG_LOW);
}

void Process(IOBuffer::Block* block, size_t size) {
  channels[Moire::current_channel]->Update(block->input[Moire::current_channel],
        size);
};

void Moire::Update()
{
  if (tick)
  {
    tick = 0;
    ui.Poll();
    for (size_t i = 0; i < kNumChannels; i++)
    {
      int primary = AddCV(mux.value(i), mux.value(1));
      int secondary = mux.value_mux(channels[i]->def.secondary_mux);
      channels[i]->SetParameters(primary, secondary);
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
      io_buffer.Process(&Process);
    }
  }
}

int Moire::AddCV(int param, int cv) {
  int sum = param + cv;
  CONSTRAIN(sum, 0, UINT12_MAX);
  return sum;
}
