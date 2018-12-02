#ifndef LogoConfig_h
#define LogoConfig_h
#include <Arduino.h>
#include <iterator>
#include <map>
#include <vector>

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

/**
 * Dynamic configuration values for the logo.
 */
class LogoConfig {
 public:
  /**
   * Constructor.
   */
  LogoConfig() { configMap = new std::map<String, String>; };

  /**
   * Returns a value from the map.
   */
  String getParameterAsString(String key);
  /**
   * Returns a value from the map.
   */
  const char* getParameterAsCString(String key);

  /**
   * Returns a value from the map.
   */
  uint8_t getParameterAsPin(String key);

  /**
   * Returns a value from the map.
   */
  std::vector<uint8_t> getParameterAsPinList(String key);

  /**
   * Returns a value from the map.
   */
  uint8_t getParameterAsShort(String key);

  /**
   * Returns a value from the map.
   */
  uint16_t getParameterAsInt(String key);

  /**
   * Returns a value from the map.
   */
  uint32_t getParameterAsLong(String key);

  /**
   * Sets a value to the map.
   */
  void setParameter(String key, String value);

  /**
   * Returns a reference to the map with he configuration values.
   */
  std::map<String, String>* getConfigMap();

 private:
  /**
   * Map with parameters.
   */
  std::map<String, String>* configMap;
};

#endif