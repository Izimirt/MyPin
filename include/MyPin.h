#pragma once

#include <Arduino.h>
#include "Const.h"

class MyPin
{
    public:
    
        MyPin(uint16_t pin, pin_mode_t mode);

        MyPin();

        void SetPin(uint16_t pin, pin_mode_t mode);

        void Change();

        void Change(bool isOn);

        void AnalogWrite(uint8_t val);

        void StartBlink(uint32_t changeTimeMls);

        void StopBlink(bool state);

        void BlinkHandler();

        bool Read();

        int16_t AnalogRead();

        uint8_t AntiRattleButton(bool signal, uint32_t shortTime = 50, uint32_t longTime = 0);

        uint32_t AntiRattleSensor(bool signal);

        uint8_t GetPin();

        bool GetState();

    private:

        uint16_t pin;
        pin_mode_t mode;
        bool state;

        bool blinkFlag = false;
        uint32_t startMillisBlink;
        uint32_t changeTimeMls;

        bool previousReaderBtn;
        bool ready = false;
        uint8_t buttonState = 0;
        uint32_t startMillisBtn;

        bool previousReaderSns;
        uint32_t startMillisSns;
};