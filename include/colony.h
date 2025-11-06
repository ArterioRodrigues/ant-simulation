#pragma once
#include "ant.h"
#include "configuration.h"
#include "helper.h"
#include "pch.h"
#include <SFML/System/Vector2.hpp>

class Colony {
public:
  Colony(int size, const sf::Vector2f spawnPoint);
  ~Colony();
  void update(sf::Time deltaTime);
  std::vector<Ant *> getAnts();

  sf::Vector2f getPosition();
  void setFoodCount(int foodCount);
  int getFoodCount();

private:
  int _size;
  int _foodCount;
  sf::Vector2f _spawnPoint;
  std::vector<Ant *> _ants;
};
