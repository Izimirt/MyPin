#include "MyPin.h"

MyPin::MyPin(uint16_t pin, pin_mode_t mode)
{
    SetPin(pin,mode);
}

MyPin::MyPin()
{ 
}

void MyPin::SetPin(uint16_t pin, pin_mode_t mode)
{
    this-> pin = pin;
    this-> mode = mode;
    state = false;

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

void MyPin::Change()
{
    Change(!state);
}

void MyPin::Change(bool isOn)
{
    digitalWrite(pin, isOn);
    state = isOn;
}

void MyPin::AnalogWrite(uint8_t val)
{
    analogWrite(pin,val);
}

void MyPin::StartBlink(uint32_t changeTimeMls)
{
    if(!blinkFlag)
    {
        blinkFlag = true;
        startMillisBlink = millis();
    }
    this->changeTimeMls = changeTimeMls;
}

void MyPin::StopBlink(bool state)
{
    if(blinkFlag)
    {
        blinkFlag = false;
        digitalWrite(pin, state);
    }
}

void MyPin::BlinkHandler()
{
    if(blinkFlag)
    {
        uint32_t mls = millis();
        if(mls - startMillisBlink >= changeTimeMls)
        {
            Change();
            startMillisBlink = mls;
        } 
    }
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
        if(!previousReaderBtn)
            startMillisBtn = mls;
        
        if(mls - startMillisBtn > shortTime)
        {
            buttonState = 1;

            if(longTime > shortTime)
            {
                if(mls - startMillisBtn > longTime)
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

    previousReaderBtn = reader;

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
        if(!previousReaderSns)
            startMillisSns = mls;

        result = mls - startMillisSns;
    }
    previousReaderSns = reader;
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
