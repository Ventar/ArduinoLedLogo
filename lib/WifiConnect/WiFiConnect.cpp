#include <Arduino.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <LogoConfig.h>
#include <WiFiClient.h>
#include <WiFiConnect.h>
#include <WiFiManager.h>

WiFiConnect::WiFiConnect(LogoConfig* config) { this->config = config; }

void configModeCallback(WiFiManager* myWiFiManager) {
  debug(
      "WifiConnect::configModeCallback - Entered WiFi config mode IP ::=[%s], "
      "SSID ::=[%s], MAC "
      "::= [%s]",
      WiFi.softAPIP().toString().c_str(),
      myWiFiManager->getConfigPortalSSID().c_str(), WiFi.macAddress().c_str());
}

void WiFiConnect::reset() {
  WiFiManager wifiManager;
  wifiManager.resetSettings();
}

void WiFiConnect::connect() {
  debugln(
      "WiFiConnect::connect -  Connect executed, try to connect to WiFi "
      "network ...");

  WiFiManager wifiManager;
  wifiManager.setDebugOutput(WIFI_DEBUG);
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.autoConnect(config->getParameterAsCString("WIFI_NAME"),
                          config->getParameterAsCString("WIFI_PASSWORD"));
  WiFi.hostname("MROPOINT");

  debug(
      "WifiConnect::connect - Connected to IP ::= [%s], SSID ::= [%s], MAC "
      "Address ::= "
      "[%s]",
      WiFi.localIP().toString().c_str(), WiFi.SSID().c_str(),
      WiFi.macAddress().c_str());

  if (MDNS.begin(config->getParameterAsCString("WIFI_MDNS_NAME"))) {
    debug("WiFiConnect::connect - MDNS responder started with name ::= [%s]",
          config->getParameterAsString("WIFI_MDNS_NAME").c_str());
  }
}
