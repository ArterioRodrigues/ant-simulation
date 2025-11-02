#include "configuration.h"
#include "resourceManager.h"
#include <SFML/Graphics/Texture.hpp>

ResourceManager<sf::Texture, int> Configuration::textures;

World *Configuration::world = nullptr;
unsigned int Configuration::windowX = 0;
unsigned int Configuration::windowY = 0;
double Configuration::frameRate = 0;
float Configuration::tileX = 0.f;
float Configuration::tileY = 0.f;
int Configuration::colonySize = 0;

Configuration::Configuration() {
  Configuration::windowX = 1000;
  Configuration::windowY = 1000;
  Configuration::frameRate = 10;
  Configuration::tileX = 25;
  Configuration::tileY = 25;
  Configuration::colonySize = 5;

  Configuration::world = new World(windowX, windowY, tileX, tileY);
  textures.load(Textures::Ant, "media/player/ant.png");
}
