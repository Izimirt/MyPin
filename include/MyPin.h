#pragma once

#include <Arduino.h>
#include "Const.h"
#include "Structs.h"

class MyPin
{
    public:
    
        MyPin(uint16_t pin, pin_mode_t mode);

        MyPin();

        void SetPin(uint16_t pin, pin_mode_t mode);

        void Change(fader_t* set = nullptr);

        void Change(bool isOn, fader_t* set = nullptr);

        void AnalogWrite(uint8_t val);
        
        bool Read();

        int16_t AnalogRead();

        uint8_t AntiRattleButton(bool signal, uint32_t shortTime = 50, uint32_t longTime = 0);

        uint8_t AnalogAntiRattleButton(uint16_t minLvl, uint16_t maxLvl, uint32_t shortTime = 50, uint32_t longTime = 0);

        uint32_t AntiRattleSensor(bool signal);

        uint32_t AnalogAntiRattleSensor(uint16_t minLvl, uint16_t maxLvl);

        uint8_t GetPin();

        bool GetState();


        static void BlinkHandler();

        void StartBlink(uint32_t changeTime_ms);

        void StopBlink(bool state);

        void EnableBlink();

        static void EnableBlinkAll();


        static void SetFader(fader_t** ptrSet, uint16_t fadeTime_ms, uint8_t onBright = 255, uint8_t offBright = 0);

        static void FaderHandler();

        void StartFader(fader_t* set);

        static void StartFaderAll(fader_t* set);

        void StopFader();

        void EnableFader();

        static void EnableFaderAll();

    private:

        void InternalBlinkHandler(uint32_t currentMs);

        void InternalFaderHandler(uint32_t currentMs);

        uint16_t pin;               //  Common
        pin_mode_t mode;
        bool state;
        bool needState;

        MyPin* ptrOnOther = nullptr;
        static MyPin* currentPtr;
        

        bool btnPreviousReader;     //  AntiRattleButton
        bool ready = false;
        uint8_t btnState = 0;
        uint32_t btnStartMs;

        bool btnPreviousGoodLvl;     //  AnalogAntiRattleButton
        bool analogReady = false;
        uint8_t btnAnalogState = 0;
        uint32_t btnAnalogStartMs;

        bool snsPreviousReader;     //  AntiRattleSensor
        uint32_t snsStartMs;

        bool snsPreviousGoodLvl;  //  AnalogAntiRattleSensor
        uint32_t snsAnalogStartMs;


        bool blinkFlag = false;     //  Blink
        uint32_t blinkStartMs;
        uint32_t changeTime_ms;

        MyPin* blinkPtrOnOther = nullptr;
        static MyPin* blinkCurrentPtr;


        uint32_t faderPreviousMs;   //  Fader
        uint8_t faderBright;
        bool firstFaderBrightSet = false;
        bool changeModePtr = false;
        fader_t* modePtr = nullptr;
        fader_t* singlePtr = nullptr;

        MyPin* faderPtrOnOther = nullptr;
        static MyPin* faderCurrentPtr;
};