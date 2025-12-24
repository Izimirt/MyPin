#pragma once

typedef enum
{
    OUT,
    IN,
    IN_PULLUP,
    #ifdef ESP32
    IN_PULLDOWN
    #endif
} pin_mode_t;