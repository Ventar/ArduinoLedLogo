#include <LEDStrip.h>

static const char* LEDAnimationNames[] = {
    "OFF",  "RAINBOW",     "FADE_IN_OUT",   "STATIC",
    "FIRE", "METEOR_RAIN", "THEATER_CHASE", "SPARKLE"};

LEDStrip::LEDStrip() {}

String LEDStrip::getModeName() { return LEDAnimationNames[mode]; }

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

LEDAnimation LEDStrip::getMode() { return mode; }

void LEDStrip::setMode(LEDAnimation mode, String colors, uint16_t delay) {
  for (int i = 0; i < getAnimationsCount(); i++) {
    if (animations[i]->getMode() == mode) {
      this->mode = mode;
      debug("LEDStrip: Set mode to ::=[%s], delay ::= [%d], colors ::= [%s]",
            getModeName().c_str(), delay, colors.c_str());

      if (colors != "null") {
        animations[i]->setColorListFromString(colors);
      }

      if (delay != 0) {
        animations[i]->setDelay(delay);
      }
      animations[i]->reset();
    }
  }
}

void LEDStrip::setMode(LEDAnimation mode, String colors) {
  setMode(mode, colors, 0);
}

void LEDStrip::setMode(LEDAnimation mode) { setMode(mode, "null", 0); }

void LEDStrip::loop() {
  for (int i = 0; i < getAnimationsCount(); i++) {
    if (animations[i]->getMode() == mode) {
      animations[i]->update();
    }
  }
}

Animation* LEDStrip::getAnimation() {
  for (int i = 0; i < getAnimationsCount(); i++) {
    if (animations[i]->getMode() == mode) {
      return animations[i];
    }
  }
}

Animation** LEDStrip::getAnimations() { return animations; }
