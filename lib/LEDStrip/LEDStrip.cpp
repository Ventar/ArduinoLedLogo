#include <LEDStrip.h>

LEDStrip::LEDStrip() {}

void LEDStrip::setup() {
  this->animations = new Animation*[getAnimationsCount()];
  animations[0] = new AnimationOff(&strip);
  animations[1] = new AnimationStatic(&strip);
  animations[2] = new AnimationFadeInOut(&strip);
  animations[3] = new AnimationRainbow(&strip);
  animations[4] = new AnimationFire(&strip);
  animations[5] = new AnimationMeteorRain(&strip);
  animations[6] = new AnimationTheaterChase(&strip);
  animations[7] = new AnimationSparkle(&strip);
  animations[8] = new AnimationCircle(&strip);

  strip.begin();
  setMode("Off");
}

uint8_t LEDStrip::getAnimationsCount() { return 9; }

void LEDStrip::setMode(String modeName, String colors, String speed) {
  for (int i = 0; i < getAnimationsCount(); i++) {
    if (animations[i]->getSceneData()->modeName == modeName) {
      this->modeName = modeName;

      if (colors != "") {
        animations[i]->setColorListFromString(colors);
      }

      if (speed != "") {
        animations[i]->setSpeed(speed.toInt());
      }

      debug(
          "LEDStrip::setMode - Status mode ::=[%s], delay ::= [%d], speed ::= "
          "[%d], colors ::= [%s]",
          animations[i]->getSceneData()->modeName.c_str(),
          animations[i]->getSceneData()->delay,
          animations[i]->getSceneData()->speed, colors.c_str());

      animations[i]->reset();
    }
  }
}

void LEDStrip::setMode(String modeName, String colors) {
  setMode(modeName, colors, "");
}

void LEDStrip::setMode(String modeName) { setMode(modeName, "", ""); }

void LEDStrip::loop() {
  for (int i = 0; i < getAnimationsCount(); i++) {
    if (animations[i]->getSceneData()->modeName == modeName) {
      animations[i]->update();
    }
  }
}

Animation* LEDStrip::getAnimation() {
  for (int i = 0; i < getAnimationsCount(); i++) {
    if (animations[i]->getSceneData()->modeName == modeName) {
      return animations[i];
    }
  }
}

Animation** LEDStrip::getAnimations() { return animations; }
