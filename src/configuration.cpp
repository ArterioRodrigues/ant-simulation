#include "configuration.h"
#include "resourceManager.h"
#include <SFML/Graphics/Texture.hpp>

ResourceManager<sf::Texture, int> Configuration::textures;

World *Configuration::world = nullptr;
unsigned int Configuration::_windowX = 0;
unsigned int Configuration::_windowY = 0;
double Configuration::_frameRate = 0;
float Configuration::_tileX = 0.f;
float Configuration::_tileY = 0.f;

Configuration::Configuration() {
  Configuration::_windowX = 1000;
  Configuration::_windowY = 1000;
  Configuration::_frameRate = 10;
  Configuration::_tileX = 25;
  Configuration::_tileY = 25;

  Configuration::world = new World(_windowX, _windowY, _tileX, _tileY);
  textures.load(Textures::Ant, "media/player/ant.png");
}
