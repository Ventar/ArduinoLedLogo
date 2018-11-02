#ifndef LogoButton_h
#define LogoButton_h

#include <LEDStrip.h>
#include <LogoStorage.h>

class LogoButton {
 public:
  /**
   * Constructor.
   */
  LogoButton(String name, uint8_t pin, LEDStrip* strip, LogoStorage* storage) {
    this->name = name;
    this->pin = pin;
    this->strip = strip;
    this->storage = storage;
  };

  /**
   * Performs the needed setup for the Button.
   */
  void setup();

  /**
   * Execute the logic during an Arduino loop cycle.
   * */
  void loop();

  /**
   * Stores the current scene of the device in the memory (SPIFFS).
   */
  void storeScene();

  /**
   * Loads the current scene of the device in the memory (SPIFFS).
   */
  void loadScene();

 private:
  /**
   * Access tot he SPIFFS file system.
   */
  LogoStorage* storage;

  /**
   * The LED Strip to manage LEDs.
   */
  LEDStrip* strip;

  /**
   * The name of the button to use for debug output and storing the assigned
   * data.
   */
  String name;

  /**
   * The PIN used for the button.
   */
  uint8_t pin;
};

#endif
