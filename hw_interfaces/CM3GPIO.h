#ifndef CM3GPIO_H
#define CM3GPIO_H

#include <stdio.h>  
#include <stdlib.h>  
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "../OledScreen.h"
#include "../OSC/OSCMessage.h"
#include "../OSC/SimpleWriter.h"
#include "../SLIPEncodedSerial.h"
#include "../Serial.h"

#include "led.h"
#include "dispdrv.h"
#include "encoder.h"
#include "button.h"

class CM3GPIO
{
    public:
        CM3GPIO();
        void init();
        void poll();
        void pollKnobs();
        void updateOLED(OledScreen &s);
        void ping();
        void shutdown();
        void setLED(unsigned c);
        void clearFlags();

        uint32_t encBut;
        uint32_t encButFlag;
        uint32_t encTurn;
        uint32_t encTurnFlag;
        uint32_t knobFlag;
        uint32_t adcs[8];
        uint32_t footswitch;
        uint32_t footswitchFlag;
        uint32_t keyStates;
        uint32_t keyStatesLast;
        uint32_t keyFlag;
        
        // organelle m specific stuff
        uint32_t pwrStatus;
        uint32_t micSelSwitch;
	    float batteryVoltage;
	    uint32_t batteryBars;
        bool lowBatteryShutdown;

    private:        
        uint32_t shiftRegRead();
        void getEncoder();
        void getKeys();
        uint32_t adcRead(uint8_t adcnum);
        void displayPinValues();
	    void checkFootSwitch ();
        
        uint32_t pinValues;
        uint32_t pinValuesLast;
		LED red;
		LED green;
        LED blue;
        DisplayDriver oled;
       

        class Footswitch : public Button
        {
            public:
                Footswitch()
                {
                    foot_last = foot_cur = 0;
                }

                bool Changed(int &curStatus)
                {
                    if (foot_cur != foot_last)
                    {
                        curStatus = foot_last = foot_cur;
                        return true;
                    }
                    return false;
                }
        
            protected:
            virtual void onButtonDown() 
            {
                foot_cur=1;
            }

            virtual void onButtonUp() 
            {
                foot_cur=0;
            }

            private:
            uint8_t foot_last;
            uint8_t foot_cur;
        };      
        Footswitch fswitch;      

        PulseButton auxbtn;

        class Encoder : public RotaryEncoder
        {
        public:
            Encoder() : RotaryEncoder()  {direction = 0; }
            void Init(int enca, int encb, int btn)
            {
                RotaryEncoder::Init(enca, encb);
                butn.Init(btn);
            }
            bool Rotating(int &dir) 
            {
                dir = direction;
                direction = 0;
                return dir != 0;
            }
            bool ButtonChanged(int &curStatus) {return butn.Changed(curStatus);}

        private:
            virtual void onEncoder(int way) {direction=way; }

        private:
            int direction;
            Footswitch butn;
        };
        Encoder encdr;

    }
};


#endif
