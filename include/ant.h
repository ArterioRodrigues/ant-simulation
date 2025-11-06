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

  bool move(int distance, sf::Angle angle = sf::degrees(0));
  void setFoodCount(int foodCount);
  int getFoodCount();
  float getAngleFromDirection(Direction &direction);
  Direction getDirectionFromAngle(float angle);

  sf::Vector2f getPosition();

  void returnHome();
  AntType type;

  Direction getDirectionToward(sf::Vector2f target);
  float nextSearchAngle();

private:
  void draw(sf::RenderTarget &target, sf::RenderStates states = sf::RenderStates::Default) const override;

  int _foodCount;
  int _movementDistance;

  sf::Sprite _sprite;
  sf::Vector2f _colonyPosition;
  sf::Vector2f _foodPosition;

  Direction _lastDirection;
};
