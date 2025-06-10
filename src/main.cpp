#include <Arduino.h>
#include <AsyncTCP.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoOTA.h>

#include "SerialWsWrapper.h"
#include "Terminal.h"

#include "../secrets.h"

#include "index.h"

#define HOSTNAME "uartkwm0"

static AsyncWebServer server(80);




void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(HOSTNAME);
  WiFi.begin(KVM_WIFI_SSID, KVM_WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}


static const char *htmlContent PROGMEM = R"(
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1.0" />
		<title>ESP32-UART-KVM</title>
		<script type="module" crossorigin src="/assets/index.js"></script>
		<link rel="stylesheet" crossorigin href="/assets/index.css">
	</head>
	<body>
		<div id="app" data-num-terminals="%NUM_TERMINALS%" data-host="%HOST%"></div>
	</body>
</html>
)";
static const size_t htmlContentLength = strlen_P(htmlContent);

void setup_server() {
  for (int i = 0; i < NUM_SERIAL_WRAPPERS; i++) {
    server.addHandler(&serialWrappers[i]->ws);
  }

  server.on("/set", HTTP_POST, [](AsyncWebServerRequest *request) {
 
    int channel = -1;
    int terminal = -1;

    for(int i = 0; i < request->params(); i++) {
      const AsyncWebParameter *param = request->getParam(i);
    
      if (param->name().equals("channel")) {
        channel = param->value().toInt();
      }
      if (param->name().equals("terminal")) {
        terminal = param->value().toInt();
      }
    }

    if (channel < 0 || channel >= NUM_SERIAL_WRAPPERS) {
      request->send(400, "text/plain", "channel must be betwen 0 and " + (NUM_SERIAL_WRAPPERS-1));
      return;
    }
    if (terminal < 0 || terminal >= NUM_TERMINALS) {
      request->send(400, "text/plain", "terminal must be betwen 0 and " + (NUM_TERMINALS-1));
      return;
    }
    serialWrappers[channel]->attach(terminals[terminal]);
    request->send(200, "text/plain", "done");

  });

  server.on("/assets/index.js", [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse* resp = request->beginResponse(200, "text/javascript", index_js, sizeof(index_js));  
    resp->addHeader("Cache-Control", "no-cache");
    request->send(resp);
  });


  server.on("/assets/index.css", [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse* resp = request->beginResponse(200, "text/css", index_css, sizeof(index_css));  
    resp->addHeader("Cache-Control", "no-cache");
    request->send(resp);
  });

  server.onNotFound([](AsyncWebServerRequest *request) {
    AsyncWebServerResponse* resp = request->beginResponse(200, "text/html", htmlContent, [request](const String &var) -> String {
      if (var == "HOST") {
        return request->host();
      }
      if (var == "NUM_TERMINALS") {
        return String(NUM_TERMINALS).c_str();
      }
      return emptyString;
    });  
    resp->addHeader("Cache-Control", "no-cache");
    request->send(resp);
  });


  server.begin();
}

void setup() {
  setup_wifi();
  ArduinoOTA.setPassword(KVM_OTA_PASSWORD);
  ArduinoOTA.begin();

  setup_server();
  for (int i = 0; i < NUM_TERMINALS; i++) {
    terminals[i]->setup();
  }
  for (int i = 0; i < NUM_SERIAL_WRAPPERS; i++) {
    serialWrappers[i]->setup();
  }

  for (int i = 0; i < min(NUM_SERIAL_WRAPPERS, NUM_TERMINALS); i++) {
    serialWrappers[i]->attach(terminals[i]);
  }

}


static uint32_t lastWS = 0;
static uint32_t deltaWS = 2000;


void loop() {
  ArduinoOTA.handle();
  for (int i = 0; i < NUM_SERIAL_WRAPPERS; i++) {
    serialWrappers[i]->loop();
  }
}
