
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
  LEDStrip(LogoDynamicConfig* config);

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

  /**
   * Returns the number of pixels.
   */
  uint16_t numPixels();

 private:
  /**
   * Configuration class.
   */
  LogoDynamicConfig* config;
  /**
   * Reference to the NeoPixel strip.
   */
  Adafruit_NeoPixel* strip;
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