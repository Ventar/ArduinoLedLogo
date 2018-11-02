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

String LEDWebServer::createInfoMessage() {
  Animation* animation = strip->getAnimation();
  String info = "<html>\n";
  info += "<body>\n";
  info += "<h1>Logo Info Page</h1>\n";
  info += "<h2>Current Animation</h2>\n";
  info += "<p>\n";
  info += "Mode: ";
  info += strip->getModeName();
  info += "\n";
  info += "</p>\n";
  info +=
      "<p style='width: 200px; word-wrap:break-word; display:inline-block;'>\n";
  info += "Colors: ";
  info += animation->getColorListAsString();
  info += "\n";
  info += "</p>\n";
  info += "<p>\n";
  info += "Delay: ";
  info += animation->getSceneData()->delay;
  info += "\n";
  info += "</p>\n";

  info += "</body>\n";
  info += "</html>\n";
  return info;
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
    String speed = server->arg("speed");

    if (speed != "") {
      uint32_t s = speed.toInt();
      animation->setDelay(MAX_DELAY - (s * 1000));
    }

    String colorList = server->arg("colors");

    if (colorList != "") {
      animation->setColorListFromString(colorList);
    }

    strip->setMode(animation->getMode());
    server->send(200, "text/html", createInfoMessage());
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
