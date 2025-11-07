#include "world.h"
#include "configuration.h"
#include "helper.h"

int World::_id = 0;

World::World(int windowX, int windowY, int colonySize, int foodCount)
    : _colony(colonySize,
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

  for (int i = 0; i < foodCount; i++) {
    sf::Vector2f circlePosition({float(randomNumberGenerator(0, windowX)), float(randomNumberGenerator(0, windowY))});
    for (int j = 0; j < 30; j++) {
      Entity food;
      food.circle.setRadius(Configuration::foodSize);
      food.circle.setFillColor(Configuration::foodColor);
      food.circle.setPosition(
          {circlePosition.x + randomNumberGenerator(-20, 20), circlePosition.y + randomNumberGenerator(-20, 20)});
      food.id = _id++;
      _entities.emplace(food.id, food);
    }
  }
}

void World::setPhemromone(sf::Vector2f position, enum Pheromones type) {
  Entity pheromone;

  switch (type) {
  case Pheromones::toFood:
    pheromone.circle.setRadius(1);
    pheromone.circle.setFillColor(Configuration::foodColor);
    pheromone.circle.setPosition(position);
    pheromone.type = EntityTypes::Pheromones;
    pheromone.id = _id++;
    _toFoodPheromones.emplace(pheromone.id, pheromone);
    break;
  case Pheromones::toHome:
    pheromone.circle.setRadius(1);
    pheromone.circle.setFillColor(Configuration::colonyColor);
    pheromone.circle.setPosition(position);
    pheromone.type = EntityTypes::Pheromones;
    pheromone.id = _id++;
    _toHomePheromones.emplace(pheromone.id, pheromone);
    break;
  }
}

float World::getPheromoneStrength(sf::Vector2f position, enum Pheromones pheromone, int radius) {
  if (pheromone == Pheromones::toFood) {
    for (auto pheromones : _toFoodPheromones) {
      sf::Vector2f pheromonesPosition = pheromones.second.circle.getPosition();
      if (position.x > pheromonesPosition.x - radius && position.x < pheromonesPosition.x + radius &&
          position.y > pheromonesPosition.y - radius && position.y < pheromonesPosition.y + radius) {
        sf::Color color = pheromones.second.circle.getFillColor();
        return color.r + color.b + color.g;
      }
    }
  } else {
    for (auto pheromones : _toHomePheromones) {
      sf::Vector2f pheromonesPosition = pheromones.second.circle.getPosition();
      if (position.x > pheromonesPosition.x - radius && position.x < pheromonesPosition.x + radius &&
          position.y > pheromonesPosition.y - radius && position.y < pheromonesPosition.y + radius) {
        sf::Color color = pheromones.second.circle.getFillColor();
        return color.r + color.b + color.g;
      }
    }
   

  return 0;
}

std::unordered_map<int, Entity> World::getEntities() { return _entities; }
std::unordered_map<int, Entity> World::getToHomePheromones() { return _toHomePheromones; }
std::unordered_map<int, Entity> World::getToFoodPheromones() { return _toFoodPheromones; }

void World::decrementFood(int index) { _entities.erase(index); }
void World::update(sf::Time deltaTime) {
  _colony.update(deltaTime);
  _deltaTime = sf::Time::Zero;

  std::vector<int> toRemove = {};

  for (auto pheromones : _toFoodPheromones) {
    int index = pheromones.first;
    sf::Color color = pheromones.second.circle.getFillColor();
    color.r = std::min(255, color.r + 1);
    color.g = std::min(255, color.g + 1);
    color.b = std::min(255, color.b + 1);

    _toFoodPheromones[index].circle.setFillColor(color);

    if (color.r == 255 && color.g == 255 && color.b == 255) {
      toRemove.push_back(index);
    }
  }

  for (auto pheromones : _toHomePheromones) {
    int index = pheromones.first;
    sf::Color color = pheromones.second.circle.getFillColor();
    color.r = std::min(255, color.r + 1);
    color.g = std::min(255, color.g + 1);
    color.b = std::min(255, color.b + 1);

    _toFoodPheromones[index].circle.setFillColor(color);

    if (color.r == 255 && color.g == 255 && color.b == 255) {
      toRemove.push_back(index);
    }
  }

  for (auto index : toRemove) {
    _toFoodPheromones.erase(index);
  }
}

int World::getFoodCount() { return 0; }
class Colony World::getColony() { return _colony; }
Entity World::getColonyEntity() { return _colonyEntity; }
