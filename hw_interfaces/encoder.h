#pragma once
#include "button.h"

class RotaryEncoder 
{
   public:
   virtual ~RotaryEncoder();
   void Release();
   RotaryEncoder(int gpioA, int gpioB);
   void Init(int gpioA, int gpioB);
   RotaryEncoder();

protected:
   virtual void onEncoder(int way) = 0;

private:
   void setGPIOs();
   void onEvent(int gpio, int level, uint32_t tick);
   static void _cback(int gpio, int level, uint32_t tick, void *user);

   int gpioA;
   int gpioB;
   int levA;
   int levB;
   int lastGpio;
   bool initialized;
};
