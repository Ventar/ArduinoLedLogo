#ifndef LogoStorage_h
#define LogoStorage_h

#include <FS.h>
#include <LEDStrip.h>

class LogoStorage {
 public:
  /**
   * Constructor.
   */
  LogoStorage(LEDStrip* strip) { this->strip = strip; };

  /**
   * Stores the current scene of the device in the memory (SPIFFS).
   */
  void storeScene(String name);

  /**
   * Loads the current scene of the device in the memory (SPIFFS).
   */
  void loadScene(String name);

  /**
   * Open a file from the internal storage in read only mode.
   */
  File open(String name);

  /**
   * Checks if the file with the given name exists.
   */
  boolean exists(String name);

  /**
   * Performs the needed setup for the LED web server.
   */
  void setup();

 private:
  LEDStrip* strip;
};

#endif
