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

        void StartBlink(uint32_t changeTime_ms);

        void StopBlink(bool state);

        static void BlinkHandler();

        bool Read();

        int16_t AnalogRead();

        uint8_t AntiRattleButton(bool signal, uint32_t shortTime = 50, uint32_t longTime = 0);

        uint32_t AntiRattleSensor(bool signal);

        uint8_t GetPin();

        bool GetState();

    private:

        void InternalBlinkHandler(uint32_t currentMs);

        uint16_t pin;
        pin_mode_t mode;
        bool state;

        bool blinkFlag = false;
        uint32_t blinkStartMs;
        uint32_t changeTime_ms;

        bool btnPreviousReader;
        bool ready = false;
        uint8_t buttonState = 0;
        uint32_t btnStartMs;

        bool snsPreviousReader;
        uint32_t snsStartMs;

        MyPin* ptrOnOther = nullptr;
        static MyPin* currentPtr;
};