#ifndef WiFiConnect_h
#define WiFiConnect_h

#include <Arduino.h>

/**
 * Simple wrapper class arround the WiFi libary.
 */
class WiFiConnect {
 public:
  /**
   * Constructor.
   */
  WiFiConnect(LogoConfig* config);

  /**
   * Starts the connection process. Either connects to the stored WiFi network
   * or opens an access point.
   */
  void connect();

  /**
   * Reset the stored WiFi credentials.
   */
  void reset();

 private:
  /**
   * Logo configration.
   */
  LogoConfig* config;
};

#endif
