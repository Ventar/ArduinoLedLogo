#include <AnimationCircle.h>
#include <AnimationCloud.h>
#include <AnimationDoubleCircle.h>
#include <AnimationFadeInOut.h>
#include <AnimationFire.h>
#include <AnimationMeteorRain.h>
#include <AnimationOff.h>
#include <AnimationRainbow.h>
#include <AnimationSparkle.h>
#include <AnimationStatic.h>
#include <AnimationTheaterChase.h>
#include <LEDStrip.h>

LEDStrip::LEDStrip() {}

void LEDStrip::setup() {
  this->animations.reserve(10);
  this->animations.push_back(new AnimationOff(&strip));
  this->animations.push_back(new AnimationStatic(&strip));
  this->animations.push_back(new AnimationFadeInOut(&strip));
  this->animations.push_back(new AnimationRainbow(&strip));
  this->animations.push_back(new AnimationFire(&strip));
  this->animations.push_back(new AnimationMeteorRain(&strip));
  this->animations.push_back(new AnimationTheaterChase(&strip));
  this->animations.push_back(new AnimationSparkle(&strip));
  this->animations.push_back(new AnimationCircle(&strip));
  this->animations.push_back(new AnimationCloud(&strip));
  //  this->animations.push_back(new AnimationDoubleCircle(&strip));

  strip.begin();
  setMode("Off");
}

void LEDStrip::setMode(String modeName, String colors, String speed) {
  for (size_t i = 0; i < this->animations.size(); i++) {
    if (animations.at(i)->getSceneData()->modeName == modeName) {
      this->modeName = modeName;

      if (colors != "") {
        animations.at(i)->setColorListFromString(colors);
      }

      if (speed != "") {
        animations.at(i)->setSpeed(speed.toInt());
      }

      debug(
          "LEDStrip::setMode - mode ::= [%s], delay ::= [%d], speed ::= "
          "[%d], colors ::= [%s]",
          animations.at(i)->getSceneData()->modeName.c_str(),
          animations.at(i)->getSceneData()->delay,
          animations.at(i)->getSceneData()->speed, colors.c_str());

      animations.at(i)->reset();
    }
  }
}

void LEDStrip::setMode(String modeName, String colors) {
  setMode(modeName, colors, "");
}

void LEDStrip::setMode(String modeName) { setMode(modeName, "", ""); }

void LEDStrip::loop() {
  for (size_t i = 0; i < this->animations.size(); i++) {
    if (animations.at(i)->getSceneData()->modeName == modeName) {
      animations.at(i)->update();
    }
  }
}

Animation* LEDStrip::getAnimation() {
  for (size_t i = 0; i < this->animations.size(); i++) {
    if (animations.at(i)->getSceneData()->modeName == modeName) {
      return animations.at(i);
    }
  }
  return NULL;
}

std::vector<Animation*> LEDStrip::getAnimations() { return animations; }
