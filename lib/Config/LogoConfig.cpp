#include <LogoConfig.h>

String LogoConfig::getParameterAsString(String key) {
  std::map<String, String>::iterator itr = configMap->find(key);
  return itr->second;
}

uint8_t LogoConfig::getParameterAsShort(String key) {
  return (uint8_t)getParameterAsString(key).toInt();
}

uint8_t LogoConfig::getParameterAsPin(String key) {
  String data = getParameterAsString(key);
  if (data == "D0") {
    return D0;
  } else if (data == "D1") {
    return D1;
  } else if (data == "D2") {
    return D2;
  } else if (data == "D3") {
    return D3;
  } else if (data == "D4") {
    return D4;
  } else if (data == "D5") {
    return D5;
  } else if (data == "D6") {
    return D6;
  } else if (data == "D7") {
    return D7;
  } else if (data == "D8") {
    return D8;
  }
  return 0;
}

std::vector<uint8_t> LogoConfig::getParameterAsPinList(String key) {
  String pins = getParameterAsString(key);
  std::vector<uint8_t> vector;

  char delimiter[] = ",;";
  char* ptr;

  ptr = strtok((char*)pins.c_str(), delimiter);

  while (ptr != NULL) {
    String data = String(ptr);

    if (data == "D0") {
      vector.push_back(D0);
    } else if (data == "D1") {
      vector.push_back(D0);
    } else if (data == "D2") {
      vector.push_back(D2);
    } else if (data == "D3") {
      vector.push_back(D3);
    } else if (data == "D4") {
      vector.push_back(D4);
    } else if (data == "D5") {
      vector.push_back(D5);
    } else if (data == "D6") {
      vector.push_back(D6);
    } else if (data == "D7") {
      vector.push_back(D7);
    } else if (data == "D8") {
      vector.push_back(D8);
    }

    ptr = strtok(NULL, delimiter);
  }
  return vector;
}

uint16_t LogoConfig::getParameterAsInt(String key) {
  return (uint16_t)getParameterAsString(key).toInt();
}

uint32_t LogoConfig::getParameterAsLong(String key) {
  return (uint32_t)getParameterAsString(key).toInt();
}

void LogoConfig::setParameter(String key, String value) {
  if (configMap->find(key) != configMap->end()) {
    configMap->erase(key);
  }
  configMap->insert(std::pair<String, String>(key, value));
}

std::map<String, String>* LogoConfig::getConfigMap() { return configMap; }