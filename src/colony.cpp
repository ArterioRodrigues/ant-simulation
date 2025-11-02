#include "colony.h"
#include "configuration.h"

Colony::Colony(int size, const sf::Vector2f spawnPoint) {
  _size = size;
  _spawnPoint = spawnPoint;
  
  for(int i = 0; i < size; i++) {
    Ant* ant = new Ant(Configuration::tileX/2, spawnPoint);
    _ants.push_back(ant);
  }
}

std::vector<Ant*> Colony::getAnts() {
  return _ants;
}

void Colony::update(sf::Time deltaTime) {
  for(auto ant: _ants) {
    ant->update(deltaTime);
  }
}

sf::Vector2f Colony::getPosition() {
  return _spawnPoint;
}
