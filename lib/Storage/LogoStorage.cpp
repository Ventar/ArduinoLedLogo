
#include <LogoStorage.h>
#include <iterator>
#include <map>

void LogoStorage::setup() {
  debugln("Setup LogoStorage...");
  SPIFFS.begin();

#ifdef DEBUG
  String str = "LogoStorage: file system content: \nLogoStorage:    ";
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    str += dir.fileName();
    str += " / ";
    str += dir.fileSize();
    str += "\nLogoStorage:    ";
  }
  str += "\n";
  Serial.print(str);
#endif
}

void LogoStorage::storeScene(String name) {
  String filename = "scene_" + name;
  File f = SPIFFS.open("/" + filename, "w");
  if (!f) {
    debug("LogoStorage::storeScene - Cannot open file ::= [%s] to write",
          filename.c_str());
    return;
  }

  f.print(name + "\n");
  f.print(strip->getAnimation()->getSceneData()->modeName + "\n");
  f.print(String(strip->getAnimation()->getSceneData()->speed) + "\n");
  f.print(strip->getAnimation()->getColorListAsString() + "\n");

  f.close();

  debug(
      "LogoStorage::storeScene - sceneName ::= [%s], modeName ::= [%s], speed "
      "::= [%s], colors ::= [%s]",
      name.c_str(), strip->getAnimation()->getSceneData()->modeName.c_str(),
      String(strip->getAnimation()->getSceneData()->speed).c_str(),
      strip->getAnimation()->getColorListAsString().c_str());
}

void LogoStorage::storeSceneForButton(String buttonName, String sceneName) {
  String filename = "button_" + buttonName;
  File f = SPIFFS.open("/" + filename, "w");
  if (!f) {
    debug(
        "LogoStorage::storeSceneForButton - Cannot open file ::= [%s] to write",
        filename.c_str());
    return;
  }

  f.print(sceneName + "\n");

  f.close();
}

String LogoStorage::loadSceneForButton(String buttonName) {
  String filename = "button_" + buttonName;
  File f = SPIFFS.open("/" + filename, "r");
  if (!f) {
    debug("LogoStorage::loadSceneForButton - Cannot open file ::= [%s] to read",
          filename.c_str());
    return "Off";
  }
  String sceneName = f.readStringUntil('\n');

  f.close();

  return sceneName;
}

void LogoStorage::loadScene(String name) {
  String filename = "scene_" + name;
  File f = SPIFFS.open("/" + filename, "r");
  if (!f) {
    debug("LogoStorage::loadScene - Cannot open file ::= [%s] to read",
          filename.c_str());
    return;
  }
  String sceneName = f.readStringUntil('\n');
  String modeName = f.readStringUntil('\n');
  String speed = f.readStringUntil('\n');
  String colors = f.readStringUntil('\n');

  f.close();

  debug("LogoStorage::loadScene - sceneName ::= [%s], modeName ::= [%s]",
        sceneName.c_str(), modeName.c_str());

  strip->setMode(modeName, colors, speed);
}

void LogoStorage::deleteScene(String name) {
  String filename = "scene_" + name;
  SPIFFS.remove("/" + filename);
}

boolean LogoStorage::exists(String name) { return SPIFFS.exists(name); }

File LogoStorage::open(String name) { return SPIFFS.open(name, "r"); }

void LogoStorage::loadConfig() {
  File f = SPIFFS.open("/config", "r");

  String line = f.readStringUntil('\n');
  while (line != NULL) {
    String key = line.substring(0, line.indexOf("="));
    String value = line.substring(line.indexOf("=") + 1);
    debug("LogoStorage::loadConfig - %s ::=[%s]", key.c_str(), value.c_str());
    config->setParameter(key, value);
    line = f.readStringUntil('\n');
  }

  f.close();
}

void LogoStorage::saveConfig() {
  SPIFFS.remove("/config");
  File f = SPIFFS.open("/config", "w");

  std::map<String, String>* data = config->getConfigMap();
  std::map<String, String>::iterator it;

  for (it = data->begin(); it != data->end(); it++) {
    f.print(it->first + String("=") + it->second + String("\n"));
  }

  f.close();
}