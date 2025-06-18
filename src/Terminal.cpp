#include "Terminal.h"
#include <Arduino.h>
void Terminal::begin(HardwareSerial &serial) {
    serial.begin(baud, config, rxPin, txPin, invert);
}

void Terminal::end(HardwareSerial &serial) {
    serial.end(true);
    // fload with high resistanse to gnd, if left high, Raspberry Pi etc won't successfully boot
    pinMode(txPin, INPUT_PULLDOWN);
    pinMode(rxPin, INPUT_PULLDOWN);
}