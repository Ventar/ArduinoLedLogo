#include <LogoStorage.h>

void LogoStorage::setup() {
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

  f.println(name);
  f.println(strip->getAnimation()->getSceneData()->modeName);
  f.println(strip->getAnimation()->getSceneData()->speed);
  f.println(strip->getAnimation()->getColorListAsString());

  f.close();
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

  debug(
      "LogoStorage::loadScene - sceneName ::= [%s], modeName "
      "::= [%s], speed ::= [%s], colors ::= [%s]",
      sceneName.c_str(), modeName.c_str(), speed.c_str(), colors.c_str());

  f.close();

  strip->setMode(modeName, colors, speed);
}

void LogoStorage::deleteScene(String name) {
  String filename = "scene_" + name;
  SPIFFS.remove("/" + filename);
}

boolean LogoStorage::exists(String name) { return SPIFFS.exists(name); }

File LogoStorage::open(String name) { return SPIFFS.open(name, "r"); }