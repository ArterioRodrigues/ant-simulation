#include "game.h"
#include "text.h"
#include "configuration.h"
#include "world.h"
#include <SFML/Graphics/Font.hpp>
#include <string>

Game::Game()
    : _colony(Configuration::colonySize),
      _window(sf::VideoMode({Configuration::windowX, Configuration::windowY}), "ant simulation") {
  _x = Configuration::windowX;
  _y = Configuration::windowY;
}

void Game::run(int frameRate) {
  sf::Clock clock;
  sf::Time timeSinceLastUpdate = sf::Time::Zero;
  sf::Time timePerFrame = sf::seconds(1.f / frameRate);
  while (_window.isOpen()) {
    processEvents();
    timeSinceLastUpdate += clock.restart();

    while (timeSinceLastUpdate > timePerFrame) {
      timeSinceLastUpdate -= timePerFrame;
      update(timePerFrame);
    }
    // update(timeSinceLastUpdate);
    render();
  }
}

void Game::processEvents() {
  std::optional<sf::Event> optionalEvent = _window.pollEvent();
  while (optionalEvent.has_value()) {
    sf::Event event = optionalEvent.value();

    if (event.is<sf::Event::Closed>()) {
      _window.close();
    }

    else if (event.is<sf::Event::KeyPressed>()) {
      sf::Keyboard::Key code = event.getIf<sf::Event::KeyPressed>()->code;

      if (code == sf::Keyboard::Key::Escape) {
        _window.close();
      }
    }
    optionalEvent = _window.pollEvent();
  }
}

void Game::update(sf::Time deltaTime) {
  _colony.update(deltaTime);
  Configuration::world->update(deltaTime);
}

void Game::render() {
  Text text(std::to_string(_colony.getFoodCount()));
  sf::Vector2f colonyPosition = _colony.getPosition(); 
  Configuration::world->setTileType(colonyPosition, TileType::Colony);

  _window.clear();


  for (auto tiles : Configuration::world->getTiles()) {
    for (auto tile : tiles) {
      _window.draw(tile.shape);
    }
  }

  for (auto &ant : _colony.getAnts()) {
    sf::Vector2f antPosition = ant->getPosition();
    TileType type = Configuration::world->getTileType(antPosition);
    int antFoodCount = ant->getFoodCount();

    if (type == TileType::Food) {
      ant->setFoodCount(antFoodCount + 1);
      Configuration::world->decrementFood();
      Configuration::world->setTileType(antPosition, TileType::Pheromone);
    }

    else if (type == TileType::Colony && antFoodCount) {
      ant->setFoodCount(0);
      _colony.setFoodCount(_colony.getFoodCount() + antFoodCount);
    } 

    if (ant->type == AntType::Found) {
      Configuration::world->setTileType(antPosition, TileType::Pheromone);
    }

    _window.draw(*ant);
  }
  
  _window.draw(text); 
  _window.display();
}
