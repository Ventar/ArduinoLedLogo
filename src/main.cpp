#include <Arduino.h>

#include <LEDStrip.h>
#include <LEDWebServer.h>
#include <LogoConfig.h>
#include <LogoStorage.h>
#include <WiFiConnect.h>

LEDStrip strip = LEDStrip();
LogoStorage storage = LogoStorage(&strip);
LEDWebServer ledWebServer = LEDWebServer(&strip, &storage);
#ifdef WIFI_ENABLE
WiFiConnect wifiConnection(WIFI_NAME, WIFI_PASSWORD);
#endif

uint16_t bStateA;
uint16_t bStateB;
uint16_t bStateC;
uint16_t bStateD;

void setup() {
  Serial.begin(115200);
  debugln("\n\nLogo started, execute setup...");

  strip.setup();
  storage.setup();

  pinMode(BUTTON_A, INPUT);
  pinMode(BUTTON_B, INPUT);
  pinMode(BUTTON_C, INPUT);
  pinMode(BUTTON_D, INPUT);

#ifdef WIFI_ENABLE
  strip.setMode(STATIC, "550000");
  wifiConnection.connect();
  ledWebServer.setup();
  strip.setMode(STATIC, "005500");
  delay(3000);
#endif

  strip.setMode(OFF);
  debugln("Setup finished...");
}

void loop() {
  strip.loop();
  ledWebServer.loop();

  bStateA = digitalRead(BUTTON_A);
  bStateB = digitalRead(BUTTON_B);
  bStateC = digitalRead(BUTTON_C);
  bStateD = digitalRead(BUTTON_D);

  if (bStateA == HIGH) {
    Serial.println("Button A pressed...");
    strip.setMode(FIRE);
    delay(1000);
  }

  if (bStateB == HIGH) {
    Serial.println("Button B pressed...");
    strip.setMode(RAINBOW);
    delay(1000);
  }

  if (bStateC == HIGH) {
    Serial.println("Button C pressed...");
    strip.setMode(STATIC, "005500");

    delay(1000);
  }

  if (bStateD == HIGH) {
    Serial.println("Button D pressed...");
    strip.setMode(OFF);
    delay(1000);
  }
}