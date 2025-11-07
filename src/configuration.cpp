#include "configuration.h"
#include "resourceManager.h"
#include "world.h"

ResourceManager<sf::Texture, int> Configuration::textures;

unsigned int Configuration::windowX = 0;
unsigned int Configuration::windowY = 0;
double Configuration::frameRate = 0;

int Configuration::colonySize = 0;
int Configuration::foodCount = 0;
int Configuration::foodSize = 0;
int Configuration::movementDistance = 0;
int Configuration::pheromoneLimit = 0;
bool Configuration::toHomePheromones = false;
bool Configuration::toFoodPheromones = false;

sf::Color Configuration::foodColor;
sf::Color Configuration::colonyColor;
sf::Color Configuration::normalColor;

World *Configuration::world = nullptr;

Configuration::Configuration() {
  Configuration::windowX = 1000;
  Configuration::windowY = 800;

  Configuration::frameRate = 60;
  Configuration::foodCount = 1;

  Configuration::pheromoneLimit = 10000;
  Configuration::foodSize = 5;
  Configuration::colonySize = 50;
  Configuration::movementDistance = 20;

  Configuration::foodColor = {174, 234, 148};
  Configuration::colonyColor = {183, 113, 229};
  Configuration::normalColor = {17, 34, 78};
  Configuration::toHomePheromones = true;
  Configuration::toFoodPheromones = true;
  Configuration::textures.load(Textures::Ant, "media/player/ant.png");

  Configuration::world = new World(windowX, windowY, colonySize, foodCount);
}
