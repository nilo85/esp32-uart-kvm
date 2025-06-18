#ifndef _SERIAL_WS_WRAPPER_H
#define _SERIAL_WS_WRAPPER_H

#include <HardwareSerial.h>
#include <AsyncWebSocket.h>

#include "Terminal.h"

#define NUM_SERIAL_WRAPPERS SOC_UART_NUM

class SerialWsWrapper
{
private:
    AsyncWebSocketMessageHandler handler;
    HardwareSerial &serial;
    Terminal* _terminal;

    void detach();
public:
    AsyncWebSocket ws;

    SerialWsWrapper(HardwareSerial &serial, String path) : serial(serial), handler(), ws(path, handler.eventHandler()) {}
    void clear();
    void setup();
    void loop();
    void attach(Terminal* wrapper);
};

static SerialWsWrapper* serialWrappers[] = {
    #if NUM_SERIAL_WRAPPERS >= 1
        new SerialWsWrapper(Serial0, "/ws_0"),
    #endif
    #if NUM_SERIAL_WRAPPERS >= 2
        new SerialWsWrapper(Serial1, "/ws_1"),
    #endif
    #if NUM_SERIAL_WRAPPERS >= 3
        new SerialWsWrapper(Serial2, "/ws_2"),
    #endif
};


#endif
