#include "ant.h"
#include "configuration.h"
#include "helper.h"
#include "world.h"
#include <SFML/System/Angle.hpp>

Ant::Ant(int movementDistance, sf::Vector2f position, sf::Vector2f colonyPosition)
    : _sprite(Configuration::textures.get(Configuration::Textures::Ant)) {
  type = AntType::Searching;

  _weight = 0;
  _foodCount = 0;
  _deltaTime = sf::Time::Zero;
  sf::Angle rotate = sf::degrees(randomNumberGenerator(0, 360));

  _sprite.setOrigin({16, 16});
  _sprite.setScale({0.5, 0.5});
  _sprite.setPosition(position);
  _sprite.rotate(rotate);

  _food.setRadius(Configuration::foodSize);
  _food.setFillColor(Configuration::foodColor);
  _food.setOrigin({float(Configuration::foodSize), float(Configuration::foodSize)});
  _food.setPosition(position);

  _movementDistance = movementDistance;
  _colonyPosition = colonyPosition;
  _lastDirection = getDirectionFromAngle(rotate.asDegrees());
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

std::vector<PheromoneData> Ant::update(sf::Time deltaTime) {

  std::vector<PheromoneData> pheromonesToDeposit;
  sf::Vector2f antPosition = _sprite.getPosition();

  type = _foodCount ? AntType::Found : AntType::Searching;
  float colonyRadius = Configuration::world->getColonyEntity().circle.getRadius() * 2;

  _stuckCounter++;
  bool shouldDeposit = (_stuckCounter % 5 == 0);

  if (type == AntType::Searching) {
    pheromonesToDeposit.push_back({antPosition, Pheromones::toFood});
    followPheromones();
    move(_movementDistance, _sprite.getRotation());

    std::vector entities = Configuration::world->getEntities(antPosition);
    for (auto food : entities) {
      sf::Vector2f position = food.circle.getPosition();
      float size = food.circle.getRadius() * 2;

      if (antPosition.x >= position.x - size && antPosition.x <= position.x + size &&
          antPosition.y >= position.y - size && antPosition.y <= position.y + size) {
        _foodCount += 1;
        break;
      }
    }
  } else {
    pheromonesToDeposit.push_back({antPosition, Pheromones::toHome});
    followPheromones();
    move(_movementDistance, _sprite.getRotation());
  }

  if (antPosition.x >= _colonyPosition.x - colonyRadius && antPosition.x <= _colonyPosition.x + colonyRadius &&
      antPosition.y >= _colonyPosition.y - colonyRadius && antPosition.y <= _colonyPosition.y + colonyRadius) {
    _foodCount = 0;
  }

  return pheromonesToDeposit;
}

bool Ant::followPheromones() {
  int index = 0;
  int numSamples = 32;
  int searchRadius = _movementDistance * 10;

  float maxStrength = 0;

  enum Pheromones pheromone = _foodCount ? Pheromones::toFood : Pheromones::toHome;

  Direction currentDirection;

  sf::Vector2f antPosition = _sprite.getPosition();
  sf::Vector2f bestDirection = antPosition;

  std::vector<std::pair<sf::Vector2f, float>> samples;

  for (int i = 0; i < numSamples; i++) {
    float angle = (360.0f / numSamples) * i;
    float radians = angle * 3.14159f / 180.0f;

    sf::Vector2f samplePosition(antPosition.x + searchRadius * std::cos(radians),
                                antPosition.y + searchRadius * std::sin(radians));

    float strength = Configuration::world->getPheromoneStrength(samplePosition, pheromone);
    samples.push_back({samplePosition, strength});
  }

  for (const auto &sample : samples) {
    if (sample.second > maxStrength) {
      currentDirection = getDirectionFromAngle((360.0f / numSamples) * index);
      maxStrength = sample.second;
      bestDirection = sample.first;
    }
    index++;
  }
  if (maxStrength > 40.0f) {
    sf::Vector2f direction = bestDirection - antPosition;
    float targetAngle = std::atan2(direction.y, direction.x) * 180.0f / 3.14159f;

    targetAngle += randomNumberGenerator(-10, 10);
    _sprite.setRotation(sf::degrees(90 + targetAngle));
  }

  return true;
}
// bool Ant::followPheromones() {
//   sf::Vector2f antPosition = _sprite.getPosition();
//   float antRotation = _sprite.getRotation().asDegrees();
//   int searchRadius = _movementDistance * 3;
//
//   enum Pheromones pheromone = _foodCount ? Pheromones::toHome : Pheromones::toFood;
//
//   std::vector<int> angleOffsets = {-270, -180, -90, -45, 0, 45, 90, 180, 270};
//
//   float maxStrength = 0.0f;
//   float bestAngle = antRotation;
//
//   for (int offset : angleOffsets) {
//     float checkAngle = antRotation + offset;
//     float radians = checkAngle * 3.14159f / 180.0f;
//
//     sf::Vector2f samplePosition(antPosition.x + searchRadius * std::cos(radians),
//                                 antPosition.y + searchRadius * std::sin(radians));
//
//     float strength = Configuration::world->getPheromoneStrength(samplePosition, pheromone);
//
//     if (strength > maxStrength) {
//       maxStrength = strength;
//       bestAngle = checkAngle;
//     }
//   }
//
//   if (maxStrength > 1.0f) {
//     float randomVariation = randomNumberGenerator(-15, 15);
//     _sprite.setRotation(sf::degrees(bestAngle + randomVariation));
//     return true;
//   } else {
//     float randomTurn = randomNumberGenerator(-30, 30);
//     _sprite.setRotation(sf::degrees(antRotation + randomTurn));
//   }
//
//   return false;
// }

// bool Ant::followPheromones() {
//   sf::Vector2f antPosition = _sprite.getPosition();
//   sf::Vector2f bestDirection = antPosition;
//
//   int searchRadius = _movementDistance * 3;
//   float antRotation = _sprite.getRotation().asDegrees();
//   enum Pheromones pheromone = _foodCount ? Pheromones::toHome : Pheromones::toFood;
//
//   std::vector<int> direction = {getDirectionFromAngle(antRotation - 45), getDirectionFromAngle(antRotation),
//                                 getDirectionFromAngle(antRotation + 45)};
//   static thread_local std::random_device rd;
//   static thread_local std::mt19937 gen(rd());
//   std::shuffle(direction.begin(), direction.end(), gen);
//   float maxStrength;
//   int move = 0;
//
//   for (int i : direction) {
//     float angle = (360.0f / 8) * i;
//     float radians = angle * 3.14159f / 180.0f;
//
//     sf::Vector2f samplePosition((antPosition.x + searchRadius) * std::cos(radians),
//                                 (antPosition.y + searchRadius) * std::sin(radians));
//     float strength = Configuration::world->getPheromoneStrength(samplePosition, pheromone);
//     if (strength > maxStrength) {
//       maxStrength = strength;
//       bestDirection = samplePosition;
//       move = i;
//     }
//   }
//
//   if (maxStrength > 100.0f) {
//     _sprite.setRotation(sf::degrees(getAngleFromDirection(static_cast<Direction>(move))));
//     return true;
//   }
//   return false;
// }

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
    _sprite.setRotation(
        sf::degrees(randomNumberGenerator(0, 10) > 5 ? randomNumberGenerator(270, 360) : randomNumberGenerator(0, 90)));
  }
  if (newPosition.x - _movementDistance <= 10) {
    _sprite.setRotation(sf::degrees(randomNumberGenerator(0, 180)));
  }
  if (newPosition.x + _movementDistance + 10 >= Configuration::windowX) {
    _sprite.setRotation(sf::degrees(randomNumberGenerator(180, 360)));
  }
  _sprite.setPosition(newPosition);
  _food.setPosition(newPosition);

  return true;
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

int Ant::getFoodCount() { return _foodCount; }
sf::Vector2f Ant::getPosition() { return _sprite.getPosition(); }

void Ant::processEvent() {}
void Ant::setFoodCount(int foodCount) { _foodCount = foodCount; }
void Ant::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(_sprite, states);
  if (_foodCount) {
    target.draw(_food, states);
  }
}
float Ant::getAngleFromDirection(Direction direction) {
  switch (direction) {
  case Direction::North:
    return randomNumberGenerator(0, 100) > 50 ? randomNumberGenerator(337.5f, 360.0f)
                                              : randomNumberGenerator(0.0f, 22.5f);
  case Direction::NorthEast:
    return randomNumberGenerator(22.5f, 67.5f);

  case Direction::East:
    return randomNumberGenerator(67.5f, 112.5f);

  case Direction::SouthEast:
    return randomNumberGenerator(112.5f, 157.5f);

  case Direction::South:
    return randomNumberGenerator(157.5f, 202.5f);

  case Direction::SouthWest:
    return randomNumberGenerator(202.5f, 247.5f);

  case Direction::West:
    return randomNumberGenerator(247.5f, 292.5f);

  case Direction::NorthWest:
    return randomNumberGenerator(292.5f, 337.5f);

  default:
    return 0.0f;
  }
}

//
// std::vector<PheromoneData> Ant::update(sf::Time deltaTime) {
//   std::vector<PheromoneData> pheromonesToDeposit;
//   sf::Vector2f antPosition = _sprite.getPosition();
//
//   type = _foodCount ? AntType::Found : AntType::Searching;
//
//   if (_foodCount) {
//     pheromonesToDeposit.push_back({antPosition, Pheromones::toHome});
//   } else {
//     pheromonesToDeposit.push_back({antPosition, Pheromones::toFood});
//   }
//
//   if (type == AntType::Searching) {
//     followPheromones();
//     move(_movementDistance, _sprite.getRotation());
//     std::vector entities = Configuration::world->getEntities(antPosition);
//     for (auto entity : entities) {
//       sf::Vector2f position = entity.circle.getPosition();
//       if (antPosition.x > position.x - Configuration::foodSize &&
//           antPosition.x < position.x + Configuration::foodSize &&
//           antPosition.y > position.y - Configuration::foodSize &&
//           antPosition.y < position.y + Configuration::foodSize) {
//         _foodCount += 1;
//       }
//     }
//   } else {
//     followPheromones();
//     move(_movementDistance, _sprite.getRotation());
//   }
//   float colonyRadius = Configuration::world->getColonyEntity().circle.getRadius();
//
//   if (antPosition.x >= _colonyPosition.x - colonyRadius && antPosition.x <= _colonyPosition.x + colonyRadius &&
//       antPosition.y >= _colonyPosition.y - colonyRadius && antPosition.y <= _colonyPosition.y + colonyRadius) {
//     _foodCount = 0;
//   }
//
//   return pheromonesToDeposit;
// }
// _recentPositions.push_back(antPosition);
// if (_recentPositions.size() > 10) {
//   float area = 50;
//   float positionsInArea = 0;

//   for (int i = 1; i < _recentPositions.size(); i++) {
//     if (_recentPositions[i].x < antPosition.x + area && _recentPositions[i].x > antPosition.x - area &&
//         _recentPositions[i].y < antPosition.y + area && _recentPositions[i].y > antPosition.y - area) {
//       positionsInArea++;
//     }
//   }

//   if (positionsInArea < 5) {
//     _stuckCounter++;
//      move(_movementDistance, _sprite.getRotation());
//   } else if (!_stuckCounter) {
//     _recentPositions.erase(_recentPositions.begin());
//   }
// }
