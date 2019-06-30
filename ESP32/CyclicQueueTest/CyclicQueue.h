#ifndef _CYCLICQUEUE_H_INCLUDED
#define _CYCLICQUEUE_H_INCLUDED

template <typename T>
class CyclicQueue
{
public:
  T *buf;
  uint32_t buffersiz;
  bool overriden = false;
  uint32_t sp, ep;
  int32_t len = 0;
  CyclicQueue(T *bp, uint32_t siz)
  {
    buf = bp;
    buffersiz = siz;
    clear();
  }
  void clear()
  {
    sp = 0;
    ep = 0;
    len = 0;
    for (uint32_t i = 0; i < buffersiz; i++)
      buf[i] = 0;
  }
  bool push(T val)
  {
    buf[sp] = val;
    sp = (sp + 1) % buffersiz;
    if (sp == ep)
    {
      overriden = true;
      ep = (ep + 1) % buffersiz;
      return false;
    }
    else
    {
      len++;
      return true;
    }
  }
  bool pop(T *val)
  {
    if (ep == sp)
    {
      return true;
    }
    else
    {
      *val = buf[ep];
      ep = (ep + 1) % buffersiz;
      len--;
      return false;
    }
  }
};
#endif