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
  static unsigned int _windowX;
  static unsigned int _windowY;
  static double _frameRate;
  static float _tileX;
  static float _tileY;
  Configuration();
};
