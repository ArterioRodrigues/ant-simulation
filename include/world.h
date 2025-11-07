#pragma once
#include "colony.h"
#include <SFML/System/Vector2.hpp>
#include <unordered_map>

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
  std::unordered_map<int, Entity> getEntities();
  std::unordered_map<int, Entity> getToHomePheromones();
  std::unordered_map<int, Entity> getToFoodPheromones();

  float getPheromoneStrength(sf::Vector2f position, enum Pheromones pheromone, int radius = 1);
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
  class Colony _colony;
  std::unordered_map<int, Entity> _entities;
  std::unordered_map<int, Entity> _toFoodPheromones;
  std::unordered_map<int, Entity> _toHomePheromones;
};
