#include "world.h"
#include "configuration.h"
#include "helper.h"
#include <SFML/System/Vector2.hpp>

int World::_id = 0;

World::World(int windowX, int windowY, int colonySize, int foodCount)
    : _toFoodPheromones(windowX / 10), _toHomePheromones(windowY / 10),
      _colony(colonySize,
              sf::Vector2f({float(randomNumberGenerator(0, windowX)), float(randomNumberGenerator(0, windowY))})) {
  _worldX = windowX;
  _worldY = windowY;
  _foodCount = foodCount;

  _deltaTime = sf::Time::Zero;

  sf::Vector2f colonyPosition = _colony.getPosition();

  _colonyEntity.circle.setRadius(30);
  _colonyEntity.circle.setOrigin({30, 30});
  _colonyEntity.circle.setFillColor(Configuration::colonyColor);
  _colonyEntity.circle.setPosition(colonyPosition);
  _colonyEntity.type = EntityTypes::Colony;
  _colonyEntity.id = _id++;

  _food = {};
  for (int i = 0; i < foodCount; i++) {
    sf::Vector2f circlePosition({float(randomNumberGenerator(0, windowX)), float(randomNumberGenerator(0, windowY))});
    for (int j = 0; j < 30; j++) {
      Entity food;
      food.circle.setRadius(Configuration::foodSize);
      food.circle.setFillColor(Configuration::foodColor);
      food.circle.setPosition(
          {circlePosition.x + randomNumberGenerator(-20, 20), circlePosition.y + randomNumberGenerator(-20, 20)});
      food.id = _id++;
      _entities.insert(circlePosition, food);
    }
  }
}

void World::setPhemromone(sf::Vector2f position, enum Pheromones type) {
  if (_toFoodPheromones.size() + _toHomePheromones.size() > Configuration::pheromoneLimit) {
    return;
  }

  // std::vector<Entity> &pheromones = _toFoodPheromones.getSubCellVector(position);
  // if (pheromones.size() > 10) {
  //   return;
  // }

  Entity pheromone;

  switch (type) {
  case Pheromones::toFood:
    pheromone.circle.setRadius(1);
    pheromone.circle.setFillColor(Configuration::foodColor);
    pheromone.circle.setPosition(position);
    pheromone.type = EntityTypes::Pheromones;
    pheromone.id = _id++;
    _toFoodPheromones.insert(position, pheromone);
    break;
  case Pheromones::toHome:
    pheromone.circle.setRadius(1);
    pheromone.circle.setFillColor(Configuration::colonyColor);
    pheromone.circle.setPosition(position);
    pheromone.type = EntityTypes::Pheromones;
    pheromone.id = _id++;
    _toHomePheromones.insert(position, pheromone);
    break;
  }
}

float World::getPheromoneStrength(sf::Vector2f position, enum Pheromones pheromoneType, int radius) {
  if (pheromoneType == Pheromones::toFood) {
    std::vector<Entity> pheromones = _toFoodPheromones.getNearby(position);
    for (auto pheromone : pheromones) {
      sf::Vector2f pheromonesPosition = pheromone.circle.getPosition();
      if (position.x > pheromonesPosition.x - radius && position.x < pheromonesPosition.x + radius &&
          position.y > pheromonesPosition.y - radius && position.y < pheromonesPosition.y + radius) {
        sf::Color color = pheromone.circle.getFillColor();
        return color.r + color.b + color.g;
      }
    }
  } else {
    std::vector<Entity> pheromones = _toHomePheromones.getNearby(position);
    for (auto pheromone : pheromones) {
      sf::Vector2f pheromonesPosition = pheromone.circle.getPosition();
      if (position.x > pheromonesPosition.x - radius && position.x < pheromonesPosition.x + radius &&
          position.y > pheromonesPosition.y - radius && position.y < pheromonesPosition.y + radius) {
        sf::Color color = pheromone.circle.getFillColor();
        return color.r + color.b + color.g;
      }
    }
  }
  return 0;
}

const std::vector<Entity> &World::getEntities() { return _entities.getVector(); }
const std::vector<Entity> &World::getToHomePheromones() { return _toHomePheromones.getVector(); }
const std::vector<Entity> &World::getToFoodPheromones() { return _toFoodPheromones.getVector(); }

void World::decrementFood(int id) {
  _entities.remove([id](Entity &entity) -> bool {
    if (entity.id == id) {
      return true;
    }
    return false;
  });
}

void World::update(sf::Time deltaTime) {
  _colony.update(deltaTime);
  _deltaTime += deltaTime;

  if (_deltaTime >= sf::seconds(1 / Configuration::frameRate * 20)) {
    _deltaTime = sf::Time::Zero;
    _toFoodPheromones.forEach([](Entity &entity) -> void {
      sf::Color color = entity.circle.getFillColor();
      color.r = std::min(255, color.r + 20);
      color.g = std::min(255, color.g + 20);
      color.b = std::min(255, color.b + 20);

      entity.circle.setFillColor(color);
    });

    _toFoodPheromones.remove([](Entity &entity) -> bool {
      sf::Color color = entity.circle.getFillColor();

      if (color.r == 255 && color.g == 255 && color.b == 255) {
        return true;
      }

      return false;
    });

    _toHomePheromones.forEach([](Entity &entity) -> void {
      sf::Color color = entity.circle.getFillColor();
      color.r = std::min(255, color.r + 20);
      color.g = std::min(255, color.g + 20);
      color.b = std::min(255, color.b + 20);

      entity.circle.setFillColor(color);
    });

    _toHomePheromones.remove([](Entity &entity) -> bool {
      sf::Color color = entity.circle.getFillColor();

      if (color.r == 255 && color.g == 255 && color.b == 255) {
        return true;
      }

      return false;
    });
  }
}

int World::getFoodCount() { return 0; }
class Colony World::getColony() { return _colony; }
Entity World::getColonyEntity() { return _colonyEntity; }
