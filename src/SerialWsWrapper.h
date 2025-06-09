#ifndef _SERIAL_WS_WRAPPER_H
#define _SERIAL_WS_WRAPPER_H

#include <HardwareSerial.h>
#include <AsyncWebSocket.h>

class SerialWsWrapper
{
private:
    HardwareSerial &serial;
    AsyncWebSocketMessageHandler handler;

public:
    AsyncWebSocket ws;

    SerialWsWrapper(HardwareSerial &serial, String path) : serial(serial), handler(), ws(path, handler.eventHandler()) {}
    void setup();
    void loop();
};

#endif
