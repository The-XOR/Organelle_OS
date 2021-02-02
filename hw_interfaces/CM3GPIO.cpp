#include <pigpio.h>
#include "CM3GPIO.h"

#define AUX_LED_RED_OFF     red.Off();
#define AUX_LED_RED_ON      red.On();
#define AUX_LED_GREEN_OFF   green.Off();
#define AUX_LED_GREEN_ON    green.On();
#define AUX_LED_BLUE_OFF    blue.Off();
#define AUX_LED_BLUE_ON     blue.On();

CM3GPIO::CM3GPIO() 
{
}

void CM3GPIO::init()
{
    gpioInitialise();
    green.Init(25);
    red.Init(16);
    blue.Init(24);
    oled.Init(0x3c,128,64);
    fswitch.Init(13);
    auxbtn.Init(19);
    encdr.Init(20,21,12);

    // keys
    keyStatesLast = 0;
    clearFlags();

    // get initial pin states
    shiftRegRead();
    pinValuesLast = pinValues;

    // set 
    micSelSwitch = 0;
    batteryVoltage = 5;
    batteryBars = 5;
    lowBatteryShutdown = false;
}

void CM3GPIO::clearFlags() 
{
    encButFlag = 0;
    encTurnFlag = 0;
    knobFlag = 0;
    keyFlag = 0;
    footswitchFlag = 0;
}

void CM3GPIO::poll()
{

    // read keys (updates pinValues)
    shiftRegRead();

    // get key values if a key pin changed (ignore the encoder pins)
    if ((pinValues & 0xFFFFFF80) != (pinValuesLast & 0xFFFFFF80)) {
        getKeys();
        keyFlag = 1;
    }
    pinValuesLast = pinValues;

    micSelSwitch = (pinValues >> 3) & 1;
    
    // check encoder, gotta check every time for debounce purposes
    getEncoder();
    
}

void CM3GPIO::pollKnobs()
{    

    adcs[0] = adcRead(0);
    adcs[1] = adcRead(1);
    adcs[2] = adcRead(2);
    adcs[3] = adcRead(3);
    adcs[4] = adcRead(4);
    adcs[5] = adcRead(5);
    adcs[6] = adcRead(7);

    
    checkFootSwitch();

    knobFlag = 1;
}

void CM3GPIO::updateOLED(OledScreen &s)
{
    oled.Render(s.pix_buf, 1024);
}

void CM3GPIO::ping()
{
}

void CM3GPIO::shutdown() 
{
}

void CM3GPIO::setLED(unsigned stat) 
{

    stat %= 8;

    if (stat == 0) {
        AUX_LED_RED_OFF;
        AUX_LED_GREEN_OFF;
        AUX_LED_BLUE_OFF;
    }
    else if (stat == 1) {
        AUX_LED_RED_ON;
        AUX_LED_GREEN_OFF;
        AUX_LED_BLUE_OFF;
    }
    else if (stat == 2) {
        AUX_LED_RED_ON;
        AUX_LED_GREEN_ON;
        AUX_LED_BLUE_OFF;
    }
    else if (stat == 3) {
        AUX_LED_RED_OFF;
        AUX_LED_GREEN_ON;
        AUX_LED_BLUE_OFF;
    }
    else if (stat == 4) {
        AUX_LED_RED_OFF;
        AUX_LED_GREEN_ON;
        AUX_LED_BLUE_ON;
    }
    else if (stat == 5) {
        AUX_LED_RED_OFF;
        AUX_LED_GREEN_OFF;
        AUX_LED_BLUE_ON;
    }
    else if (stat == 6) {
        AUX_LED_RED_ON;
        AUX_LED_GREEN_OFF;
        AUX_LED_BLUE_ON;
    }
    else if (stat == 7) {
        AUX_LED_RED_ON;
        AUX_LED_GREEN_ON;
        AUX_LED_BLUE_ON;
    }
}

uint32_t CM3GPIO::shiftRegRead(void)
{
    uint32_t bytesVal = auxbtn.Down() ? 1 : 0; //todo svagare quale bit e il tasto aux
    
    pinValues = bytesVal;
    return(bytesVal);
}

void CM3GPIO::getKeys(void){
    keyStates = 0;
    
    keyStates |= (pinValues >> (0 + 7) & 1) << 24;
    keyStates |= (pinValues >> (1 + 7) & 1) << 16;
    keyStates |= (pinValues >> (2 + 7) & 1) << 17;
    keyStates |= (pinValues >> (3 + 7) & 1) << 18;
    keyStates |= (pinValues >> (4 + 7) & 1) << 19;
    keyStates |= (pinValues >> (5 + 7) & 1) << 20;
    keyStates |= (pinValues >> (6 + 7) & 1) << 21;
    keyStates |= (pinValues >> (7 + 7) & 1) << 22;
   
    keyStates |= (pinValues >> (8 + 7) & 1) << 23;
    keyStates |= (pinValues >> (9 + 7) & 1) << 8;
    keyStates |= (pinValues >> (10 + 7) & 1) << 9;
    keyStates |= (pinValues >> (11 + 7) & 1) << 10;
    keyStates |= (pinValues >> (12 + 7) & 1) << 11;
    keyStates |= (pinValues >> (13 + 7) & 1) << 12;
    keyStates |= (pinValues >> (14 + 7) & 1) << 13;
    keyStates |= (pinValues >> (15 + 7) & 1) << 14;
    
    keyStates |= (pinValues >> (16 + 7) & 1) << 15;
    keyStates |= (pinValues >> (17 + 7) & 1) << 0;
    keyStates |= (pinValues >> (18 + 7) & 1) << 1;
    keyStates |= (pinValues >> (19 + 7) & 1) << 2;
    keyStates |= (pinValues >> (20 + 7) & 1) << 3;
    keyStates |= (pinValues >> (21 + 7) & 1) << 4;
    keyStates |= (pinValues >> (22 + 7) & 1) << 5;
    keyStates |= (pinValues >> (23 + 7) & 1) << 6;
    
    keyStates |= (pinValues >> (24 + 7) & 1) << 7;
    
    keyStates |= (0xFE000000);  // zero out the bits not key bits
    keyStates = ~keyStates;
}

void CM3GPIO::getEncoder(void)
{
     if(encdr.ButtonChanged(encBut))
        encButFlag = 1;

	// turning
    int direction;
    if(encdr.Rotating(direction))
    {
        encTurnFlag = 1; 
	    encTurn = direction == 1 ? 1 : 0;
    }
}

uint32_t CM3GPIO::adcRead(uint8_t adcnum)
{ 
   return 0;
    
}

void CM3GPIO::checkFootSwitch (void) 
{
    if(fswitch.Changed(footswitch))
        footswitchFlag = 1;
}
