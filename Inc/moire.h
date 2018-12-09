#ifndef __MOIRE_H__
#define __MOIRE_H__

#include "stmlib/stmlib.h"

class Moire {
  public:
    Moire() { }
    ~Moire() { }

    void Update();
    void Init();
    bool tick = 0;
    static size_t current_channel;
  private:
    int AddCV(int param, int cv);
    void SetTriggerSource(int channel_id);
    void SetChannelMode(int channel_id);
};

#endif /* __MOIRE_H__ */