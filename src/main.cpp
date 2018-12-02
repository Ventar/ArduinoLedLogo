#include <Arduino.h>

#include <LEDStrip.h>
#include <LEDWebServer.h>
#include <LogoButton.h>
#include <LogoConfig.h>
#include <LogoStorage.h>
#include <WiFiConnect.h>
#include <vector>

LogoConfig config = LogoConfig();
WiFiConnect wifiConnection(&config);
LEDStrip strip = LEDStrip(&config);
LogoStorage storage = LogoStorage(&config, &strip);

std::vector<LogoButton*> buttons;
LEDWebServer ledWebServer = LEDWebServer(&config, &strip, &storage, &buttons);

void setup() {
  Serial.begin(115200);
  debugln("\n\nMain: Logo started, execute setup...");
  debugln("Main: -------------------------------------------------------");
  storage.setup();
  storage.loadConfig();
  strip.setup();

  // TODO: free memory
  std::vector<uint8_t> buttonpins = config.getParameterAsPinList("BUTTON_PINS");

  for (size_t i = 0; i < buttonpins.size(); i++) {
    buttons.push_back(
        new LogoButton(String(char(65 + i)), buttonpins[i], &strip, &storage));
    buttons[i]->setup();
  }

  strip.setMode("Static", "550000");
  wifiConnection.connect();
  ledWebServer.setup();
  strip.setMode("Static", "005500");
  delay(3000);

  strip.setMode("Off");
  debugln("Main: Setup finished...");
  debugln("Main: -------------------------------------------------------");
}

void loop() {
  strip.loop();
  ledWebServer.loop();

  for (size_t i = 0; i < buttons.size(); i++) {
    buttons[i]->loop();
  }
}