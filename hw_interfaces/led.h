#pragma once

class LED
{
    public:
        LED(int pin);
        LED();
        ~LED();
        void Init(int pin);
        void Release();
        void Set(bool on);
        void On();
        void Off();

    private:
        int io_pin;    
};