#include <Arduino.h>

#include <LEDStrip.h>
#include <LEDWebServer.h>
#include <LogoButton.h>
#include <LogoConfig.h>
#include <LogoStorage.h>
#include <WiFiConnect.h>

LogoDynamicConfig config = LogoDynamicConfig();
WiFiConnect wifiConnection(&config);
LEDStrip strip = LEDStrip(&config);
LogoStorage storage = LogoStorage(&strip);

LogoButton* buttons[NUMBER_OF_BUTTONS];
LEDWebServer ledWebServer = LEDWebServer(&config, &strip, &storage, buttons);

void setup() {
  Serial.begin(115200);
  debugln("\n\nMain: Logo started, execute setup...");
  debugln("Main: -------------------------------------------------------");

  strip.setup();
  storage.setup();

  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    buttons[i] =
        new LogoButton(String(char(65 + i)), BUTTON_PINS[i], &strip, &storage);
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

  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    buttons[i]->loop();
  }
}