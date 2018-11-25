#ifndef LogoDynamicConfig_h
#define LogoDynamicConfig_h
#include <Arduino.h>
#include <map>

//  Debugging configuration
// -----------------------------------------

#define DEBUG

#ifdef DEBUG

#define debug(x, ...) \
  Serial.printf((String(x) + String("\n")).c_str(), __VA_ARGS__);
#define debugln(x) Serial.printf((String(x) + String("\n")).c_str());

#else
#define debugln(x)
#define debug(x, ...)
#endif

// setting to true will enable debugging information for the WiFiManage libary.
#define WIFI_DEBUG true

#define NUMBER_OF_BUTTONS 4
static const uint8_t BUTTON_PINS[NUMBER_OF_BUTTONS] = {D7, D5, D6, D8};

/**
 * Configuration Keys for the logo.
 */
enum ConfigKeys {
  NEOPIXEL_NUMBER,
  NEOPIXEL_PIN,
  DEFAULT_SPEED,
  MAX_DELAY,
  WIFI_NAME,
  WIFI_PASSWORD,
  WIFI_MDNS_NAME
};

/**
 * Dynamic configuration values for the logo.
 */
class LogoDynamicConfig {
 public:
  /**
   * Constructor.
   */
  LogoDynamicConfig() { configMap = new std::map<ConfigKeys, String>; };

  /**
   * Returns a value from the map.
   */
  String getParameterAsString(ConfigKeys key);
  /**
   * Returns a value from the map.
   */
  const char* getParameterAsCString(ConfigKeys key);

  /**
   * Returns a value from the map.
   */
  uint8_t getParameterAsShort(ConfigKeys key);

  /**
   * Returns a value from the map.
   */
  uint16_t getParameterAsInt(ConfigKeys key);

  /**
   * Returns a value from the map.
   */
  uint32_t getParameterAsLong(ConfigKeys key);

  /**
   * Sets a value to the map.
   */
  void setParameter(ConfigKeys key, String value);

  /**
   * Returns a reference to the map with he configuration values.
   */
  std::map<ConfigKeys, String>* getConfigMap();

 private:
  /**
   * Map with parameters.
   */
  std::map<ConfigKeys, String>* configMap;
};

#endif