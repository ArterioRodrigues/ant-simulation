#pragma once
#include "pch.h"

struct CellHash {
  std::size_t operator()(const std::pair<int, int> &cell) const {
    return std::hash<int>()(cell.first) ^ (std::hash<int>()(cell.second) << 1);
  }
};

template <typename T> class SpatialHash {
public:
  SpatialHash(int cellSize = 50) : _cellSize(cellSize) { _values = {}; }

  void clear() { _grid.clear(); }
  void insert(sf::Vector2f position, const T &entity) {
    auto cell = getCell(position);
    _grid[cell].push_back(entity);
  }
  std::vector<T> getNearby(sf::Vector2f position, int radiusCells = 1) const {
    std::vector<T> nearby;
    auto centerCell = getCell(position);

    for (int dx = -radiusCells; dx <= radiusCells; dx++) {
      for (int dy = -radiusCells; dy <= radiusCells; dy++) {
        std::pair<int, int> cell = {centerCell.first + dx, centerCell.second + dy};
        auto it = _grid.find(cell);
        if (it != _grid.end()) {
          nearby.insert(nearby.end(), it->second.begin(), it->second.end());
        }
      }
    }
    return nearby;
  }

  std::vector<T> getSubCellVector(sf::Vector2f position) {
    std::vector<T> cell = {};

    auto cellPosition = getCell(position);
    auto it = _grid.find(cellPosition);
    if (it != _grid.end()) {
      cell.insert(cell.end(), it->second.begin(), it->second.end());
    }

    return cell;
  }

  size_t size() const {
    size_t total = 0;
    for (const auto &[cell, entities] : _grid) {
      total += entities.size();
    }

    return total;
  }

  const std::vector<T> &getVector() {
    _values.clear();
    for (auto cell : _grid) {
      _values.insert(_values.end(), cell.second.begin(), cell.second.end());
    }

    return _values;
  }

  size_t cellCount() const { return _grid.size(); }
//{
//  unsigned int numThreads = std::thread::hardware_concurrency();
//  if (numThreads == 0) numThreads = 4;
//
//  numThreads = std::min(numThreads, static_cast<unsigned int>(_ants.size()));
//
//  std::vector<std::thread> threads;
//  std::vector<std::vector<PheromoneData>> threadPheromones(numThreads);
//
//  int antsPerThread = _ants.size() / numThreads;
//
//  for (unsigned int i = 0; i < numThreads; ++i) {
//    int start = i * antsPerThread;
//    int end = (i == numThreads - 1) ? _ants.size() : (i + 1) * antsPerThread;
//
//    threads.emplace_back([this, start, end, deltaTime, &threadPheromones, i]() {
//      for (int j = start; j < end; ++j) {
//        std::vector<PheromoneData> antPheromones = _ants[j]->update(deltaTime);
//        threadPheromones[i].insert(threadPheromones[i].end(), antPheromones.begin(), antPheromones.end());
//      }
//    });
//  }
//  for (auto &thread : threads) {
//    thread.join();
//  }
//
//  std::vector<PheromoneData> allPheromones;
//  for (const auto &pheromones : threadPheromones) {
//    allPheromones.insert(allPheromones.end(), pheromones.begin(), pheromones.end());
//  }
//
//  Configuration::world->batchInsertPheromones(allPheromones);
//}
  void forEach(std::function<void(T &)> callback) {
  unsigned 
    for (auto &[cell, entities] : _grid) {
      for (size_t i = 0; i < entities.size(); i++) {
        T &entity = entities[i];
        callback(entity);
      }
    }
  }

  void remove(std::function<bool(T &)> callback) {
    for (auto &[cell, entities] : _grid) {
      for (size_t i = 0; i < entities.size();) {
        T &entity = entities[i];
        if (callback(entity)) {
          entities[i] = entities.back();
          entities.pop_back();
        } else {
          i++;
        }
      }
    }
  }
  std::pair<int, int> getCell(sf::Vector2f position) const {
    return {
        static_cast<int>(position.x / _cellSize),
        static_cast<int>(position.y / _cellSize),
    };
  }

private:
  int _cellSize;
  std::unordered_map<std::pair<int, int>, std::vector<T>, CellHash> _grid;

  std::vector<T> _values;
};
