
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
  LEDWebServer(LEDStrip* strip, LogoStorage* storage, LogoButton** buttons);

  /**
   * Performs the needed setup for the LED web server.
   */
  void setup();

  /**
   * Called during the loop() call to check if a ne request came in to update
   * the underlying LED strip
   */
  void loop();

 private:
  void streamStatus();

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
   * Reference to the storage class to store the crrent scene in the SPIFFS.
   */
  LogoStorage* storage = NULL;

  /**
   * The LED strip to manage.
   */
  LEDStrip* strip = NULL;

  /**
   * Buttons configured to control the logo.
   */
  LogoButton** buttons;

  /**
   * The actual webserver.
   */
  ESP8266WebServer* server = NULL;
};

#endif