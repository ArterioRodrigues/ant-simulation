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

struct PheromoneData {
  sf::Vector2f position;
  enum Pheromones type;
};

class World {
public:
  World(int windowX, int windowY, int colonySize = 10, int foodCount = 5);

  const std::vector<Entity> &getEntities();
  const std::vector<Entity> &getToHomePheromones();
  const std::vector<Entity> &getToFoodPheromones();

  const std::vector<Entity> getEntities(sf::Vector2f position);
  const std::vector<Entity> getToHomePheromones(sf::Vector2f position);
  const std::vector<Entity> getToFoodPheromones(sf::Vector2f position);
  float getPheromoneStrength(sf::Vector2f position, enum Pheromones pheromoneType);
  Entity getColonyEntity();
  class Colony getColony();

  void batchInsertPheromones(const std::vector<PheromoneData> &pheromones);
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

  mutable std::mutex _foodMutex;

  SpatialHash<Entity> _entities;
  SpatialHash<Entity> _toFoodPheromones;
  SpatialHash<Entity> _toHomePheromones;

  class Colony _colony;

  std::mutex _pheromoneMutex;
};
