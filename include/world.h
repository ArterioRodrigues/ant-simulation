#pragma once
#include "pch.h"

enum TileType {
  Normal,
  Pheromone,
  SearchingPheromone,
  Food,
  Colony,
};

struct Tile {
  sf::RectangleShape shape;
  int index;
  TileType type;
};

class World {
public:
  World(int windowX, int windowY, float tileX, float tileY, int foodCount = 5);
  std::vector<std::vector<Tile>> getTiles();

  int collisionIndex(sf::Vector2f position);
  void setTileType(sf::Vector2f position, TileType type = TileType::Pheromone);
  void update(sf::Time deltaTime);
  void decrementFood();
  TileType getTileType(sf::Vector2f position);

  std::optional<Tile> getTile(sf::Vector2f position);

private:
  int _worldX;
  int _worldY;
  int _foodCount;
  float _tileX;
  float _tileY;
  sf::Time _deltaTime;
  std::vector<std::vector<Tile>> _tiles;
};
