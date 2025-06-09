#include <Arduino.h>
#include <AsyncTCP.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoOTA.h>

#include "SerialWsWrapper.h"

#include "../secrets.h"

#define HOSTNAME "uartkwm0"

static AsyncWebServer server(80);

static SerialWsWrapper wrapper0(Serial0, "/ws_0");
static SerialWsWrapper wrapper1(Serial1, "/ws_1");
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

#define SERIAL_0_CONFIG 1500000, SERIAL_8N1, RX_0, TX_0
#define SERIAL_1_CONFIG 1500000, SERIAL_8N1, RX_1, TX_1
#define SERIAL_2_CONFIG 1500000, SERIAL_8N1, RX_2, TX_2
#define SERIAL_3_CONFIG 1500000, SERIAL_8N1, RX_3, TX_3
#define SERIAL_4_CONFIG 1500000, SERIAL_8N1, RX_4, TX_4

void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(HOSTNAME);
  WiFi.begin(KVM_WIFI_SSID, KVM_WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}

void setup_server() {

  server.addHandler(&wrapper0.ws);
  server.addHandler(&wrapper1.ws);

  server.begin();
}

void setup() {
  stdout;
  setup_wifi();
  ArduinoOTA.setPassword(KVM_OTA_PASSWORD);
  ArduinoOTA.begin();

  setup_server();
  wrapper0.setup();
  wrapper1.setup();
  Serial0.begin(SERIAL_0_CONFIG);
  Serial1.begin(SERIAL_1_CONFIG);

}


static uint32_t lastWS = 0;
static uint32_t deltaWS = 2000;


void loop() {
  ArduinoOTA.handle();
  wrapper0.loop();
  wrapper1.loop();
}
