#include <LogoStorage.h>

void LogoStorage::setup() { SPIFFS.begin(); }

boolean LogoStorage::exists(String name) { return SPIFFS.exists(name); }

File LogoStorage::open(String name) { return SPIFFS.open(name, "r"); }