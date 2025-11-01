#include "ant.h"
#include "configuration.h"
#include "helper.h"
#include <SFML/System/Vector2.hpp>

Ant::Ant(int movementDistance)
    : _sprite(Configuration::textures.get(Configuration::Textures::Ant)) {
  _sprite.setOrigin({16, 16});
  _sprite.setPosition({12.5, 12.5});
  _movementDistance = movementDistance;
}

void Ant::update(sf::Time deltaTime) {

  int number = randomNumberGenerator(0, 4);
  Direction direction = static_cast<Direction>(number);
  move(direction);
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
    if (position.x + _movementDistance > Configuration::_windowX) return;
    _sprite.setPosition({position.x + _movementDistance, position.y});
    rotate(Direction::East);
    break;
  case Direction::South:
    if (position.y + _movementDistance > Configuration::_windowY) return;
    _sprite.setPosition({position.x, position.y + _movementDistance});
    rotate(Direction::South);
    break;
  case Direction::North:
    if (position.y - _movementDistance < 0) return;
    _sprite.setPosition({position.y, position.y - _movementDistance});
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
