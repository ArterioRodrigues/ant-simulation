#pragma once
#include "pch.h"
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

  static double frameRate;
  static int colonySize;
  static int foodSize;
  static int pheromoneLimit;

  static int movementDistance;
  static int foodCount;

  static bool toHomePheromones;
  static bool toFoodPheromones;
  static bool enableMultitThreading;

  static sf::Color foodColor;
  static sf::Color colonyColor;
  static sf::Color normalColor;

  static World *world;
  Configuration();
};
