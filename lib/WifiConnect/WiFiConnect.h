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
  WiFiConnect(String ssid, String pwd);

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
   * The name of the SSID of the network that is created when no WiFi connect
   * was possible.
   */
  String ssid;
  /**
   * The password to connect to the SSID network creatend when no WiFi connect
   * is possible.
   */
  String pwd;
};

#endif
