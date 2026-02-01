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

    if (mode == OUT)
        Change(false);
    else if (mode == IN)
        pinMode(pin,INPUT);
    else if (mode == IN_PULLUP)
        pinMode(pin,INPUT_PULLUP);

    #ifdef ESP32

    else if (mode == IN_PULLDOWN)
        pinMode(pin,INPUT_PULLDOWN);

    #endif
}

void MyPin::Change()
{
    Change(!needState,nullptr);
}

void MyPin::Change(bool isOn, fader_t* set)
{
    if (mode == OUT)
    {
        if ((isOn != needState) ||                       //  common turn
            ((isOn == state) && (state != needState)))   //  turn back when changing haven't done yet
        {
            needState = isOn;

            if (set != nullptr)
            {
                singlePtr = set;
                if (!firstFaderBrightSet)
                {
                    firstFaderBrightSet = true;
                    faderBright = (state ? set->onBright : set->offBright);
                } 
            }
            else if (modePtr != nullptr)
            {
                if (!firstFaderBrightSet)
                {
                    firstFaderBrightSet = true;
                    faderBright = (state ? modePtr->onBright : modePtr->offBright);
                }
            }
            else
            {
                AnalogWrite(isOn ? 255 : 0);     //  I've done it especially for correct work Fader functions
                state = isOn;
            }
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
    if (!signal)
        reader = !reader;

    if (reader)
    {
        if (ready)
            return 0;

        uint32_t ms = millis();
        if (!btnPreviousReader)
            btnStartMs = ms;
        
        if (ms - btnStartMs >= shortTime)
        {
            btnState = 1;

            if (longTime > shortTime)
            {
                if (ms - btnStartMs >= longTime)
                {
                    btnState = 2;
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

    if (ready || (!reader && btnState))
    {
        uint8_t bS = btnState;
        btnState = 0;
        return bS;
    }

    return 0;
}

uint8_t MyPin::AnalogAntiRattleButton(uint16_t minLvl, uint16_t maxLvl, uint32_t shortTime, uint32_t longTime)
{
    bool goodLvl = false;
    uint16_t lvl = analogRead(pin);
    if ((lvl >= minLvl) && (lvl <= maxLvl))
        goodLvl = true;

    if (goodLvl)
    {
        if (analogReady)
            return 0;

        uint32_t ms = millis();
        if (!btnPreviousGoodLvl)
            btnAnalogStartMs = ms;
        
        if (ms - btnAnalogStartMs >= shortTime)
        {
            btnAnalogState = 1;

            if (longTime > shortTime)
            {
                if (ms - btnAnalogStartMs >= longTime)
                {
                    btnAnalogState = 2;
                    analogReady = true;
                }
            }
            else
                analogReady = true;
        }
    }
    else
        analogReady = false;

    btnPreviousGoodLvl = goodLvl;

    if (analogReady || (!goodLvl && btnAnalogState))
    {
        uint8_t bS = btnAnalogState;
        btnAnalogState = 0;
        return bS;
    }

    return 0;
}

uint32_t MyPin::AntiRattleSensor(bool signal)
{
    bool reader = digitalRead(pin);
    if (!signal)
        reader = !reader;

    uint32_t result = 0;
    if (reader)
    {
        uint32_t ms = millis();
        if (!snsPreviousReader)
            snsStartMs = ms;

        result = ms - snsStartMs;
    }
    snsPreviousReader = reader;
    return result;
}

uint32_t MyPin::AnalogAntiRattleSensor(uint16_t minLvl, uint16_t maxLvl)
{
    bool goodLvl = false;
    uint16_t lvl = analogRead(pin);
    if ((lvl >= minLvl) && (lvl <= maxLvl))
        goodLvl = true;

    uint32_t result = 0;
    if (goodLvl)
    {
        uint32_t ms = millis();
        if (!snsPreviousGoodLvl)
            snsAnalogStartMs = ms;

        result = ms - snsStartMs;
    }
    snsPreviousGoodLvl = goodLvl;
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