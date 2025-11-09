#include "colony.h"
#include "configuration.h"
#include "helper.h"
#include "world.h"

Colony::Colony(int size, const sf::Vector2f spawnPoint) {
  std::cout << spawnPoint.x  << "  "<< spawnPoint.y << std::endl;
  _size = size;
  _spawnPoint = {spawnPoint.x, spawnPoint.y };

  for (int i = 0; i < size; i++) {
    int x = _spawnPoint.x - randomNumberGenerator(-50, 50);
    int y = _spawnPoint.y - randomNumberGenerator(-50, 50);

    sf::Vector2f position(x, y);
    Ant *ant = new Ant(Configuration::movementDistance, position, spawnPoint);
    _ants.push_back(ant);
  }
}

void Colony::update(sf::Time deltaTime) {
  if (Configuration::enableMultitThreading && _ants.size() >= 20) {
    updateAntsParallel(deltaTime);
  } else {
    updateAntsSingleThreaded(deltaTime);
  }
}

void Colony::updateAntsSingleThreaded(sf::Time deltaTime) {
  std::vector<PheromoneData> allPheromones;

  for (auto ant : _ants) {
    std::vector<PheromoneData> antPheromones = ant->update(deltaTime);
    allPheromones.insert(allPheromones.end(), antPheromones.begin(), antPheromones.end());
  }

  Configuration::world->batchInsertPheromones(allPheromones);
}

void Colony::updateAntsParallel(sf::Time deltaTime) {
  unsigned int numThreads = std::thread::hardware_concurrency();
  if (numThreads == 0) numThreads = 4;

  numThreads = std::min(numThreads, static_cast<unsigned int>(_ants.size()));

  std::vector<std::thread> threads;
  std::vector<std::vector<PheromoneData>> threadPheromones(numThreads);

  int antsPerThread = _ants.size() / numThreads;

  for (unsigned int i = 0; i < numThreads; ++i) {
    int start = i * antsPerThread;
    int end = (i == numThreads - 1) ? _ants.size() : (i + 1) * antsPerThread;

    threads.emplace_back([this, start, end, deltaTime, &threadPheromones, i]() {
      for (int j = start; j < end; ++j) {
        std::vector<PheromoneData> antPheromones = _ants[j]->update(deltaTime);
        threadPheromones[i].insert(threadPheromones[i].end(), antPheromones.begin(), antPheromones.end());
      }
    });
  }
  for (auto &thread : threads) {
    thread.join();
  }

  std::vector<PheromoneData> allPheromones;
  for (const auto &pheromones : threadPheromones) {
    allPheromones.insert(allPheromones.end(), pheromones.begin(), pheromones.end());
  }

  Configuration::world->batchInsertPheromones(allPheromones);
}

std::vector<Ant *> Colony::getAnts() { return _ants; }

void Colony::setFoodCount(int foodCount) { _foodCount = foodCount; }
int Colony::getFoodCount() { return _foodCount; }
sf::Vector2f Colony::getPosition() { return _spawnPoint; }
