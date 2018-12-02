#ifndef JSONOutput_h
#define JSONOutput_h

#include <ArduinoJson.h>
#include <LEDStrip.h>
#include <LEDWebServer.h>
#include <LogoButton.h>
#include <LogoConfig.h>
#include <LogoStorage.h>
#include <vector>

/**
 * Add the data of the current running scene to the JSON object.
 */
void addActiveSceneData(LEDWebServer* server, JsonObject& root) {
  root["mode"] =
      server->getLEDStrip()->getAnimation()->getSceneData()->modeName;
  root["usage"] =
      server->getLEDStrip()->getAnimation()->getSceneData()->ledUsageName;
  root["delay"] = server->getLEDStrip()->getAnimation()->getSceneData()->delay;
  root["speed"] = server->getLEDStrip()->getAnimation()->getSceneData()->speed;
  root["path"] = "/led/" + server->getLEDStrip()->getAnimation()->getPath();
  root["leds"] = server->getLEDStrip()->numPixels();

  JsonArray& colors = root.createNestedArray("colors");
  for (int i = 0; i < server->getLEDStrip()->numPixels(); i++) {
    String hexString = String(
        server->getLEDStrip()->getAnimation()->getSceneData()->colors[i], HEX);
    while (hexString.length() < 6) hexString = "0" + hexString;

    colors.add(hexString);
  }
}

/**
 * Adds a list of supported modes / animations to the JSON object.
 */
void addAvailableModes(LEDWebServer* server, JsonObject& root) {
  JsonArray& modes = root.createNestedArray("modes");
  std::vector<Animation*> animations = server->getLEDStrip()->getAnimations();
  for (size_t i = 0; i < animations.size(); i++) {
    JsonObject& mode = modes.createNestedObject();
    mode["name"] = animations[i]->getSceneData()->modeName;
    mode["path"] = "/led/" + animations[i]->getPath();
  }
}

/**
 * Adds a list of available buttons and their assigned scenes to the JSON
 * object.
 */
void addAvailableButtons(LEDWebServer* server, JsonObject& root) {
  JsonArray& buttons = root.createNestedArray("buttons");
  Serial.println(server->getButtons()->size());
  for (size_t i = 0; i < server->getButtons()->size(); i++) {
    JsonObject& btn = buttons.createNestedObject();
    btn["name"] = server->getButtons()->at(i)->getName();
    btn["scene"] = server->getButtons()->at(i)->getSceneName();
  }
}

/**
 * Adds a list of available scenes with their names to the JSON object.
 */
void addAvailableScenes(LEDWebServer* server, JsonObject& root) {
  JsonArray& scenes = root.createNestedArray("scenes");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    if (dir.fileName().startsWith("/scene_")) {
      scenes.add(dir.fileName().substring(7));
    }
  }
}

/**
 * Adds the data of the filesystem to the JSON object.
 */
void addFileSystemData(LEDWebServer* server, JsonObject& root) {
  uint32_t totalSize = 0;
  uint32_t numerOfFiles = 0;
  JsonObject& fileSystem = root.createNestedObject("filesystem");
  JsonArray& files = fileSystem.createNestedArray("files");

  Dir dir = SPIFFS.openDir("/");
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
}

/**
 * Adds a list of supported modes / animations to the JSON object.
 */
void addConfigData(LEDWebServer* server, JsonObject& root) {
  JsonArray& modes = root.createNestedArray("config");
  std::map<String, String>* config = server->getConfig()->getConfigMap();

  std::map<String, String>::iterator itr;
  for (itr = config->begin(); itr != config->end(); ++itr) {
    JsonObject& mode = modes.createNestedObject();
    mode["name"] = itr->first;
    mode["value"] = itr->second;
  }
}

String createJsonStatus(LEDWebServer* server) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  addActiveSceneData(server, root);
  addAvailableModes(server, root);
  addAvailableButtons(server, root);
  addAvailableScenes(server, root);
  addFileSystemData(server, root);
  addConfigData(server, root);

  String content;
  root.prettyPrintTo(content);

  return content;
}

void streamStatus(ESP8266WebServer* httpServer, LEDWebServer* server) {
  httpServer->sendHeader("Connection", "close");
  httpServer->send(200, "application/json", createJsonStatus(server));
}

#endif