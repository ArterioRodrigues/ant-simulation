#include "player.h"
#include "configuration.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

Player::Player() : _sprite(Configuration::textures.get(Configuration::Textures::Player)) {
  sf::Texture texture = Configuration::textures.get(Configuration::Textures::Player);

}

void Player::update(int deltaTime) {
_sprite.setOrigin(sf::Vector2f());
}

void Player::processEvent() {}
void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(_sprite, states);
}
