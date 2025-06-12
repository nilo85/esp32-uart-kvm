#include "SerialWsWrapper.h"

const AsyncWebSocketSharedBuffer clearScreenSequence = std::make_shared<std::vector<uint8_t>>(std::vector<uint8_t>{
    0x1b, 'c'
  });

void SerialWsWrapper::setup() {
   const  HardwareSerial &hwSerial = serial;
  serial.setRxBufferSize(4096);
  

  handler.onConnect([this](AsyncWebSocket *server, AsyncWebSocketClient *client) {
    //Serial.printf("Client %" PRIu32 " connected\n", client->id());
  });

  handler.onDisconnect([](AsyncWebSocket *server, uint32_t clientId) {
    //Serial.printf("Client %" PRIu32 " disconnected\n", clientId);
  });

  handler.onError([](AsyncWebSocket *server, AsyncWebSocketClient *client, uint16_t errorCode, const char *reason, size_t len) {
    //Serial.printf("Client %" PRIu32 " error: %" PRIu16 ": %s\n", client->id(), errorCode, reason);
  });

  handler.onMessage([this](AsyncWebSocket *server, AsyncWebSocketClient *client, const uint8_t *data, size_t len) {
    serial.write(data, len);
    serial.flush();
    //Serial.printf("Recieved %s (%d)", data, len);
  });

  handler.onFragment([](AsyncWebSocket *server, AsyncWebSocketClient *client, const AwsFrameInfo *frameInfo, const uint8_t *data, size_t len) {
    //Serial.printf("Client %" PRIu32 " fragment %" PRIu32 ": %s\n", client->id(), frameInfo->num, (const char *)data);
  });

}


static uint32_t lastWS = 0;
static uint32_t deltaWS = 2000;


void SerialWsWrapper::loop() {
  uint32_t now = millis();

  if (now - lastWS >= deltaWS) {
    ws.cleanupClients();
    lastWS = millis();
  }
}

void SerialWsWrapper::clear() {
  ws.binaryAll(clearScreenSequence);
}

void SerialWsWrapper::attach(Terminal* terminal) {
  if (_terminal != NULL) {
    _terminal->end(serial);
  }
  clear();
  serial.onReceive([this]() {
      uint8_t buff[1024];
      while(serial.available()) {
        size_t read = serial.readBytes(buff, sizeof(buff));
        ws.binaryAll(buff, read);
      }
  }, false);
  serial.setTimeout(2);
  _terminal = terminal;
  _terminal->begin(serial);
}