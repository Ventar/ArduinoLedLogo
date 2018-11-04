#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <LEDWebServer.h>
#include <LogoConfig.h>

ESP8266WebServer espServer(80);

LEDWebServer::LEDWebServer(LEDStrip* strip, LogoStorage* storage) {
  this->server = &espServer;
  this->strip = strip;
  this->storage = storage;
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

boolean LEDWebServer::handleStore() { return false; }

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

  responseSend = responseSend || handleStore();

  if (!responseSend) {
    String path = uri;

    if (uri == "/status") {
      streamStatus();
      return;
    }

    if (uri == "/") {
      path = "index.htm";
    }

    if (uri == "/info") {
      path = "info.htm";
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
