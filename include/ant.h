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

struct PheromoneData;

class Ant : public sf::Drawable {
public:
  Ant(const Ant &) = delete;
  Ant &operator=(const Ant &) = delete;
  Ant(int movementDistance, sf::Vector2f position, sf::Vector2f colonyPosition);

  bool move(int distance, sf::Angle angle);
  void setFoodCount(int foodCount);
  bool followPheromones();

  void rotate(Direction direction);
  void returnHome();
  AntType type;

  Direction getDirectionFromAngle(float angle);

  std::vector<PheromoneData> update(sf::Time deltaTime);

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

  std::vector<sf::Vector2f> _recentPositions;
  int _stuckCounter;
};
