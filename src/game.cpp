#include "game.h"
#include "configuration.h"

Game::Game()
    : _window(sf::VideoMode({Configuration::_windowX, Configuration::_windowY}), "ant simulation") {
  _x = Configuration::_windowX;
  _y = Configuration::_windowY;
}

void Game::run(int frame_per_seconds) {
  sf::Clock clock;
  sf::Time timeSinceLastUpdate = sf::Time::Zero;
  sf::Time TIME_PER_FRAME = sf::seconds(1.f / frame_per_seconds);
  while (_window.isOpen()) {
    processEvents();
    timeSinceLastUpdate += clock.restart();

    while (timeSinceLastUpdate > TIME_PER_FRAME) {
      timeSinceLastUpdate -= TIME_PER_FRAME;
      update(TIME_PER_FRAME);
    }

    update(timeSinceLastUpdate);
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

void Game::update(sf::Time deltaTime) {}

void Game::render() {
  _window.clear();

  for (auto tiles : Configuration::world->getTiles()) {
    for (auto tile : tiles) {
      _window.draw(tile.shape);
    }
  }
  _window.draw(_player);
  _window.display();
}
