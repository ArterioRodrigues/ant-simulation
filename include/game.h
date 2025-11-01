#pragma once
#include "pch.h"
#include "player.h"

class Game {
public:
  Game();

  void run(int frame_per_seconds = 6000);
  void processEvents();
  void update(sf::Time deltaTime);
  void render();

private:
  sf::RenderWindow _window;
  Player _player;
  int _x;
  int _y;
};
