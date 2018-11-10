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
  private:
    int AddCV(int param, int cv);
};

#endif /* __MOIRE_H__ */