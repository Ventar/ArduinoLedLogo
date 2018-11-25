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
   * Deletes the scene with the given name from file system (SPIFFS).
   */
  void deleteScene(String name);

  /**
   * Loads the current scene of the device in the memory (SPIFFS).
   */
  void loadScene(String name);

  /**
   * Loads the configuration for the given button.
   */
  String loadSceneForButton(String buttonName);

  /**
   * Stores the configuration for the given button.
   */
  void storeSceneForButton(String buttonName, String sceneName);

  /**
   * Load the configuration from the storage.
   */
  void loadConfig();

  /**
   * Save the configuration value to the storage
   */
  void saveConfig(String name, String value);

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
