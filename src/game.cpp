#include "game.h"
#include "configuration.h"
#include "text.h"
#include "world.h"

Game::Game() : _window(sf::VideoMode({Configuration::windowX, Configuration::windowY}), "ant simulation") {
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

void Game::update(sf::Time deltaTime) { Configuration::world->update(deltaTime); }

void Game::render() {
  Text text(std::to_string(Configuration::world->getFoodCount()));

  _window.clear(Configuration::normalColor);

  if (Configuration::toFoodPheromones) {
    for (auto entity : Configuration::world->getToFoodPheromones()) {
      _window.draw(entity.circle);
    }
  }

  if (Configuration::toHomePheromones) {
    for (auto entity : Configuration::world->getToHomePheromones()) {
      _window.draw(entity.circle);
    }
  }

  for (auto entity : Configuration::world->getEntities()) {
    _window.draw(entity.circle);
  }

  for (Ant *ant : Configuration::world->getColony().getAnts()) {
    _window.draw(*ant);
  }

  _window.draw(Configuration::world->getColonyEntity().circle);
  _window.draw(text);

  _window.display();
}
