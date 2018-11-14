#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <LEDWebServer.h>
#include <LogoConfig.h>

ESP8266WebServer espServer(80);

LEDWebServer::LEDWebServer(LEDStrip* strip, LogoStorage* storage,
                           LogoButton** buttons) {
  this->server = &espServer;
  this->strip = strip;
  this->storage = storage;
  this->buttons = buttons;
}

void LEDWebServer::streamStatus() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["mode"] = strip->getAnimation()->getSceneData()->modeName;
  root["usage"] = strip->getAnimation()->getSceneData()->ledUsageName;
  root["delay"] = strip->getAnimation()->getSceneData()->delay;
  root["speed"] = strip->getAnimation()->getSceneData()->speed;
  root["path"] =
      String(HTTP_LED_CONTROL_PREFIX) + "/" + strip->getAnimation()->getPath();
  root["leds"] = NUMBER_OF_PIXELS;
  JsonArray& modes = root.createNestedArray("modes");

  Animation** animations = strip->getAnimations();
  for (int i = 0; i < strip->getAnimationsCount(); i++) {
    JsonObject& mode = modes.createNestedObject();
    mode["name"] = animations[i]->getSceneData()->modeName;
    mode["path"] =
        String(HTTP_LED_CONTROL_PREFIX) + "/" + animations[i]->getPath();
  }

  JsonArray& buttons = root.createNestedArray("buttons");
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
    JsonObject& btn = buttons.createNestedObject();
    btn["name"] = this->buttons[i]->getName();
    btn["scene"] = this->buttons[i]->getSceneName();
  }

  JsonArray& scenes = root.createNestedArray("scenes");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    if (dir.fileName().startsWith("/scene_")) {
      scenes.add(dir.fileName().substring(7));
    }
  }

  JsonArray& colors = root.createNestedArray("colors");

  for (int i = 0; i < NUMBER_OF_PIXELS; i++) {
    String hexString =
        String(strip->getAnimation()->getSceneData()->colors[i], HEX);
    while (hexString.length() < 6) hexString = "0" + hexString;

    colors.add(hexString);
  }

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
  if (uri.equals(String(HTTP_LED_CONTROL_PREFIX) + "/" +
                 animation->getPath())) {
    strip->setMode(animation->getSceneData()->mode, server->arg("colors"),
                   server->arg("speed"));

    streamStatus();
    return true;
  }
  return false;
}

boolean LEDWebServer::handleScenes() {
  const String uri = server->uri();
  const String name = server->arg("name");
  if (uri.equals(String(HTTP_LED_CONTROL_PREFIX) + "/scene/add") &&
      name != "") {
    storage->storeScene(name);
    streamStatus();
    return true;
  } else if (uri.equals(String(HTTP_LED_CONTROL_PREFIX) + "/scene/remove") &&
             name != "") {
    storage->deleteScene(name);
    streamStatus();
    return true;
  } else if (uri.equals(String(HTTP_LED_CONTROL_PREFIX) + "/scene/set") &&
             name != "") {
    if (name == "Off") {
      strip->setMode(OFF);
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
  if (uri.equals(String(HTTP_LED_CONTROL_PREFIX) + "/button/assign") &&
      buttonName != "" && sceneName != "") {
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

  Serial.print("HTTP request to URI ::= ");
  Serial.println(uri);

  boolean responseSend = false;
  Animation** animations = strip->getAnimations();
  for (int i = 0; i < strip->getAnimationsCount(); i++) {
    if (handleAnimation(animations[i])) {
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

    if (uri == "/") {
      path = "/index.htm";
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
