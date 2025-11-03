#include "ant.h"
#include "configuration.h"
#include "helper.h"
#include <SFML/System/Vector2.hpp>
#include <cstdint>

Ant::Ant(int movementDistance, sf::Vector2f position, sf::Vector2f colonyPosition)
    : _sprite(Configuration::textures.get(Configuration::Textures::Ant)) {
  _sprite.setOrigin({16, 16});
  _sprite.setPosition(position);
  _movementDistance = movementDistance;
  _foodCount = 0;
  _colonyPosition = colonyPosition;
  type = AntType::Searching;
}

void Ant::update(sf::Time deltaTime) {
  sf::Vector2f position = _sprite.getPosition();

  std::optional<Tile> eastTile =
      Configuration::world->getTile({position.x + Configuration::tileX, position.y});
  std::optional<Tile> westTile =
      Configuration::world->getTile({position.x - Configuration::tileX, position.y});
  std::optional<Tile> northTile =
      Configuration::world->getTile({position.x, position.y + Configuration::tileY});
  std::optional<Tile> southTile =
      Configuration::world->getTile({position.x, position.y - Configuration::tileY});

  std::optional<Tile> antTile = Configuration::world->getTile(position);

  if (antTile->type == TileType::Colony) {
    _foodCount = 0;
  }

  if (_foodCount) {
    type = AntType::Found;
    Direction toColony = getDirectionToward(_colonyPosition);

    if (randomNumberGenerator(0, 100) < 80) {
      move(toColony);
    } else {
      uint8_t eastWeight = eastTile.has_value() ? eastTile.value().shape.getFillColor().b : 255;
      uint8_t westWeight = westTile.has_value() ? westTile.value().shape.getFillColor().b : 255;
      uint8_t northWeight = northTile.has_value() ? northTile.value().shape.getFillColor().b : 255;
      uint8_t southWeight = southTile.has_value() ? southTile.value().shape.getFillColor().b : 255;

      int number = weightedRandomGenerator({eastWeight, westWeight, northWeight, southWeight});
      Direction direction = static_cast<Direction>(number);
      move(direction);
    }
  } else {
    type = AntType::Searching;

    uint8_t eastWeight = eastTile.has_value() ? eastTile.value().shape.getFillColor().g : 255;
    uint8_t westWeight = westTile.has_value() ? westTile.value().shape.getFillColor().g : 255;
    uint8_t northWeight = northTile.has_value() ? northTile.value().shape.getFillColor().g : 255;
    uint8_t southWeight = southTile.has_value() ? southTile.value().shape.getFillColor().g : 255;
    eastWeight = 256 - eastWeight;
    westWeight = 256 - westWeight;
    northWeight = 256 - northWeight;
    southWeight = 256 - southWeight;
    int number = weightedRandomGenerator({eastWeight, westWeight, northWeight, southWeight});
    Direction direction = static_cast<Direction>(number);
    move(direction);
  }
}

Direction Ant::getDirectionToward(sf::Vector2f target) {
  sf::Vector2f position = _sprite.getPosition();
  sf::Vector2f difference = target - position;

  if (std::abs(difference.x) > std::abs(difference.y)) {
    return difference.x > 0 ? Direction::East : Direction::West;
  } else {
    return difference.y > 0 ? Direction::South : Direction::North;
  }
}

void Ant::processEvent() {}

void Ant::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(_sprite, states);
}

sf::Vector2f Ant::getPosition() { return _sprite.getPosition(); }

void Ant::move(Direction direction) {
  sf::Vector2f position = _sprite.getPosition();

  switch (direction) {
  case Direction::West:
    if (position.x - _movementDistance < 0) return;
    _sprite.setPosition({position.x - _movementDistance, position.y});
    rotate(Direction::West);
    break;
  case Direction::East:
    if (position.x + _movementDistance > Configuration::windowX) return;
    _sprite.setPosition({position.x + _movementDistance, position.y});
    rotate(Direction::East);
    break;
  case Direction::South:
    if (position.y + _movementDistance > Configuration::windowY) return;
    _sprite.setPosition({position.x, position.y + _movementDistance});
    rotate(Direction::South);
    break;
  case Direction::North:
    if (position.y - _movementDistance < 0) return;
    _sprite.setPosition({position.x, position.y - _movementDistance});
    rotate(Direction::North);
    break;
  }
}

void Ant::rotate(Direction direction) {

  switch (direction) {
  case Direction::North:
    _sprite.setRotation(sf::degrees(0));
    break;
  case Direction::South:
    _sprite.setRotation(sf::degrees(180));
    break;
  case Direction::East:
    _sprite.setRotation(sf::degrees(90));
    break;
  case Direction::West:
    _sprite.setRotation(sf::degrees(270));
    break;
  }
}
void Ant::incrementFood() { _foodCount++; }
