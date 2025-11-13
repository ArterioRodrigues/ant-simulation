#include "ant.h"
#include "configuration.h"
#include "helper.h"
#include "world.h"

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

std::vector<PheromoneData> Ant::update(sf::Time deltaTime) {
  std::vector<PheromoneData> pheromonesToDeposit;
  sf::Vector2f antPosition = _sprite.getPosition();

  type = _foodCount ? AntType::Found : AntType::Searching;
  float colonyRadius = Configuration::world->getColonyEntity().circle.getRadius() * 2;

  if (type == AntType::Searching) {
    pheromonesToDeposit.push_back({antPosition, Pheromones::toFood});
    followPheromones();
    move(_movementDistance, _sprite.getRotation());

    std::vector entities = Configuration::world->getEntities(antPosition);
    for (auto food : entities) {
      sf::Vector2f foodPosition = food.circle.getPosition();
      float size = food.circle.getRadius() * 2;
      float foodRadius= food.circle.getRadius();
      float antRadius = 16;

      float dx = antPosition.x - foodPosition.x;
      float dy = antPosition.y - foodPosition.y;
      float distanceSquared = dx * dx + dy * dy;
      float collisionDistance = antRadius + foodRadius; 
      if (distanceSquared <= collisionDistance * collisionDistance) {
        _foodCount += 1;
        Configuration::world->decrementFood(food.id);
        break;
      }
    }
  } else {
    pheromonesToDeposit.push_back({antPosition, Pheromones::toHome});
    followPheromones();
    move(_movementDistance, _sprite.getRotation());

    if (antPosition.x >= _colonyPosition.x - colonyRadius && antPosition.x <= _colonyPosition.x + colonyRadius &&
        antPosition.y >= _colonyPosition.y - colonyRadius && antPosition.y <= _colonyPosition.y + colonyRadius) {
      _foodCount += Configuration::world->getColony()->getFoodCount();
      Configuration::world->getColony()->setFoodCount(_foodCount);
      _foodCount = 0;
    }
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

void Ant::setFoodCount(int foodCount) { _foodCount = foodCount; }
void Ant::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(_sprite, states);
  if (_foodCount) {
    target.draw(_food, states);
  }
}
