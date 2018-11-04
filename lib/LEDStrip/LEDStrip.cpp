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

  strip.begin();
  setMode(OFF);
}

uint8_t LEDStrip::getAnimationsCount() { return 8; }

void LEDStrip::setMode(LEDAnimation mode, String colors, String speed) {
  for (int i = 0; i < getAnimationsCount(); i++) {
    if (animations[i]->getSceneData()->mode == mode) {
      this->mode = mode;

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

void LEDStrip::setMode(LEDAnimation mode, String colors) {
  setMode(mode, colors, "");
}

void LEDStrip::setMode(LEDAnimation mode) { setMode(mode, "", ""); }

void LEDStrip::loop() {
  for (int i = 0; i < getAnimationsCount(); i++) {
    if (animations[i]->getSceneData()->mode == mode) {
      animations[i]->update();
    }
  }
}

Animation* LEDStrip::getAnimation() {
  for (int i = 0; i < getAnimationsCount(); i++) {
    if (animations[i]->getSceneData()->mode == mode) {
      return animations[i];
    }
  }
}

Animation** LEDStrip::getAnimations() { return animations; }
