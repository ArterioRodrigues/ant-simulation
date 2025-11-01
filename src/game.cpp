#include "game.h"
#include "configuration.h"

Game::Game()
    : _ant(Configuration::_tileX),
      _window(sf::VideoMode({Configuration::_windowX, Configuration::_windowY}), "ant simulation") {
  _x = Configuration::_windowX;
  _y = Configuration::_windowY;
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

void Game::update(sf::Time deltaTime) { _ant.update(deltaTime); }

void Game::render() {
  _window.clear();

  for (auto tiles : Configuration::world->getTiles()) {
    for (auto tile : tiles) {
      sf::Vector2f tilePosition = tile.shape.getPosition();
      sf::Vector2f antPosition = _ant.getPosition();

      if (antPosition.x < tilePosition.x + Configuration::_tileX &&
          antPosition.x > tilePosition.x &&
          antPosition.y < tilePosition.y + Configuration::_tileY &&
          antPosition.y > tilePosition.y) {
        _pheromoneTiles.push_back(tile.index);
      }
      for(auto index: _pheromoneTiles) {
        if(tile.index == index)
          tile.shape.setFillColor({0, 255, 0});
      }
      _window.draw(tile.shape);
    }
  }
  _window.draw(_ant);
  _window.display();
}
