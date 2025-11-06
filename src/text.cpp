#include "text.h"

Text::Text(std::string content): _text(_font)  {
  _font = sf::Font("media/fonts/roboto/Roboto-Black.ttf");
  _text.setString(content);
  _text.setFont(_font);

  _text.setCharacterSize(42);
  _text.setFillColor(sf::Color::Red);
  _text.setStyle(sf::Text::Bold);
  _text.setPosition({30, 30});
}
void Text::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(_text, states);
}
