#include <Arduino.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <LogoConfig.h>
#include <WiFiClient.h>
#include <WiFiConnect.h>
#include <WiFiManager.h>

WiFiConnect::WiFiConnect(String ssid, String pwd) {
  this->ssid = ssid;
  this->pwd = pwd;
}

void configModeCallback(WiFiManager *myWiFiManager) {
  debug(
      "WifiConnect: Entered WiFi config mode IP ::=[%s], SSID ::=[%s], MAC "
      "::= [%s]",
      WiFi.softAPIP().toString().c_str(),
      myWiFiManager->getConfigPortalSSID().c_str(), WiFi.macAddress().c_str());
}

void WiFiConnect::reset() {
  WiFiManager wifiManager;
  wifiManager.resetSettings();
}

void WiFiConnect::connect() {
  debugln("WiFiConnect: Connect executed, try to connect to WiFi network ...");

  WiFiManager wifiManager;
  wifiManager.setDebugOutput(WIFI_DEBUG);
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.autoConnect(ssid.c_str(), pwd.c_str());
  WiFi.hostname("Flower of Life");

  debug(
      "WifiConnect: Connected to IP ::= [%s], SSID ::= [%s], MAC Address ::= "
      "[%s]",
      WiFi.localIP().toString().c_str(), WiFi.SSID().c_str(),
      WiFi.macAddress().c_str());

  if (MDNS.begin(WIFI_MDNS_NAME)) {
    debug("WiFiConnect: MDNS responder started with name ::= [%s]",
          WIFI_MDNS_NAME);
  }
}
