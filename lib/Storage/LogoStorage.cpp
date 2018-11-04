#include <LogoStorage.h>

void LogoStorage::setup() {
  SPIFFS.begin();

#ifdef DEBUG
  String str = "LogoStorage: file system content: \nLogoStorage:    ";
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    str += dir.fileName();
    str += " / ";
    str += dir.fileSize();
    str += "\nLogoStorage:    ";
  }
  str += "\n";
  Serial.print(str);
#endif
}

boolean LogoStorage::exists(String name) { return SPIFFS.exists(name); }

File LogoStorage::open(String name) { return SPIFFS.open(name, "r"); }