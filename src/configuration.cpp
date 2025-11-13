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
bool Configuration::enableMultitThreading = false;

sf::Color Configuration::foodColor;
sf::Color Configuration::colonyColor;
sf::Color Configuration::normalColor;

World *Configuration::world = nullptr;

Configuration::Configuration() {
  Configuration::windowX = 1000;
  Configuration::windowY = 800;

  Configuration::frameRate = 24;
  Configuration::foodCount = 3;
  Configuration::foodSize = 5;

  Configuration::pheromoneLimit = 1000;
  Configuration::colonySize = 50;
  Configuration::movementDistance = 20;

  Configuration::foodColor = {0, 255, 0};
  Configuration::colonyColor = {255, 0, 225};
  Configuration::normalColor = {49, 54, 71};
  Configuration::toHomePheromones = true;
  Configuration::toFoodPheromones = true;
  Configuration::enableMultitThreading = true;
  Configuration::textures.load(Textures::Ant, "media/player/ant.png");

  Configuration::world = new World(windowX, windowY, colonySize, foodCount);
}
