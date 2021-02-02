#include <pigpio.h>
#include "rotary_encoder.h"

RotaryEncoder::RotaryEncoder(int _gpioA, int _gpioB)
{
   Init(_gpioA, _gpioB);
}

RotaryEncoder::RotaryEncoder()
{
   gpioA = gpioB = -1;
   levA = levB = lastGpio = 0;
}

void RotaryEncoder::Init(int _gpioA, int _gpioB)
{
   gpioA = _gpioA;
   gpioB =_gpioB;
   levA = levB = lastGpio = 0;
   setGPIOs();
}

RotaryEncoder::~RotaryEncoder()
{
   Release();
}

/*

             +---------+         +---------+      0
             |         |         |         |
   A         |         |         |         |
             |         |         |         |
   +---------+         +---------+         +----- 1

       +---------+         +---------+            0
       |         |         |         |
   B   |         |         |         |
       |         |         |         |
   ----+         +---------+         +---------+  1

*/

void RotaryEncoder::_cback(int gpio, int level, uint32_t tick, void *user)
{
   ((RotaryEncoder *)user)->onEvent(gpio, level, tick);
}

void RotaryEncoder::onEvent(int gpio, int level, uint32_t tick)
{
   if(!initialized)
      return;

   if (gpio == gpioA) 
      levA = level; 
   else 
      levB = level;

   if (gpio != lastGpio) /* debounce */
   {
      lastGpio = gpio;

      if ((gpio == gpioA) && (level == 1))
      {
         if (levB) 
            onEncoder(1);
      } else if ((gpio == gpioB) && (level == 1))
      {
         if (levA) 
            onEncoder(-1);
      }
   }
}

void RotaryEncoder::setGPIOs()
{
   gpioSetMode(gpioA, PI_INPUT);
   gpioSetMode(gpioB, PI_INPUT);
   /* pull up is needed as encoder common is grounded */
   gpioSetPullUpDown(gpioA, PI_PUD_UP);
   gpioSetPullUpDown(gpioB, PI_PUD_UP);
   gpioSetAlertFuncEx(gpioA, _cback, this);
   gpioSetAlertFuncEx(gpioB, _cback, this);

   initialized = true;
}

void RotaryEncoder::Release()
{
   initialized = false;
   if(gpioA >= 0)
   {
      gpioSetAlertFunc(gpioA, 0);
      gpioSetAlertFunc(gpioB, 0);
      gpioA = gpioB = -1;
   }
}

