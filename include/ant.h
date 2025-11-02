#pragma once
#include "pch.h"

enum Direction {
  North = 0,
  East = 1,
  South = 2,
  West = 3,
};

class Ant : public sf::Drawable {
public:
  Ant(const Ant &) = delete;
  Ant &operator=(const Ant &) = delete;
  Ant(int movementDistance, sf::Vector2f position);

  void processEvent();
  void update(sf::Time deltaTime);

  void move(Direction direction);

  sf::Vector2f getPosition();

  void rotate(Direction direction);
private:
  void draw(sf::RenderTarget &target,
            sf::RenderStates states = sf::RenderStates::Default) const override;
  sf::Sprite _sprite;
  int _movementDistance;
};
