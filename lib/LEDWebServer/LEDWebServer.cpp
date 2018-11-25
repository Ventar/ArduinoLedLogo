#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <LEDWebServer.h>
#include <vector>

ESP8266WebServer espServer(80);

LEDWebServer::LEDWebServer(LogoDynamicConfig* config, LEDStrip* strip,
                           LogoStorage* storage, LogoButton** buttons) {
  this->config = config;
  this->server = &espServer;
  this->strip = strip;
  this->storage = storage;
  this->buttons = buttons;
}

void LEDWebServer::streamStatus() {
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
    btn["name"] = this->buttons[i]->getName();
    btn["scene"] = this->buttons[i]->getSceneName();
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

  server->sendHeader("Connection", "close");
  server->send(200, "application/json", content);
}

void LEDWebServer::setup() {
  server->on("/", [=]() { handleRequest(); });
  server->onNotFound([=]() { handleRequest(); });
  server->begin();
}

void LEDWebServer::loop() { server->handleClient(); }

boolean LEDWebServer::handleAnimation(Animation* animation) {
  const String uri = server->uri();
  if (uri.equals("/led/" + animation->getPath())) {
    strip->setMode(animation->getSceneData()->modeName, server->arg("colors"),
                   server->arg("speed"));

    streamStatus();
    return true;
  }
  return false;
}

boolean LEDWebServer::handleScenes() {
  const String uri = server->uri();
  const String name = server->arg("name");
  if (uri.equals("/led/scene/add") && name != "") {
    storage->storeScene(name);
    streamStatus();
    return true;
  } else if (uri.equals("/led/scene/remove") && name != "") {
    storage->deleteScene(name);
    streamStatus();
    return true;
  } else if (uri.equals("/led/scene/set") && name != "") {
    if (name == "Off") {
      strip->setMode("Off");
    } else {
      storage->loadScene(name);
    }

    streamStatus();
    return true;
  }
  return false;
}

boolean LEDWebServer::handleButtons() {
  const String uri = server->uri();
  const String buttonName = server->arg("button");
  const String sceneName = server->arg("scene");
  if (uri.equals("/led/button/assign") && buttonName != "" && sceneName != "") {
    for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
      if (buttons[i]->getName() == buttonName) {
        buttons[i]->assignScene(sceneName);
      }
    }
    streamStatus();
    return true;
  }

  return false;
}

void LEDWebServer::handleRequest() {
  server->sendHeader("Access-Control-Allow-Origin", "*");
  const String uri = server->uri();

  debug("HTTP request to URI ::= [%s]", uri.c_str());

  boolean responseSend = false;
  std::vector<Animation*> animations = strip->getAnimations();
  for (size_t i = 0; i < animations.size(); i++) {
    if (handleAnimation(animations.at(i))) {
      responseSend = true;
      break;
    }
  }

  responseSend = responseSend || handleScenes() || handleButtons();

  if (!responseSend) {
    String path = uri;

    if (uri == "/status") {
      streamStatus();
      return;
    }

    if (uri == "/reset") {
      ESP.restart();
      return;
    }

    if (uri == "/") {
      path = "/index.html";
    }

    if (!storage->exists(path)) {
      Serial.println("Cannot access file...");
      server->send(404);
      return;
    }

    File dataFile = storage->open(path);

    server->setContentLength(dataFile.size());
    server->sendHeader("Connection", "keep-alive");
    server->send(200, "text/html", "");

    WiFiClient client = server->client();
    client.setNoDelay(true);

    char buf[1024];
    int dataToSend = dataFile.size();
    while (dataToSend > 0) {
      size_t len = std::min((int)(sizeof(buf) - 1), dataToSend);
      dataFile.readBytes(buf, len);
      client.write(buf, len);
      client.flush();
      dataToSend -= len;
    }

    dataFile.close();
  }
}
