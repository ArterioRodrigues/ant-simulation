#include "colony.h"
#include "configuration.h"
#include "helper.h"
Colony::Colony(int size, const sf::Vector2f spawnPoint) {
  _size = size;
  _spawnPoint = spawnPoint;

  for (int i = 0; i < size; i++) {
    int x = spawnPoint.x - randomNumberGenerator(spawnPoint.x - 10, spawnPoint.x);
    int y = spawnPoint.y - randomNumberGenerator(spawnPoint.y - 10, spawnPoint.y);
    sf::Vector2f position(x, y);
    Ant *ant = new Ant(Configuration::tileX / 4, spawnPoint, spawnPoint);
    _ants.push_back(ant);
  }
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
