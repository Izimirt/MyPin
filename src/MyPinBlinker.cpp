#include "MyPin.h"

MyPin* MyPin::blinkCurrentPtr = nullptr;

bool MyPin::blinkEnableAll = false;

void MyPin::StartBlink(uint32_t changeTime_ms)
{
    if(!blinkFlag)
    {
        blinkFlag = true;
        blinkStartMs = millis();
    }
    this->changeTime_ms = changeTime_ms;
}

void MyPin::StopBlink(bool state)
{
    if(blinkFlag)
    {
        blinkFlag = false;
        digitalWrite(pin, state);
    }
}

void MyPin::EnableBlink()
{
    blinkPtrOnOther = blinkCurrentPtr;
    blinkCurrentPtr = this;
}

void MyPin::EnableBlinkAll()
{
    blinkEnableAll = true;
}

void MyPin::BlinkHandler()
{
    uint32_t ms = millis();

    if(blinkEnableAll)
    {
        MyPin* ptr = currentPtr;
    
        while(ptr != nullptr)
        {
            ptr->InternalBlinkHandler(ms);
            ptr = ptr->ptrOnOther;
        }
    }
    else
    {
        MyPin* ptr = blinkCurrentPtr;
    
        while(ptr != nullptr)
        {
            ptr->InternalBlinkHandler(ms);
            ptr = ptr->blinkPtrOnOther;
        }
    }
}

void MyPin::InternalBlinkHandler(uint32_t currentMs)
{
    if(blinkFlag)
    {
        if(currentMs - blinkStartMs >= changeTime_ms)
        {
            Change();
            blinkStartMs = currentMs;
        } 
    }
}