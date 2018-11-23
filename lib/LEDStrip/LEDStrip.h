
#ifndef LEDStrip_h
#define LEDStrip_h

#include <Adafruit_NeoPixel.h>
#include <Animation.h>
#include <LogoConfig.h>
#include <vector>

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
  void setMode(String modeName);

  /**
   * Set the mode of the LED strip. The mode defines if the strip is off,
   * displays single colors or special effects.
   */
  void setMode(String modeName, String colors);

  /**
   * Set the mode of the LED strip. The mode defines if the strip is off,
   * displays single colors or special effects.
   */
  void setMode(String modeName, String colors, String speed);

  /**
   * Returns the animation object of the currently active animation.
   * */
  Animation* getAnimation();

  /**
   * Returns an array of animations.
   * */
  std::vector<Animation*> getAnimations();

 private:
  /**
   * Reference to the NeoPixel strip.
   */
  Adafruit_NeoPixel strip =
      Adafruit_NeoPixel(NUMBER_OF_PIXELS, NEOPIXEL_PIN, NEO_RGB + NEO_KHZ800);
  /**
   * The current mode of the animation.
   */
  String modeName = "Off";
  /**
   * A vector with the available animations.
   */
  std::vector<Animation*> animations;
};

#endif