#ifndef LogoConfig_h
#define LogoConfig_h

//  Debugging configuration
// -----------------------------------------

#define DEBUG

#ifdef DEBUG

#define debug(x, ...) \
  Serial.printf((String(x) + String("\n")).c_str(), __VA_ARGS__);
#define debugln(x) Serial.printf((String(x) + String("\n")).c_str());

#else
#define debugln(x)
#define debug(x, ...)
#endif

//  Controls the WIFI settings for the Logo.
// ------------------------------------------------------------------------------------

// enables the WiFi support for the logo. If this is commented out the logo will
// not try to setup a WiFi connection nor open an access point. In this case you
// can only use the buttons of the logo to control the LEDs. This makes only
// sense if the buttons were configured. Otherwise you will not have any
// possibility to control the logo beside of the default scene.
#define WIFI_ENABLE
// name of the access point that is created in case the logo was not able to
// find WiFi configuration values in the EEPROM
#define WIFI_NAME "Flower Of Life"
// password to use to connect to the access point
#define WIFI_PASSWORD "flower"
// MDNS name that can be used to connect to the logo from a device that supports
// MDNS service. To use this with windows please install bonjour.
#define WIFI_MDNS_NAME "flower"
// setting to true will enable debugging information for the WiFiManage libary.
#define WIFI_DEBUG false

//
// Global configuration for the used NeoPixel strip of the logo
// ------------------------------------------------------------------------------------
#define NEOPIXEL_PIN D3
#define NUMBER_OF_PIXELS 45

// Default delay for the animation
#define DEFAULT_SPEED 50
#define MAX_DELAY 20000

#define HTTP_LED_CONTROL_PREFIX "/led"

#define NUMBER_OF_BUTTONS 4
static const uint8_t BUTTON_PINS[NUMBER_OF_BUTTONS] = {D7, D5, D6, D8};

#endif