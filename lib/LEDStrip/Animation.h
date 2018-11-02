#ifndef Animation_h
#define Animation_h

#include <Adafruit_NeoPixel.h>
#include <LogoConfig.h>

/**
 * The available LED animation modes.
 */
enum LEDAnimation {
  OFF,
  RAINBOW,
  FADE_IN_OUT,
  STATIC,
  FIRE,
  METEOR_RAIN,
  THEATER_CHASE,
  SPARKLE
};

/**
 * Comination of data fields that make a scene displayed by the logo.
 */
struct SceneData {
  /**
   * The mode that is represented by this animation.
   */
  LEDAnimation mode;

  /**
   * The delay to wait before the next step of the animation happens.
   */
  uint16_t delay = DEFAULT_DELAY;

  /**
   * The colors managed by the static animation. This class has to remember to
   * colors to overwrite the LEDs in case an effect (other animation) is
   * running.
   */
  uint32_t* colors;
};

/**
 * Base class for all animations. Contains the number of LEDs for the logo and
 * the delay for the the animation updates.
 */
class Animation {
 public:
  /**
   * Constructor.
   */
  Animation(Adafruit_NeoPixel* strip, LEDAnimation mode, String path);

  /**
   * The delay value that is used to decide if the process() method is
   * triggered. Dring the loop of the micro controller the update method of the
   * animation is triggered. Every time the delay is reached the business logic
   * is executed. This is the workarround for missing multi threading in the
   * Arduino framework which is needed to handle buttos and HTTP requests for
   * example.
   */
  void setDelay(int delay);

  /**
   * Update method called for every cycle of the loop method. Based on the delay
   * this method either does nothing or triggers the process method of the
   * extending animations.
   */
  void update();

  /**
   * Virtual method that has to be overriden by the extending animation to
   * update the LEDs.
   */
  virtual void process() = 0;

  /**
   * Resets the animation to the initial values.
   */
  virtual void reset();

  /**
   * Updates the pixel of the strip to show the current color.
   */
  void show();

  /**
   * Sets the LED values to the colors encoded in the String. If the list is
   * shorter then the number of LEDS, all remaining LEDs are set to the last
   * color passed in the list.
   */
  void setColorListFromString(String pixels);

  /**
   * Returns the stored colors as a comma separated String object.
   */
  String getColorListAsString();

  /**
   * Returns the path for the LED webserver that is used to trigger this
   * animation.
   */
  String getPath();

  /**
   * Returns the mode that is represented by this animtaion.
   */
  LEDAnimation getMode();

  /**
   * Returns the current scene data representing this animation.
   */
  SceneData* getSceneData();

 protected:
  uint16_t counter = 0;

  /**
   * Comination of data fields that make a scene displayed by the logo.
   */
  SceneData* data;

  /**
   * The path for the LED webserver that is used to trigger this
   * animation.
   */
  String path;

  /**
   * The last color of the static color list.
   */
  uint32_t lastColor = 0;

  /**
   * Neopixeo strip that is controlled by the extending animations.
   */
  Adafruit_NeoPixel* strip;

  /**
   * Sets a single pixel to the given RGB value.
   */
  void setPixel(int Pixel, byte red, byte green, byte blue);

  /**
   * Sets a single pixel to the given RGB Hex encoded as unsigned integer.
   */
  void setPixel(int Pixel, uint32_t);

  /**
   * Set all pixels of the LED strip to the RGB value.
   */
  void setAll(byte red, byte green, byte blue);

  /**
   * Separates an incoming list of type <hexcolor>,<hexcolor>,...
   */
  uint32_t separateStringList(String data, uint16_t index,
                              uint32_t defaultColor);

 private:
  /**
   * The current tick counter.
   */
  uint16_t tick = 0;
};

#endif