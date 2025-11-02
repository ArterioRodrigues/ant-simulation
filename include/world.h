#pragma once
#include "pch.h"

enum TileType {
  Normal,
  Green,
};

struct Tile {
  sf::RectangleShape shape;
  int index;
  TileType type; 
};

class World {
public:
  World(int windowX, int windowY, float tileX, float tileY);
  std::vector<std::vector<Tile>> getTiles();

  int collisionIndex(sf::Vector2f position);
  void colorTile(sf::Vector2f position, sf::Color color, TileType type = TileType::Green);
  void update(sf::Time deltaTime);

private:
  int _worldX;
  int _worldY;
  float _tileX;
  float _tileY;
  std::vector<std::vector<Tile>> _tiles;
};
