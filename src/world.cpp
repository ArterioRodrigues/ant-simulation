#include "world.h"
#include "configuration.h"
#include "helper.h"
#include <SFML/System/Vector2.hpp>
#include <optional>

World::World(int windowX, int windowY, float tileX, float tileY, int foodCount) {
  _tileX = tileX;
  _tileY = tileY;
  _worldX = windowX;
  _worldY = windowY;
  _foodCount = foodCount;
  for (uint8_t i = 0; i < windowX / tileX; i++) {
    std::vector<Tile> row;
    for (uint8_t j = 0; j < windowY / tileY; j++) {
      Tile tile;

      tile.shape.setSize(sf::Vector2f{tileX, tileY});
      tile.shape.setPosition(sf::Vector2f(i * tileX, j * tileY));
      tile.shape.setFillColor({255, 255, 255});

      tile.type = TileType::Normal;
      tile.index = (i * windowX / tileX) + j;

      // tile.shape.setOutlineColor({0, 0, 0});
      // tile.shape.setOutlineThickness(1.0f);
      row.push_back(tile);
    }
    _tiles.push_back(row);
  }

  for (int i = 0; i < Configuration::foodCount; i++) {
    float x = float(randomNumberGenerator(0, windowX));
    float y = float(randomNumberGenerator(0, windowY));
    setTileType({x, y}, TileType::Food);
  }
}
std::vector<std::vector<Tile>> World::getTiles() { return _tiles; }

int World::collisionIndex(sf::Vector2f position) {
  return int(position.x / _tileX * _worldX / _tileX) + int(position.y / _tileY);
}

std::optional<Tile> World::getTile(sf::Vector2f position) {

  int x = position.x / _tileX;
  int y = position.y / _tileY;
  if (x >= _tiles.size() || y >= _tiles.size()) {
    return std::nullopt;
  }

  return _tiles[x][y];
}
void World::setTileType(sf::Vector2f position, TileType type) {
  int x = position.x / _tileX;
  int y = position.y / _tileY;
  switch (type) {
  case TileType::Food:
    _tiles[x][y].shape.setFillColor(Configuration::foodColor);
    _tiles[x][y].type = type;
    break;
  case TileType::Normal:
    _tiles[x][y].shape.setFillColor(Configuration::normalColor);
    _tiles[x][y].type = type;
    break;
  case TileType::Colony:
    _tiles[x][y].shape.setFillColor(Configuration::colonyColor);
    _tiles[x][y].type = type;
    break;
  case TileType::Pheromone:
    _tiles[x][y].shape.setFillColor(Configuration::pheromoneColor);
    _tiles[x][y].type = type;
    break;
  }
}

TileType World::getTileType(sf::Vector2f position) {
  int x = position.x / _tileX;
  int y = position.y / _tileY;

  return _tiles[x][y].type;
}

void World::decrementFood() { _foodCount = std::max(0, _foodCount - 1); }
void World::update(sf::Time deltaTime) {
   for (int i = 0; i < std::max(0, Configuration::foodCount - _foodCount); i++) {
     float x = float(randomNumberGenerator(0, _worldX));
     float y = float(randomNumberGenerator(0, _worldY));
    setTileType({x, y}, TileType::Food);
   }
   _foodCount = Configuration::foodCount;

  for (uint8_t i = 0; i < _tiles.size(); i++) {
    for (uint8_t j = 0; j < _tiles[0].size(); j++) {

      if (_tiles[i][j].type == TileType::Pheromone) {
        sf::Color color = _tiles[i][j].shape.getFillColor();
        color.r = std::min(255, color.r + 2);
        color.g = std::min(255, color.g + 2);
        _tiles[i][j].shape.setFillColor(color);

        if (color.r >= 255 && color.g >= 255 && color.b >= 255) {
          _tiles[i][j].type = TileType::Normal;
        }
      }
    }
  }
}
