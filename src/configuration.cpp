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
sf::Color Configuration::toHomePheromoneColor;
sf::Color Configuration::toFoodPheromoneColor;
sf::Color Configuration::strongToHomePheromoneColor;
sf::Color Configuration::strongToFoodPheromoneColor;
sf::Color Configuration::normalColor;
sf::Color Configuration::colonyColor;
sf::Color Configuration::strongPheromoneColor;

World *Configuration::world = nullptr;
Configuration::Configuration() {
  Configuration::windowX = 1000;
  Configuration::windowY = 1000;
  Configuration::frameRate = 60;
  Configuration::tileX = 25;
  Configuration::tileY = 25;
  Configuration::colonySize = 5;
  Configuration::foodCount = 10;
  Configuration::foodColor = {255, 0, 255};                // Magenta
  Configuration::toHomePheromoneColor = {200, 255, 200};   // Light green
  Configuration::toFoodPheromoneColor = {200, 200, 255};   // Light blue
  Configuration::strongToHomePheromoneColor = {0, 255, 0}; // Bright green
  Configuration::strongToFoodPheromoneColor = {0, 0, 255}; // Bright blue
  Configuration::normalColor = {255, 255, 255};            // White
  Configuration::colonyColor = {255, 0, 0};
  world = new World(windowX, windowY, tileX, tileY, foodCount);
  textures.load(Textures::Ant, "media/player/ant.png");
}
