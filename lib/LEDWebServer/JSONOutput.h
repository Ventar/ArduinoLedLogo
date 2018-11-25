#ifndef JSONOutput_h
#define JSONOutput_h

#include <ArduinoJson.h>
#include <LEDStrip.h>
#include <LogoButton.h>
#include <LogoConfig.h>
#include <LogoStorage.h>
#include <vector>

String createJsonStatus(LogoDynamicConfig* config, LEDStrip* strip,
                        LogoStorage* storage, LogoButton** but) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  // common data
  // -------------------------------------------------------------

  root["mode"] = strip->getAnimation()->getSceneData()->modeName;
  root["usage"] = strip->getAnimation()->getSceneData()->ledUsageName;
  root["delay"] = strip->getAnimation()->getSceneData()->delay;
  root["speed"] = strip->getAnimation()->getSceneData()->speed;
  root["path"] = "/led/" + strip->getAnimation()->getPath();
  root["leds"] = strip->numPixels();

  // modes
  // -------------------------------------------------------------
  JsonArray& modes = root.createNestedArray("modes");
  std::vector<Animation*> animations = strip->getAnimations();
  for (size_t i = 0; i < animations.size(); i++) {
    JsonObject& mode = modes.createNestedObject();
    mode["name"] = animations[i]->getSceneData()->modeName;
    mode["path"] = "/led/" + animations[i]->getPath();
  }

  // buttons
  // -------------------------------------------------------------

  JsonArray& buttons = root.createNestedArray("buttons");
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    JsonObject& btn = buttons.createNestedObject();
    btn["name"] = but[i]->getName();
    btn["scene"] = but[i]->getSceneName();
  }

  // scenes
  // -------------------------------------------------------------

  JsonArray& scenes = root.createNestedArray("scenes");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    if (dir.fileName().startsWith("/scene_")) {
      scenes.add(dir.fileName().substring(7));
    }
  }

  // colors
  // -------------------------------------------------------------

  JsonArray& colors = root.createNestedArray("colors");
  for (int i = 0; i < strip->numPixels(); i++) {
    String hexString =
        String(strip->getAnimation()->getSceneData()->colors[i], HEX);
    while (hexString.length() < 6) hexString = "0" + hexString;

    colors.add(hexString);
  }

  // file system
  // -------------------------------------------------------------

  uint32_t totalSize = 0;
  uint32_t numerOfFiles = 0;
  JsonObject& fileSystem = root.createNestedObject("filesystem");
  JsonArray& files = fileSystem.createNestedArray("files");

  dir = SPIFFS.openDir("/");
  while (dir.next()) {
    numerOfFiles++;
    totalSize += dir.fileSize();
    JsonObject& file = files.createNestedObject();
    file["name"] = dir.fileName();
    file["size"] = dir.fileSize();
  }

  fileSystem["count"] = numerOfFiles;
  fileSystem["size"] = totalSize;
  fileSystem["total"] = 4 * 1024 * 1024;

  // END
  // -------------------------------------------------------------

  String content;
  root.prettyPrintTo(content);

  return content;
}

#endif