#include "configuration.h"

World *Configuration::world = nullptr;
unsigned int Configuration::_windowX = 0;
unsigned int Configuration::_windowY = 0;
double Configuration::_frameRate = 0;
float Configuration::_tileX = 0.f;
float Configuration::_tileY = 0.f;

Configuration::Configuration() {

  Configuration::_windowX = 500;
  Configuration::_windowY = 500;
  Configuration::_frameRate = 144;
  Configuration::_tileX = 50;
  Configuration::_tileY = 50;

  Configuration::world = new World(_windowX, _windowY, _tileX, _tileY);
}
