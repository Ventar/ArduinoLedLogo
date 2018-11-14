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
   Assigns a scene to the button and stores it in the file system (SPIFFS).
   */
  void assignScene(String scene);

  /**
   * Assigns a scene to the button without saving it.
   */
  void setScene(String scene);
  /**
   * Returns the name of button.
   */
  String getName();

  /**
   * Returns the name of the assigned scene for this button.
   */
  String getSceneName();

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
   * The name of scene that is assigned to the button.
   */
  String sceneName = "Off";

  /**
   * The PIN used for the button.
   */
  uint8_t pin;
};

#endif
