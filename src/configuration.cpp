#include "configuration.h"
#include "resourceManager.h"
#include <SFML/Graphics/Texture.hpp>

ResourceManager<sf::Texture, int> Configuration::textures;

unsigned int Configuration::windowX = 0;
unsigned int Configuration::windowY = 0;
double Configuration::frameRate = 0;
float Configuration::tileX = 0.f;
float Configuration::tileY = 0.f;
int Configuration::colonySize = 0;
int Configuration::foodCount = 0;
sf::Color Configuration::foodColor;
sf::Color Configuration::pheromoneColor;
sf::Color Configuration::normalColor;
sf::Color Configuration::colonyColor;
sf::Color Configuration::strongPheromoneColor;
sf::Color Configuration::searchingColor;

World *Configuration::world = nullptr;
Configuration::Configuration() {
  Configuration::windowX = 1000;
  Configuration::windowY = 800;
  Configuration::frameRate = 60;
  Configuration::tileX = 10;
  Configuration::tileY = 10;
  Configuration::colonySize = 100;
  Configuration::foodCount = 10;
  Configuration::foodColor = {255, 0, 255};
  Configuration::pheromoneColor = {0, 255, 0};
  Configuration::searchingColor = {150, 0, 150};
  Configuration::normalColor = {255, 255 , 255};
  Configuration::colonyColor = {255, 0, 0};
  world = new World(windowX, windowY, tileX, tileY, foodCount);
  textures.load(Textures::Ant, "media/player/ant.png");
}
