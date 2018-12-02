#ifndef LEDWebServer_h
#define LEDWebServer_h

#include <ESP8266WebServer.h>
#include <LEDStrip.h>
#include <LogoButton.h>
#include <LogoStorage.h>

/**
 * Handler for the ESP8266 web server class. Sets up the web server to handle
 * incoming requests for the control of a logo.
 */
class LEDWebServer {
 public:
  /**
   * Constructor.
   */
  LEDWebServer(LogoConfig* config, LEDStrip* strip, LogoStorage* storage,
               std::vector<LogoButton*>* buttons);

  /**
   * Performs the needed setup for the LED web server.
   */
  void setup();

  /**
   * Called during the loop() call to check if a ne request came in to update
   * the underlying LED strip
   */
  void loop();

  /**
   * Returns the configuration class of the logo.
   */
  LogoConfig* getConfig();

  /**
   * Returns the controlled LEDStrip.
   */
  LEDStrip* getLEDStrip();

  /**
   * Returns the storage.
   */
  LogoStorage* getStorage();

  /**
   * Returns the available buttons
   */
  std::vector<LogoButton*>* getButtons();

 private:
  /**
   * Method to handle an incoming request
   */
  void handleRequest();

  /**
   * Utility method to check if an icoming request wants to update the
   * animations.
   */
  boolean handleAnimation(Animation* animation);

  /**
   * Utility method to check if an icoming request wants to manage scenes for
   * the logo.
   */
  boolean handleScenes();

  /**
   * Utility method to check if an icoming request wants to manage buttons for
   * the logo.
   */
  boolean handleButtons();

  /**
   * Utility method to check if an icoming request wants to manage configuration
   * parameters for the logo.
   */
  boolean handleConfig();

  /**
   * Handles the auto update of the software via OTA.
   */
  boolean handleOTA();

  /**
   * Reference to the storage class to store the crrent scene in the SPIFFS.
   */
  LogoStorage* storage = NULL;

  /**
   * Configuration class.
   */
  LogoConfig* config;

  /**
   * The LED strip to manage.
   */
  LEDStrip* strip = NULL;

  /**
   * Buttons configured to control the logo.
   */
  std::vector<LogoButton*>* buttons;

  /**
   * The actual webserver.
   */
  ESP8266WebServer* server = NULL;
};

#endif