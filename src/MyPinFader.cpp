#include "MyPin.h"

MyPin* MyPin::faderCurrentPtr = nullptr;

void MyPin::SetFader(fader_t** ptrSet, uint16_t fadeTime_ms, uint8_t onBright, uint8_t offBright)
{
    if (*ptrSet == nullptr)
        *ptrSet = new fader_t {};

    (*ptrSet)->onBright = onBright;
    (*ptrSet)->offBright = offBright;
    (*ptrSet)->fadeTime_ms = fadeTime_ms;
    if (onBright == offBright)
        (*ptrSet)->period_ms = 0;
    else
        (*ptrSet)->period_ms = fadeTime_ms / abs(onBright - offBright);
}

void MyPin::StartFader(fader_t* set)
{
    if (set != nullptr)
        modePtr = set;
}

void MyPin::StartFaderAll(fader_t* set)
{
    MyPin* ptr = currentPtr;
    
    while(ptr != nullptr)
    {
        ptr->StartFader(set);
        ptr = ptr->ptrOnOther;
    }
}

void MyPin::StopFader()
{
    modePtr = nullptr;
}

void MyPin::EnableFader()
{
    faderPtrOnOther = faderCurrentPtr;
    faderCurrentPtr = this;
}

void MyPin::EnableFaderAll()
{
    MyPin* ptr = currentPtr;

    while(ptr != nullptr)
    {
        ptr->EnableFader();
        ptr = ptr->ptrOnOther;
    }
}

void MyPin::FaderHandler()
{
    uint32_t ms = millis();

    MyPin* ptr = faderCurrentPtr;

    while(ptr != nullptr)
    {
        if (ptr->mode == OUT)
            ptr->InternalFaderHandler(ms);
        ptr = ptr->faderPtrOnOther;
    }
}

void MyPin::InternalFaderHandler(uint32_t currentMs)
{
    fader_t* ptr = nullptr;

    if (singlePtr)
        ptr = singlePtr;
    else if (modePtr)
        ptr = modePtr;
    else 
        return;

    if (ptr != prevPtr)
    {
        faderBright = (state ? ptr->onBright : ptr->offBright);
        AnalogWrite(faderBright);
    }
    prevPtr = ptr;
    
    if (faderBright != (needState ? ptr->onBright : ptr->offBright))
    {
        if (currentMs < faderPreviousMs)
            faderPreviousMs = currentMs;
        if (currentMs - faderPreviousMs >= ptr->period_ms)
        {
            if (ptr->period_ms == 0)
                faderBright = (needState ? ptr->onBright : ptr->offBright);
            else
                needState ? faderBright++ : faderBright--;
            AnalogWrite(faderBright);
            faderPreviousMs = currentMs;
        }
    }
    else
    {
        singlePtr = nullptr;
        firstFaderBrightSet = false;
        state = needState;
    }
}