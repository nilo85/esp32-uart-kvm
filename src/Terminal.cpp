#include "Terminal.h"

void Terminal::begin(HardwareSerial &serial) {
    serial.begin(baud, config, rxPin, txPin, invert);
}

void Terminal::end(HardwareSerial &serial) {
    serial.end(true);
}