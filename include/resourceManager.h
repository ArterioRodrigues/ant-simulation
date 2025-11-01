#pragma once
#include "pch.h"

template <typename RESOURCE, typename IDENTIFIER = int> class ResourceManager {
public:
  ResourceManager(const ResourceManager &) = delete;
  ResourceManager &operator=(const ResourceManager &) = delete;

  ResourceManager() = default;

  template <typename... Args> void load(const IDENTIFIER id, Args &&...args) {
    std::unique_ptr<RESOURCE> ptr(new RESOURCE);
    if (not ptr->loadFromFile(std::forward<Args>(args)...))
      throw std::runtime_error("Impossible to load file");
    _map.emplace(id, std::move(ptr));
  }

  RESOURCE &get(const IDENTIFIER &id) const { return *_map.at(id); }

private:
  std::unordered_map<IDENTIFIER, std::unique_ptr<RESOURCE>> _map;
};

template <typename IDENTIFIER> class ResourceManager<sf::Music, IDENTIFIER> {
public:
  ResourceManager(const ResourceManager &) = delete;
  ResourceManager &operator=(const ResourceManager &) = delete;

  ResourceManager() = default;

  template <typename... Args> void load(const IDENTIFIER id, Args &&...args) {
    std::unique_ptr<sf::Music> ptr(new sf::Music);
    if (not ptr->openFromFile(std::forward<Args>(args)...))
      throw std::runtime_error("Impossible to load file");
    _map.emplace(id, std::move(ptr));
  }

  sf::Music &get(const IDENTIFIER &id) const { return *_map.at(id); }

private:
  std::unordered_map<IDENTIFIER, std::unique_ptr<sf::Music>> _map;
};
