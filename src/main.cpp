#include <Arduino.h>

#include <LEDStrip.h>
#include <LEDWebServer.h>
#include <LogoButton.h>
#include <LogoConfig.h>
#include <LogoStorage.h>
#include <WiFiConnect.h>
#include <vector>

#define LED_LOGO_VERSION "v0.0.1"

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
  config.setParameter("VERSION", LED_LOGO_VERSION);
  strip.setup();

  if (config.getParameterAsInt("NUMBER_OF_BUTTONS") > 0) {
    // TODO: free memory
    std::vector<uint8_t> buttonpins =
        config.getParameterAsPinList("BUTTON_PINS");

    for (size_t i = 0; i < buttonpins.size(); i++) {
      buttons.push_back(new LogoButton(String(char(65 + i)), buttonpins[i],
                                       &strip, &storage));
      buttons[i]->setup();
    }
  }

  delay(500);

  String startScene = config.getParameterAsString("START_SCENE");
  if (startScene == "" || startScene == "Off") {
    strip.setMode("Off");
  } else if (startScene == "Last") {
    storage.loadScene(config.getParameterAsString("LAST_SCENE"));
  } else {
    storage.loadScene(startScene);
  }

  wifiConnection.connect();
  ledWebServer.setup();

  Serial.println(
      "Main: -------------------------------------------------------");
  Serial.println(
      "Main: #       ####### ######     #                            ");
  Serial.println(
      "Main: #       #       #     #    #        ####   ####   ####  ");
  Serial.println(
      "Main: #       #       #     #    #       #    # #    # #    # ");
  Serial.println(
      "Main: #       #####   #     #    #       #    # #      #    # ");
  Serial.println(
      "Main: #       #       #     #    #       #    # #  ### #    # ");
  Serial.println(
      "Main: #       #       #     #    #       #    # #    # #    # ");
  Serial.println(
      "Main: ####### ####### ######     #######  ####   ####   ####  ");
  Serial.println(
      "Main: -------------------------------------------------------");
  Serial.println("Main: Version " + config.getParameterAsString("VERSION"));
  Serial.println(
      "Main: -------------------------------------------------------");
  Serial.println("Main: Setup finished...");
}

void loop() {
  strip.loop();
  ledWebServer.loop();

  for (size_t i = 0; i < buttons.size(); i++) {
    buttons[i]->loop();
  }
}