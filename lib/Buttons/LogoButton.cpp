#include <Arduino.h>
#include <LogoButton.h>

void LogoButton::setup() {
  debug("LogoButton::setup - Setup Button %s ...", this->name.c_str());
  pinMode(pin, INPUT);
  sceneName = storage->loadSceneForButton(this->name);
};

void LogoButton::loop() {
  uint16_t bState = digitalRead(pin);

  if (bState == HIGH) {
    debug("LogoButton::loop - %s pressed, assigned scene ::= [%s] ...",
          name.c_str(), sceneName.c_str());
    if (sceneName == "Off") {
      strip->setMode("Off");
    } else if (storage->exists("/scene_" + sceneName)) {
      storage->loadScene(sceneName);
    } else {
      debug("LogoButton::loop - A scene with name ::= [%s] does not exist",
            sceneName.c_str());
    }

    delay(500);
  }
};

void LogoButton::assignScene(String sceneName) {
  if (sceneName == "Off") {
    this->sceneName = sceneName;
  } else if (storage->exists("/scene_" + sceneName)) {
    this->sceneName = sceneName;
    storage->storeSceneForButton(this->name, sceneName);
  } else {
    debug("LogoButton::assignScene - A scene with name ::= [%s] does not exist",
          sceneName.c_str());
  }
};

void LogoButton::setScene(String sceneName) {
  if (sceneName == "Off") {
    this->sceneName = sceneName;
  } else if (storage->exists("/scene_" + sceneName)) {
    this->sceneName = sceneName;
  } else {
    debug("LogoButton::setScene - A scene with name ::= [%s] does not exist",
          sceneName.c_str());
  }
};

String LogoButton::getName() { return name; };

String LogoButton::getSceneName() { return sceneName; };
