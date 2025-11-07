#pragma once
#include "pch.h"

class Game {
public:
  Game();

  void run(int frameRate = 6000);
  void processEvents();
  void update(sf::Time deltaTime);
  void render();

private:
  sf::RenderWindow _window;
  std::vector<int> _pheromoneTiles;
  int _x;
  int _y;
};
