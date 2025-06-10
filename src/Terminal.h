#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include <stdint.h>
#include <HardwareSerial.h>

#define NUM_TERMINALS 5

#define BAUD 1500000

#define TX_0 16
#define RX_0 17
#define TX_1 18
#define RX_1 21
#define TX_2 33
#define RX_2 34
#define TX_3 35
#define RX_3 36
#define TX_4 37
#define RX_4 38

#define TERMINAL_0_CONFIG BAUD, SERIAL_8N1, 17, 16, false
#define TERMINAL_1_CONFIG BAUD, SERIAL_8N1, 21, 18, false
#define TERMINAL_2_CONFIG BAUD, SERIAL_8N1, 34, 33, false
#define TERMINAL_3_CONFIG BAUD, SERIAL_8N1, 36, 35 , false
#define TERMINAL_4_CONFIG BAUD, SERIAL_8N1, 38, 37, false
#define TERMINAL_5_CONFIG BAUD, SERIAL_8N1, 40, 39, false
#define TERMINAL_6_CONFIG BAUD, SERIAL_8N1, 3, 2, false
#define TERMINAL_7_CONFIG BAUD, SERIAL_8N1, 5, 4, false
#define TERMINAL_8_CONFIG BAUD, SERIAL_8N1, 7, 6, false
#define TERMINAL_9_CONFIG BAUD, SERIAL_8N1, 9, 8, false
#define TERMINAL_10_CONFIG BAUD, SERIAL_8N1, 11, 10, false
#define TERMINAL_11_CONFIG BAUD, SERIAL_8N1, 12, 13, false


class Terminal
{
private:
    unsigned long baud;
    uint32_t config;
    int8_t rxPin;
    int8_t txPin;
    bool invert;

public:
    Terminal(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
        : baud(baud), config(config), rxPin(rxPin), txPin(txPin), invert(invert) {};
    void setup() {};
    void loop() {};
    void begin(HardwareSerial &serial);
    void end(HardwareSerial &serial);
};



 
static Terminal* terminals[] = {
#if NUM_TERMINALS >= 1
  new Terminal(TERMINAL_0_CONFIG),
#endif
#if NUM_TERMINALS >= 2
  new Terminal(TERMINAL_1_CONFIG),
#endif
#if NUM_TERMINALS >= 3
  new Terminal(TERMINAL_2_CONFIG),
#endif
#if NUM_TERMINALS >= 4
  new Terminal(TERMINAL_3_CONFIG),
#endif
#if NUM_TERMINALS >= 5
  new Terminal(TERMINAL_4_CONFIG),
#endif
#if NUM_TERMINALS >= 6
  new Terminal(TERMINAL_5_CONFIG),
#endif
#if NUM_TERMINALS >= 7
  new Terminal(TERMINAL_6_CONFIG),
#endif
#if NUM_TERMINALS >= 8
  new Terminal(TERMINAL_7_CONFIG),
#endif
#if NUM_TERMINALS >= 9
  new Terminal(TERMINAL_8_CONFIG),
#endif
#if NUM_TERMINALS >= 10
  new Terminal(TERMINAL_9_CONFIG),
#endif
#if NUM_TERMINALS >= 11
  new Terminal(TERMINAL_10_CONFIG),
#endif
#if NUM_TERMINALS >= 12
  new Terminal(TERMINAL_11_CONFIG),
#endif
};

#endif