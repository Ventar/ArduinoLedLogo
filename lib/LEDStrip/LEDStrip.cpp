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

LEDStrip::LEDStrip(LogoConfig* config) { this->config = config; }

void LEDStrip::setup() {
  strip = new Adafruit_NeoPixel(config->getParameterAsInt("NEOPIXEL_NUMBER"),
                                config->getParameterAsInt("NEOPIXEL_PIN"),
                                NEO_RGB + NEO_KHZ800);

  this->animations.reserve(10);
  this->animations.push_back(new AnimationOff(config, strip));
  this->animations.push_back(new AnimationStatic(config, strip));
  this->animations.push_back(new AnimationFadeInOut(config, strip));
  this->animations.push_back(new AnimationRainbow(config, strip));
  this->animations.push_back(new AnimationFire(config, strip));
  this->animations.push_back(new AnimationMeteorRain(config, strip));
  this->animations.push_back(new AnimationTheaterChase(config, strip));
  this->animations.push_back(new AnimationSparkle(config, strip));
  this->animations.push_back(new AnimationCircle(config, strip));
  this->animations.push_back(new AnimationCloud(config, strip));
  //  this->animations.push_back(new AnimationDoubleCircle(&strip));

  strip->begin();
  for (int i = 0; i < strip->numPixels(); i++) {
    strip->setPixelColor(i, 0);
  }
  strip->show();
}

uint16_t LEDStrip::numPixels() { return strip->numPixels(); }

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

#ifdef DEBUG
      if (colors.length() >= 21) {
        debug(
            "LEDStrip::setMode - mode ::= [%s], delay ::= [%d], speed ::=[%d], "
            "colors ::= [%s]",
            animations.at(i)->getSceneData()->modeName.c_str(),
            animations.at(i)->getSceneData()->delay,
            animations.at(i)->getSceneData()->speed,
            (colors.substring(0, 21) + String("...")).c_str());
      } else {
        debug(
            "LEDStrip::setMode - mode ::= [%s], delay ::= [%d], speed ::=[%d], "
            "colors ::= [%s]",
            animations.at(i)->getSceneData()->modeName.c_str(),
            animations.at(i)->getSceneData()->delay,
            animations.at(i)->getSceneData()->speed, colors.c_str());
      }
#endif

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
