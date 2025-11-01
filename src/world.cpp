#include "world.h"

World::World(int windowX, int windowY, float tileX, float tileY) {
  for (uint8_t i = 0; i < windowX / tileX; i++) {
    std::vector<Tile> row;
    for (uint8_t j = 0; j < windowY / tileY; j++) {
      // uint8_t colorChannel1 = int(i * tileX * 0.5) % 255;
      // uint8_t colorChannel2 = int(j * tileY * 0.5) % 255;

      Tile tile;
      tile.shape.setSize(sf::Vector2f{tileX, tileY});
      tile.shape.setPosition(sf::Vector2f(i * tileX, j * tileY));
      tile.shape.setFillColor({255, 255, 255});
      tile.index = (i * windowX/ tileX) + j; 
      //tile.shape.setOutlineColor({0, 0, 0});
      //tile.shape.setOutlineThickness(1.0f);
      row.push_back(tile);
    }
    _tiles.push_back(row);
  }
}
std::vector<std::vector<Tile>> World::getTiles() { return _tiles; }
