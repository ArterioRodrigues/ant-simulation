#include "world.h"

World::World(int windowX, int windowY, float tileX, float tileY) {
  _tileX = tileX;
  _tileY = tileY;
  _worldX = windowX;
  _worldY = windowY;
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
}
std::vector<std::vector<Tile>> World::getTiles() { return _tiles; }

int World::collisionIndex(sf::Vector2f position) {
  return int(position.x / _tileX * _worldX / _tileX) + int(position.y / _tileY);
}

void World::colorTile(sf::Vector2f position, sf::Color color, TileType type) {
  _tiles[position.x / _tileX][position.y / _tileY].shape.setFillColor(color);
  _tiles[position.x / _tileX][position.y / _tileY].type = type; 
}
void World::update(sf::Time deltaTime) {
  for (uint8_t i = 0; i < _tiles.size(); i++) {
    for (uint8_t j = 0; j < _tiles[0].size(); j++) {
      if (_tiles[i][j].type == TileType::Green) {
        sf::Color color = _tiles[i][j].shape.getFillColor();
        color.r = std::min(255, color.r + 10);
        color.b = std::max(255, color.b + 10);
        _tiles[i][j].shape.setFillColor(color);
      }
    }
  }
}
