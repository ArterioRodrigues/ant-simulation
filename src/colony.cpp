#include "colony.h"
#include "configuration.h"
#include "helper.h"
Colony::Colony(int size, const sf::Vector2f spawnPoint) {
  _size = size;
  _spawnPoint = spawnPoint;
  _foodCount = 0;

  for (int i = 0; i < size; i++) {
    float x = spawnPoint.x + randomNumberGenerator(-25, 25);
    float y = spawnPoint.y + randomNumberGenerator(-25, 25);

    Ant *ant = new Ant(Configuration::tileX / 4, {x, y}, spawnPoint);
    _ants.push_back(ant);
  }
}

Colony::~Colony() {
  for (auto ant : _ants) {
    delete ant;
  }
  _ants.clear();
}
std::vector<Ant *> Colony::getAnts() { return _ants; }

void Colony::update(sf::Time deltaTime) {
  for (auto ant : _ants) {
    ant->update(deltaTime);
  }
}

sf::Vector2f Colony::getPosition() { return _spawnPoint; }

void Colony::setFoodCount(int foodCount) { _foodCount = foodCount; }
int Colony::getFoodCount() { return _foodCount; }
