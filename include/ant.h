#pragma once
#include "pch.h"

enum AntType {
  Searching,
  Found,
};

enum Direction {
  North = 0,
  NorthEast = 1,
  East = 2,
  SouthEast = 3,
  South = 4,
  SouthWest = 5,
  West = 6,
  NorthWest = 7,
};

class Ant : public sf::Drawable {
public:
  Ant(const Ant &) = delete;
  Ant &operator=(const Ant &) = delete;
  Ant(int movementDistance, sf::Vector2f position, sf::Vector2f colonyPosition);

  void processEvent();
  void update(sf::Time deltaTime);

  void move(Direction direction);
  void setFoodCount(int foodCount);
  int getFoodCount();
  sf::Vector2f getPosition();

  void rotate(Direction direction);
  void returnHome(); 
  AntType type;

  Direction getDirectionToward(sf::Vector2f target);
private:
  void draw(sf::RenderTarget &target,
            sf::RenderStates states = sf::RenderStates::Default) const override;
  int _movementDistance;
  int _foodCount;
  sf::Vector2f _colonyPosition;
  sf::Sprite _sprite;
  Direction _lastDirection;
};
