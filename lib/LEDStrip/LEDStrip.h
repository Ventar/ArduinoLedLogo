
#ifndef LEDStrip_h
#define LEDStrip_h

#include <Adafruit_NeoPixel.h>
#include <AnimationFadeInOut.h>
#include <AnimationFire.h>
#include <AnimationMeteorRain.h>
#include <AnimationOff.h>
#include <AnimationRainbow.h>
#include <AnimationSparkle.h>
#include <AnimationStatic.h>
#include <AnimationTheaterChase.h>
#include <LogoConfig.h>

class LEDStrip {
 public:
  /**
   * Constructor.
   */
  LEDStrip();

  /**
   * Perform the setup operation of this class inside of the Arduino setup
   * method.
   */
  void setup();

  /**
   * Execute the logic during an Arduino loop cycle.
   * */
  void loop();

  /**
   * Set the mode of the LED strip. The mode defines if the strip is off,
   * displays single colors or special effects.
   */
  void setMode(LEDAnimation mode);

  /**
   * Set the mode of the LED strip. The mode defines if the strip is off,
   * displays single colors or special effects.
   */
  void setMode(LEDAnimation mode, String colors);

  /**
   * Set the mode of the LED strip. The mode defines if the strip is off,
   * displays single colors or special effects.
   */
  void setMode(LEDAnimation mode, String colors, uint16_t delay);

  /**
   * Returns the mode in which the LED strip is currently running.
   */
  LEDAnimation getMode();

  /**
   * Returns the mode name in which the LED strip is currently running.
   */
  String getModeName();

  /**
   * Returns the animation object of the currently active animation.
   * */
  Animation* getAnimation();

  /**
   * Returns an array of animations.
   * */
  Animation** getAnimations();

  /**
   * Returns the number of supported Animations.
   * */
  uint8_t getAnimationsCount();

 private:
  /**
   * Reference to the NeoPixel strip.
   */
  Adafruit_NeoPixel strip =
      Adafruit_NeoPixel(NUMBER_OF_PIXELS, NEOPIXEL_PIN, NEO_RGB + NEO_KHZ800);
  /**
   * The current mode of the animation.
   */
  LEDAnimation mode = OFF;
  /**
   * An array with the available animations.
   */
  Animation** animations;
};

#endif