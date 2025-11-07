#pragma once
#include "colony.h"
#include "spatialHash.h"

enum EntityTypes {
  Pheromones,
  Colony,
  Food,
};

enum Pheromones {
  toHome,
  toFood,
};

struct Entity {
  sf::CircleShape circle;
  EntityTypes type;
  int id;
};

class World {
public:
  World(int windowX, int windowY, int colonySize = 10, int foodCount = 5);

  const std::vector<Entity> &getEntities();
  const std::vector<Entity> &getToHomePheromones();
  const std::vector<Entity> &getToFoodPheromones();

  float getPheromoneStrength(sf::Vector2f position, enum Pheromones pheromoneType, int radius = 1);
  Entity getColonyEntity();
  class Colony getColony();

  void setPhemromone(sf::Vector2f position, enum Pheromones type = Pheromones::toFood);
  void update(sf::Time deltaTime);

  void decrementFood(int index);
  int getFoodCount();

private:
  int _worldX;
  int _worldY;
  int _foodCount;

  sf::Time _deltaTime;

  static int _id;
  Entity _colonyEntity;

  std::vector<Entity> _food;

  SpatialHash<Entity> _entities;
  SpatialHash<Entity> _toFoodPheromones;
  SpatialHash<Entity> _toHomePheromones;

  class Colony _colony;
};
