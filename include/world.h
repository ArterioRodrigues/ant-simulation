#pragma once
#include "pch.h"

struct Tile {
  sf::RectangleShape shape;
  int index;
};

class World {
public:
  World(int windowX, int windowY, float tileX, float tileY);
  std::vector<std::vector<Tile>> getTiles();

private:
  int _x;
  int _y;
  std::vector<std::vector<Tile>> _tiles;
};
