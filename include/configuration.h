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

  static World *world;
  static unsigned int windowX;
  static unsigned int windowY;
  static double frameRate;
  static float tileX;
  static float tileY;
  static int colonySize; 

  Configuration();
};
