#include "MyPin.h"

MyPin* MyPin::blinkCurrentPtr = nullptr;

void MyPin::StartBlink(uint32_t changeTime_ms)
{
    if (!blinkFlag)
    {
        blinkFlag = true;
        blinkStartMs = millis();
    }
    this->changeTime_ms = changeTime_ms;
}

void MyPin::StopBlink(bool state)
{
    if (blinkFlag)
    {
        blinkFlag = false;
        Change(state);
    }
}

void MyPin::EnableBlink()
{
    blinkPtrOnOther = blinkCurrentPtr;
    blinkCurrentPtr = this;
}

void MyPin::EnableBlinkAll()
{
    MyPin* ptr = currentPtr;

    while (ptr != nullptr)
    {
        ptr->EnableBlink();
        ptr = ptr->ptrOnOther;
    }
}

void MyPin::BlinkHandler()
{
    uint32_t ms = millis();

    MyPin* ptr = blinkCurrentPtr;

    while (ptr != nullptr)
    {
        if (ptr->mode == OUT)
            ptr->InternalBlinkHandler(ms);
        ptr = ptr->blinkPtrOnOther;
    }   
}

void MyPin::InternalBlinkHandler(uint32_t currentMs)
{
    if (blinkFlag)
    {
        if (currentMs - blinkStartMs >= changeTime_ms)
        {
            Change();
            blinkStartMs = currentMs;
        } 
    }
}