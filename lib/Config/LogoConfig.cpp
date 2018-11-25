#include <LogoConfig.h>

String LogoDynamicConfig::getParameterAsString(ConfigKeys key) {
  std::map<ConfigKeys, String>::iterator itr = configMap->find(key);
  if (itr == configMap->end()) {
    switch (key) {
      case NEOPIXEL_NUMBER:
        return "44";
      case NEOPIXEL_PIN:
        return "0";
      case DEFAULT_SPEED:
        return "50";
      case MAX_DELAY:
        return "20000";
      case WIFI_NAME:
        return "MRO Inc. LED Device";
      case WIFI_PASSWORD:
        return "mro";
      case WIFI_MDNS_NAME:
        return "mro";
    }
  } else {
    return itr->second;
  }
}

uint8_t LogoDynamicConfig::getParameterAsShort(ConfigKeys key) {
  return (uint8_t)getParameterAsString(key).toInt();
}

uint16_t LogoDynamicConfig::getParameterAsInt(ConfigKeys key) {
  return (uint16_t)getParameterAsString(key).toInt();
}

uint32_t LogoDynamicConfig::getParameterAsLong(ConfigKeys key) {
  return (uint32_t)getParameterAsString(key).toInt();
}

const char* LogoDynamicConfig::getParameterAsCString(ConfigKeys key) {
  return getParameterAsString(key).c_str();
}

void LogoDynamicConfig::setParameter(ConfigKeys key, String value) {}

std::map<ConfigKeys, String>* LogoDynamicConfig::getConfigMap() {
  return configMap;
}