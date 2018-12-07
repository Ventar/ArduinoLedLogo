#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ESP8266httpUpdate.h>
#include <FS.h>
#include <JSONOutput.h>
#include <LEDWebServer.h>
#include <vector>

ESP8266WebServer espServer(80);

LEDWebServer::LEDWebServer(LogoConfig* config, LEDStrip* strip,
                           LogoStorage* storage,
                           std::vector<LogoButton*>* buttons) {
  this->config = config;
  this->server = &espServer;
  this->strip = strip;
  this->storage = storage;
  this->buttons = buttons;
}

void LEDWebServer::setup() {
  debugln("LEDWebServer::setup - Setup LEDWebServer...");
  server->on("/", [=]() { handleRequest(); });
  server->onNotFound([=]() { handleRequest(); });
  server->begin();
  const char* headerkeys[] = {"Origin"};
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  server->collectHeaders(headerkeys, headerkeyssize);
}

void LEDWebServer::loop() { server->handleClient(); }

boolean LEDWebServer::handleAnimation(Animation* animation) {
  const String uri = server->uri();
  if (uri.equals("/led/" + animation->getPath())) {
    strip->setMode(animation->getSceneData()->modeName, server->arg("colors"),
                   server->arg("speed"));

    streamStatus(server, this);
    return true;
  }
  return false;
}

boolean LEDWebServer::handleScenes() {
  const String uri = server->uri();
  const String name = server->arg("name");
  if (uri.equals("/led/scene/add") && name != "") {
    storage->storeScene(name);
    streamStatus(server, this);
    return true;
  } else if (uri.equals("/led/scene/remove") && name != "") {
    storage->deleteScene(name);
    streamStatus(server, this);
    return true;
  } else if (uri.equals("/led/scene/set") && name != "") {
    if (name == "Off") {
      strip->setMode("Off");
      config->setParameter("LAST_SCENE", "Off");
      storage->saveConfig();
    } else {
      storage->loadScene(name);
      config->setParameter("LAST_SCENE", name);
      storage->saveConfig();
    }

    streamStatus(server, this);
    return true;
  }
  return false;
}

boolean LEDWebServer::handleButtons() {
  const String uri = server->uri();
  const String buttonName = server->arg("button");
  const String sceneName = server->arg("scene");
  if (uri.equals("/led/button/assign") && buttonName != "" && sceneName != "") {
    for (size_t i = 0; i < buttons->size(); i++) {
      if (buttons->at(i)->getName() == buttonName) {
        buttons->at(i)->assignScene(sceneName);
      }
    }
    streamStatus(server, this);
    return true;
  }

  return false;
}

boolean LEDWebServer::handleOTA() {
  const String uri = server->uri();
  const String updateURL = server->arg("url");
  const String fingerprint = server->arg("fingerprint");

  if (uri.equals("/ota") && updateURL != "") {
    debug("Exceute OTA update with url ::= [%s], fingerprint ::= [%s]",
          updateURL.c_str(), fingerprint.c_str());

    t_httpUpdate_return ret;

    if (fingerprint != "") {
      debugln("Update with fingerprint");
      ret = ESPhttpUpdate.update(updateURL, "", fingerprint);
    } else {
      debugln("Update without fingerprint");
      ret = ESPhttpUpdate.update(updateURL);
    }

    Serial.println("*** Done updating!");

    switch (ret) {
      case HTTP_UPDATE_FAILED:
        debug("Updated FAILED (%d): %s", ESPhttpUpdate.getLastError(),
              ESPhttpUpdate.getLastErrorString().c_str());
        break;

      case HTTP_UPDATE_NO_UPDATES:
        debugln("No update needed!");
        break;

      case HTTP_UPDATE_OK:
        debugln("Update OK!");
        break;
    }

    streamStatus(server, this);
    return true;
  }

  return false;
}

boolean LEDWebServer::handleConfig() {
  const String uri = server->uri();
  const String key = server->arg("key");
  const String value = server->arg("value");
  const String json = server->arg("json");
  if (uri.equals("/led/config/set") && key != "" && value != "") {
    config->setParameter(key, value);
    storage->saveConfig();
    streamStatus(server, this);
    return true;
  } else if (uri.equals("/led/config/update") && json != "") {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parse(json);
    JsonArray& data = root["config"].as<JsonArray>();

    for (auto& param : data) {
      String key = param["name"];
      String value = param["value"];
      config->setParameter(key, value);
    }
    storage->saveConfig();
    streamStatus(server, this);
    return true;
  }

  return false;
}

void LEDWebServer::handleRequest() {
  if (server->method() == HTTP_OPTIONS) {
    server->sendHeader("Access-Control-Allow-Origin", "*");
    server->sendHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
    server->sendHeader("Access-Control-Allow-Headers", "*");
    server->send(204);
    return;
  }

  if (server->hasHeader("Origin")) {
    server->sendHeader("Access-Control-Allow-Origin", server->header("Origin"));
  } else {
    server->sendHeader("Access-Control-Allow-Origin", "*");
  }

  server->sendHeader("Access-Control-Allow-Credentials", "true");

  const String uri = server->uri();

  debug("LEDWebserver::handleRequest - HTTP request to URI ::= [%s]",
        uri.c_str());

  boolean responseSend = false;
  std::vector<Animation*> animations = strip->getAnimations();
  for (size_t i = 0; i < animations.size(); i++) {
    if (handleAnimation(animations.at(i))) {
      responseSend = true;
      break;
    }
  }

  responseSend = responseSend || handleScenes() || handleButtons() ||
                 handleConfig() || handleOTA();

  if (!responseSend) {
    String path = uri;

    if (uri == "/status") {
      streamStatus(server, this);
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

LogoConfig* LEDWebServer::getConfig() { return config; }

LEDStrip* LEDWebServer::getLEDStrip() { return strip; }

LogoStorage* LEDWebServer::getStorage() { return storage; }

std::vector<LogoButton*>* LEDWebServer::getButtons() { return buttons; }
