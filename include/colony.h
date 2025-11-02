#pragma once
#include "ant.h"
#include "configuration.h"
#include "helper.h"
#include "pch.h"
#include <SFML/System/Vector2.hpp>

class Colony {
public:
  Colony(int size, const sf::Vector2f spawnPoint =
                       sf::Vector2f({float(randomNumberGenerator(0, Configuration::windowX)),
                                     float(randomNumberGenerator(0, Configuration::windowY))}));

  void update(sf::Time deltaTime);
  std::vector<Ant *> getAnts();

  sf::Vector2f getPosition();
private:
  int _size;
  sf::Vector2f _spawnPoint;
  std::vector<Ant *> _ants;
};
