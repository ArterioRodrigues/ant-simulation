#include "world.h"
#include "configuration.h"
#include "helper.h"

int World::_id = 0;

World::World(int windowX, int windowY, int colonySize, int foodCount) : _toFoodPheromones(), _toHomePheromones() {
  _colony = new class Colony(colonySize, sf::Vector2f({float(randomNumberGenerator(100, windowX - 100)),
                                                       float(randomNumberGenerator(100, windowY - 100))}));

  _worldX = windowX;
  _worldY = windowY;
  _foodCount = foodCount;

  _deltaTime = sf::Time::Zero;

  sf::Vector2f colonyPosition = _colony->getPosition();
  _colonyEntity.circle.setRadius(30);
  _colonyEntity.circle.setOrigin({30, 30});
  _colonyEntity.circle.setFillColor(Configuration::colonyColor);
  _colonyEntity.circle.setPosition(colonyPosition);
  _colonyEntity.type = EntityTypes::Colony;
  _colonyEntity.id = _id++;

  for (int i = 0; i < foodCount; i++) {
    sf::Vector2f circlePosition(
        {randomNumberGenerator(10.0f, windowX - 10.0f), randomNumberGenerator(10.0f, windowY - 10.0f)});
    Entity food;
    food.circle.setRadius(30);
    food.circle.setOrigin({30, 30});
    food.circle.setFillColor(Configuration::foodColor);
    food.circle.setPosition(circlePosition);
    food.id = _id++;
    food.health = 100;
    _entities.insert(circlePosition, food);
  }
}

void World::batchInsertPheromones(const std::vector<PheromoneData> &pheromones) {
  std::lock_guard<std::mutex> lock(_pheromoneMutex);
  for (const auto &data : pheromones) {
    if (_toFoodPheromones.getSubCellVector(data.position).size() +
            _toHomePheromones.getSubCellVector(data.position).size() >
        Configuration::pheromoneLimit) {
      continue;
    }
    Entity pheromone;
    pheromone.circle.setRadius(1);
    pheromone.circle.setFillColor(data.type == Pheromones::toFood ? Configuration::foodColor
                                                                  : Configuration::colonyColor);
    pheromone.circle.setPosition(data.position);
    pheromone.type = EntityTypes::Pheromones;
    pheromone.id = _id++;

    if (data.type == Pheromones::toFood) {
      _toFoodPheromones.insert(data.position, pheromone);
    } else {
      _toHomePheromones.insert(data.position, pheromone);
    }
  }
}

void World::setPhemromone(sf::Vector2f position, enum Pheromones type) {
  std::vector<PheromoneData> batch = {{position, type}};
  batchInsertPheromones(batch);
}

float World::getPheromoneStrength(sf::Vector2f position, enum Pheromones pheromoneType) {
  double result = 0;
  if (pheromoneType == Pheromones::toFood) {
    std::vector<Entity> pheromones = _toFoodPheromones.getSubCellVector(position);
    sf::Vector2f colonyPosition = _colony->getPosition();

    for (auto pheromone : pheromones) {
      sf::Color color = pheromone.circle.getFillColor();
      float strength = (color.r + color.b) / 4.0f;
      result = std::max(result, (double)strength);
    }

    if (_toFoodPheromones.getCell(position) == _toFoodPheromones.getCell(colonyPosition)) result += 500;

  } else {
    std::vector<Entity> pheromones = _toHomePheromones.getSubCellVector(position);
    std::vector<Entity> food = _entities.getSubCellVector(position);

    for (auto pheromone : pheromones) {
      sf::Color color = pheromone.circle.getFillColor();
      float strength = color.g / 2.0f;
      result = std::max(result, (double)strength);
    }

    if (food.size() > 0) {
      result += 500;
    }
  }

  return result;
}

const std::vector<Entity> &World::getEntities() {
  std::lock_guard<std::mutex> lock(_foodMutex);
  return _entities.getVector();
}

const std::vector<Entity> World::getEntities(sf::Vector2f position) {
  std::lock_guard<std::mutex> lock(_foodMutex);
  return _entities.getSubCellVector(position);
}
const std::vector<Entity> &World::getToHomePheromones() { return _toHomePheromones.getVector(); }
const std::vector<Entity> &World::getToFoodPheromones() { return _toFoodPheromones.getVector(); }

const std::vector<Entity> World::getToHomePheromones(sf::Vector2f position) {
  return _toHomePheromones.getSubCellVector(position);
}
const std::vector<Entity> World::getToFoodPheromones(sf::Vector2f position) {
  return _toFoodPheromones.getSubCellVector(position);
}

void World::decrementFood(int id) {
  bool shouldRemove = false;

  _entities.forEach([id, &shouldRemove](Entity &entity) -> void {
    int health = entity.health;
    float radius = entity.circle.getRadius();

    if (health <= 1) {
      shouldRemove = true;
    }

    if (entity.id == id) {
      health--;
      float newRadius = radius * health / 100;
      entity.circle.setRadius(newRadius);
      entity.circle.setOrigin(sf::Vector2f(newRadius, newRadius));
    }
  });

  if (!shouldRemove) return;
  _entities.remove([id](Entity &entity) -> bool {
    if (entity.id == id) {
      return true;
    }
    return false;
  });
}

void World::update(sf::Time deltaTime) {
  _colony->update(deltaTime);

  //_toFoodPheromones.forEach([](Entity &entity) -> void {
  //  sf::Color color = entity.circle.getFillColor();
  //  color.r = std::min(255, color.r + 1);
  //  color.g = std::min(255, color.g + 1);
  //  color.b = std::min(255, color.b + 1);

  //  entity.circle.setFillColor(color);
  //});

  //_toFoodPheromones.remove([](Entity &entity) -> bool {
  //  sf::Color color = entity.circle.getFillColor();
  //  if (color.r > 254 && color.g > 254 && color.b > 254) {
  //    return true;
  //  }
  //  return false;
  //});

  //_toHomePheromones.forEach([](Entity &entity) -> void {
  //  sf::Color color = entity.circle.getFillColor();
  //  color.r = std::min(255, color.r + 1);
  //  color.g = std::min(255, color.g + 1);
  //  color.b = std::min(255, color.b + 1);

  //  entity.circle.setFillColor(color);
  //});

  //_toHomePheromones.remove([](Entity &entity) -> bool {
  //  sf::Color color = entity.circle.getFillColor();
  //  if (color.r > 254 && color.g > 254 && color.b > 254) {
  //    return true;
  //  }
  //  return false;
  //});
}

class Colony *World::getColony() { return _colony; }
Entity World::getColonyEntity() { return _colonyEntity; }
