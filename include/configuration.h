#pragma once
#include "resourceManager.h"
#include "world.h"

class Configuration {
public:
  enum Textures : int {
    Player,
    Ant,
  };

  static ResourceManager<sf::Texture, int> textures;

  static unsigned int windowX;
  static unsigned int windowY;

  static float tileX;
  static float tileY;

  static double frameRate;
  static int colonySize;

  static int foodCount;
  static sf::Color foodColor;
  static sf::Color toHomePheromoneColor;
  static sf::Color toFoodPheromoneColor;
  static sf::Color strongToHomePheromoneColor;
  static sf::Color strongToFoodPheromoneColor;
  static sf::Color normalColor;
  static sf::Color colonyColor;
  static sf::Color strongPheromoneColor;

  static World *world;
  Configuration();
};
