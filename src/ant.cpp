#include "ant.h"
#include "configuration.h"
#include "helper.h"
#include "world.h"
#include <SFML/System/Vector2.hpp>

Ant::Ant(int movementDistance, sf::Vector2f position, sf::Vector2f colonyPosition) : _sprite(Configuration::textures.get(Configuration::Textures::Ant)) {
  type = AntType::Searching;
  sf::Texture texture = Configuration::textures.get(Configuration::Textures::Ant);
  sf::Vector2u size = texture.getSize();

  _sprite.setPosition(position);
  _sprite.setScale({0.5f, 0.5f});
  _sprite.setOrigin({size.x / 2.0f, size.y / 2.0f});
  _sprite.setRotation(sf::degrees(randomNumberGenerator(0, 360)));

  _foodCount = 0;

  _colonyPosition = colonyPosition;
  _foodPosition = sf::Vector2f({-1, -1});
  _movementDistance = std::ceil(movementDistance);
}

void Ant::update(sf::Time deltaTime) {
  sf::Vector2f spritePosition = _sprite.getPosition();
  type = _foodCount ? AntType::Found : AntType::Searching;
  std::optional<Tile> currentTile = Configuration::world->getTile(spritePosition);

  if (type == AntType::Searching) {
    if (currentTile.has_value() && currentTile.value().type == TileType::Food) {
      _foodCount += 1;
    }
    float angle = nextSearchAngle();
    _sprite.setRotation(sf::degrees(angle));
    move(_movementDistance, _sprite.getRotation());

  }

  else {
    std::optional<Tile> currentTile = Configuration::world->getTile(_sprite.getPosition());
    if (currentTile.has_value()) {
      if (currentTile.value().type == TileType::Colony) {
        _foodCount = 0;
      } else if (currentTile.value().type != TileType::Food) {
        Configuration::world->setTileType(spritePosition);
      }
    }

    Direction direction = getDirectionToward(_colonyPosition);
    float angle = getAngleFromDirection(direction);
    _sprite.setRotation(sf::degrees(angle));
    move(_movementDistance, _sprite.getRotation());
  }
}

float Ant::nextSearchAngle() {
  sf::Vector2f spritePosition = _sprite.getPosition();
  sf::Angle spriteAngle = _sprite.getRotation();
  std::optional<Tile> northTile = Configuration::world->getTile({spritePosition.x, spritePosition.y - Configuration::tileY});
  std::optional<Tile> northEastTile = Configuration::world->getTile({spritePosition.x + Configuration::tileX, spritePosition.y - Configuration::tileY});
  std::optional<Tile> eastTile = Configuration::world->getTile({spritePosition.x + Configuration::tileX, spritePosition.y});
  std::optional<Tile> southEastTile = Configuration::world->getTile({spritePosition.x + Configuration::tileX, spritePosition.y + Configuration::tileY});
  std::optional<Tile> southTile = Configuration::world->getTile({spritePosition.x, spritePosition.y + Configuration::tileY});
  std::optional<Tile> southWestTile = Configuration::world->getTile({spritePosition.x - Configuration::tileX, spritePosition.y + Configuration::tileY});
  std::optional<Tile> westTile = Configuration::world->getTile({spritePosition.x - Configuration::tileX, spritePosition.y});
  std::optional<Tile> northWestTile = Configuration::world->getTile({spritePosition.x - Configuration::tileX, spritePosition.y - Configuration::tileY});

  std::vector<std::optional<Tile>> tiles = {northTile, northEastTile, eastTile, southEastTile, southTile, southWestTile, westTile, northWestTile};
  const float alpha = 2.0f;
  const float explorationChancee = 0.01f;

  if (randomNumberGenerator(0, 100) < explorationChancee * 100) {
    return randomNumberGenerator(spriteAngle.asDegrees() - 60, spriteAngle.asDegrees() + 60);
  }

  std::vector<float> weights(8.0f, 0.0f);
  float totalWeight = 0.0f;

  for (int i = 0; i < 8; i++) {
    float pheromoneStrength = 0.0f;
    if (tiles[i].has_value() && tiles[i]->type != TileType::Normal) {
      pheromoneStrength = tiles[i]->shape.getFillColor().g;
    }
    weights[i] = std::pow(pheromoneStrength + 1.0f, alpha);
    totalWeight += weights[i];
  }

  float cumulativeWeight = 0.0f;
  Direction direction = getDirectionFromAngle(spriteAngle.asDegrees());
  std::vector<int> directions = {int(direction) - 2, int(direction) - 1, int(direction), int(direction) + 1, int(direction) + 2};

  for (int i = 0; i < 5; i++) {
    if (directions[i] == -1)
      directions[i] = 8;
    else if (directions[i] == 9)
      directions[i] = 1;
  }

  for (int i = 0; i < 5; i++) {
    cumulativeWeight += weights[directions[i]];
    if (cumulativeWeight > 750) {
      std::vector<float> selectedWeights = {weights[directions[0]], weights[directions[1]], weights[directions[2]], weights[directions[3]],
                                            weights[directions[4]]};
      Direction choice = static_cast<Direction>(weightedRandomGenerator<float>(selectedWeights));
      return getAngleFromDirection(choice);
    }
  }

  return _sprite.getRotation().asDegrees();
}

Direction Ant::getDirectionFromAngle(float angle) {
  angle = fmod(angle, 360.0f);
  if (angle < 0) angle += 360.0f;

  if ((angle >= 337.5f && angle < 360.0f) || (angle >= 0.0f && angle < 22.5f)) {
    return Direction::North;
  } else if (angle >= 22.5f && angle < 67.5f) {
    return Direction::NorthEast;
  } else if (angle >= 67.5f && angle < 112.5f) {
    return Direction::East;
  } else if (angle >= 112.5f && angle < 157.5f) {
    return Direction::SouthEast;
  } else if (angle >= 157.5f && angle < 202.5f) {
    return Direction::South;
  } else if (angle >= 202.5f && angle < 247.5f) {
    return Direction::SouthWest;
  } else if (angle >= 247.5f && angle < 292.5f) {
    return Direction::West;
  } else {
    return Direction::NorthWest;
  }
}

float Ant::getAngleFromDirection(Direction &direction) {
  float angle = 0.f;
  switch (direction) {
  case Direction::North:
    angle = randomNumberGenerator(315, 360) + randomNumberGenerator(0, 45);
    break;
  case Direction::NorthEast:
    angle = randomNumberGenerator(45, 90);
    break;
  case Direction::East:
    angle = randomNumberGenerator(90, 135);
    break;
  case Direction::SouthEast:
    angle = randomNumberGenerator(135, 180);
    break;
  case Direction::South:
    angle = randomNumberGenerator(180, 225);
    break;
  case Direction::SouthWest:
    angle = randomNumberGenerator(225, 270);
    break;
  case Direction::West:
    angle = randomNumberGenerator(270, 315);
    break;
  case Direction::NorthWest:
    angle = randomNumberGenerator(315, 360);
    break;
  }

  return angle;
}

bool Ant::move(int distance, sf::Angle angle) {
  sf::Vector2f spritePosition = _sprite.getPosition();

  float angleRadians = angle.asRadians();
  float sin = std::sin(angleRadians);
  float cos = std::cos(angleRadians);

  float xOffset = 0 * cos - distance * sin;
  float yOffset = 0 * sin + distance * cos;

  sf::Vector2f newPosition(spritePosition.x - xOffset, spritePosition.y - yOffset);

  if (newPosition.y - _movementDistance < 10) {
    _sprite.setRotation(sf::degrees(randomNumberGenerator(90, 270)));
  }
  if (newPosition.y + _movementDistance + 10 >= Configuration::windowY) {
    _sprite.setRotation(sf::degrees(randomNumberGenerator(0, 10) > 5 ? randomNumberGenerator(270, 360) : randomNumberGenerator(0, 90)));
  }
  if (newPosition.x - _movementDistance <= 10) {
    _sprite.setRotation(sf::degrees(randomNumberGenerator(0, 180)));
  }
  if (newPosition.x + _movementDistance + 10 >= Configuration::windowX) {
    _sprite.setRotation(sf::degrees(randomNumberGenerator(180, 360)));
  }
  _sprite.setPosition(newPosition);
  return true;
}

Direction Ant::getDirectionToward(sf::Vector2f target) {
  sf::Vector2f position = _sprite.getPosition();
  sf::Vector2f difference = target - position;

  float absX = std::abs(difference.x);
  float absY = std::abs(difference.y);
  float ratio = absX > absY ? absY / absX : absX / absY;

  const float diagonalThreshold = 0.4f;

  if (ratio > diagonalThreshold) {
    if (difference.x > 0 && difference.y < 0)
      return Direction::NorthEast;
    else if (difference.x < 0 && difference.y < 0)
      return Direction::NorthWest;
    else if (difference.x > 0 && difference.y > 0)
      return Direction::SouthEast;
    else
      return Direction::SouthWest;

  } else {
    if (absX > absY)
      return difference.x > 0 ? Direction::East : Direction::West;
    else
      return difference.y > 0 ? Direction::South : Direction::North;
  }
}

int Ant::getFoodCount() { return _foodCount; }

sf::Vector2f Ant::getPosition() { return _sprite.getPosition(); }

void Ant::processEvent() {}
void Ant::setFoodCount(int foodCount) { _foodCount = foodCount; }
void Ant::draw(sf::RenderTarget &target, sf::RenderStates states) const { target.draw(_sprite, states); }

//
//   sf::Vector2f position = _sprite.getPosition();
//
//   std::optional<Tile> northTile = Configuration::world->getTile({position.x, position.y + Configuration::tileY});
//   std::optional<Tile> northEastTile = Configuration::world->getTile({position.x + Configuration::tileX, position.y + Configuration::tileY});
//   std::optional<Tile> northWestTile = Configuration::world->getTile({position.x - Configuration::tileX, position.y + Configuration::tileY});
//
//   std::optional<Tile> southTile = Configuration::world->getTile({position.x, position.y - Configuration::tileY});
//   std::optional<Tile> southEastTile = Configuration::world->getTile({position.x + Configuration::tileX, position.y - Configuration::tileY});
//   std::optional<Tile> southWestTile = Configuration::world->getTile({position.x - Configuration::tileX, position.y - Configuration::tileY});
//
//   std::optional<Tile> eastTile = Configuration::world->getTile({position.x + Configuration::tileX, position.y});
//   std::optional<Tile> westTile = Configuration::world->getTile({position.x - Configuration::tileX, position.y});
//
//   int eastWeight = eastTile.has_value() ? eastTile.value().shape.getFillColor().b + eastTile.value().shape.getFillColor().r : 0;
//   int westWeight = westTile.has_value() ? westTile.value().shape.getFillColor().b + westTile.value().shape.getFillColor().r : 0;
//   int northWeight = northTile.has_value() ? northTile.value().shape.getFillColor().b + northTile.value().shape.getFillColor().r : 0;
//   int southWeight = southTile.has_value() ? southTile.value().shape.getFillColor().b + southTile.value().shape.getFillColor().r : 0;
//   int northEastWeight = northEastTile.has_value() ? northEastTile.value().shape.getFillColor().b + northEastTile.value().shape.getFillColor().r : 0;
//   int northWestWeight = northWestTile.has_value() ? northWestTile.value().shape.getFillColor().b + northWestTile.value().shape.getFillColor().r : 0;
//   int southEastWeight = southEastTile.has_value() ? southEastTile.value().shape.getFillColor().b + southEastTile.value().shape.getFillColor().r : 0;
//   int southWestWeight = southWestTile.has_value() ? southWestTile.value().shape.getFillColor().b + southWestTile.value().shape.getFillColor().r : 0;
//
//   if (eastWeight >= 255 && westWeight >= 255 && northWeight >= 255 && southWeight >= 255 && northEastWeight >= 255 && northWestWeight >= 255 &&
//       southEastWeight >= 255 && southWestWeight >= 255) {
//     if (position.y - _movementDistance < 10) {
//       return randomNumberGenerator(90, 270);
//     }
//     if (position.y + _movementDistance + 10 >= Configuration::windowY) {
//       return randomNumberGenerator(0, 10) > 5 ? randomNumberGenerator(270, 360) : randomNumberGenerator(0, 90);
//     }
//     if (position.x - _movementDistance <= 10) {
//       return randomNumberGenerator(0, 180);
//     }
//     if (position.x + _movementDistance + 10 >= Configuration::windowX) {
//       return randomNumberGenerator(180, 360);
//     }
//   }
//
//   else {
//     std::vector<int> weights = {510 - northWeight, 510 - northEastWeight, 510 - eastWeight, 510 - southEastWeight,
//                                 510 - southWeight, 510 - southWestWeight, 510 - westWeight, 510 - northWestWeight};
//     Direction direction = static_cast<Direction>(weightedRandomGenerator(weights));
//     float angle = getAngleFromDirection(direction);
//     float spriteAngle = _sprite.getRotation().asDegrees();
//     if (_currentDirection != direction && (_currentDirection - direction) > 2 || _currentDirection == Direction::North && (8 - direction) > 2) {
//       if (angle < spriteAngle) {
//         return randomNumberGenerator(angle, angle + 10);
//       } else {
//         return randomNumberGenerator(angle - 10, angle);
//       }
//     }
//   }
//
//   return _sprite.getRotation().asDegrees();
// }
