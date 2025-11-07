#pragma once
#include "pch.h"
#include <SFML/Graphics/CircleShape.hpp>

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

  bool move(int distance, sf::Angle angle);
  void setFoodCount(int foodCount);
  int getFoodCount();
  void followPheromones();
  sf::Vector2f getPosition();

  void rotate(Direction direction);
  void returnHome();
  AntType type;

  Direction getDirectionToward(sf::Vector2f target);
  Direction getDirectionFromAngle(float angle);

private:
  void draw(sf::RenderTarget &target, sf::RenderStates states = sf::RenderStates::Default) const override;

  int _movementDistance;
  int _foodCount;
  int _weight;

  sf::Vector2f _colonyPosition;
  sf::Sprite _sprite;
  sf::Time _deltaTime;
  sf::CircleShape _food;
  Direction _lastDirection;
};
