#pragma once
#include "pch.h"

class Player : public sf::Drawable {
public:
  Player(const Player &) = delete;
  Player &operator=(const Player &) = delete;
  Player();

  void processEvent(); 
  void update(int deltaTime);

private:
  void draw(sf::RenderTarget &target,
            sf::RenderStates states = sf::RenderStates::Default) const override;
  sf::Sprite _sprite;
};
