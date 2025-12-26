#include "MyPin.h"

MyPin* MyPin::currentPtr = nullptr;

MyPin::MyPin(uint16_t pin, pin_mode_t mode) : MyPin()
{
    SetPin(pin,mode);
}

MyPin::MyPin()
{
    ptrOnOther = currentPtr;
    currentPtr = this;
}

void MyPin::SetPin(uint16_t pin, pin_mode_t mode)
{
    this-> pin = pin;
    this-> mode = mode;
    state = needState = false;

    if(mode == OUT) 
    {
        pinMode(pin,OUTPUT);
        digitalWrite(pin, false);
    }
    else if(mode == IN)
        pinMode(pin,INPUT);
    else if(mode == IN_PULLUP)
        pinMode(pin,INPUT_PULLUP);

    #ifdef ESP32

    else if(mode == IN_PULLDOWN)
        pinMode(pin,INPUT_PULLDOWN);

    #endif
}

void MyPin::Change(fader_t* set)
{
    Change(!needState,set);
}

void MyPin::Change(bool isOn, fader_t* set)
{
    if((isOn != needState) ||                       //  common turn
       ((isOn == state) && (state != needState)))   //  turn back when changing haven't done yet
    {
        needState = isOn;

        if(set != nullptr)
        {
            singlePtr = set;
            if(!firstFaderBrightSet)
            {
                firstFaderBrightSet = true;
                faderBright = (state ? set->onBright : set->offBright);
            } 
        }
        else if(modePtr != nullptr)
        {
            if(!firstFaderBrightSet)
            {
                firstFaderBrightSet = true;
                faderBright = (state ? modePtr->onBright : modePtr->offBright);
            }
        }
        else
        {
            analogWrite(pin,(isOn ? 255 : 0));     //  I've done it especially for correct work Fader functions
            state = isOn;
        }
    }
}

void MyPin::AnalogWrite(uint8_t val)
{
    analogWrite(pin,val);
}

bool MyPin::Read()
{
    return digitalRead(pin);
}

int16_t MyPin::AnalogRead()
{
    return analogRead(pin);
}

uint8_t MyPin::AntiRattleButton(bool signal, uint32_t shortTime, uint32_t longTime) 
{
    bool reader = digitalRead(pin);
    if(!signal)
        reader = !reader;

    if(reader)
    {
        if(ready)
            return 0;

        uint32_t mls = millis();
        if(!btnPreviousReader)
            btnStartMs = mls;
        
        if(mls - btnStartMs > shortTime)
        {
            buttonState = 1;

            if(longTime > shortTime)
            {
                if(mls - btnStartMs > longTime)
                {
                    buttonState = 2;
                    ready = true;
                }
            }
            else
                ready = true;
        }
    }
    else
        ready = false;

    btnPreviousReader = reader;

    if(ready || (!ready && buttonState))
    {
        uint8_t bS = buttonState;
        buttonState = 0;
        return bS;
    }

    return 0;
}

uint32_t MyPin::AntiRattleSensor(bool signal)
{
    bool reader = digitalRead(pin);
    if(!signal)
        reader = !reader;

    uint32_t result = 0;
    if(reader)
    {
        uint32_t mls = millis();
        if(!snsPreviousReader)
            snsStartMs = mls;

        result = mls - snsStartMs;
    }
    snsPreviousReader = reader;
    return result;
}

uint8_t MyPin::GetPin()
{
    return pin;
}

bool MyPin::GetState()
{
    return state;
}