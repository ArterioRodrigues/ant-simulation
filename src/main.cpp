#include "configuration.h"
#include "game.h"

int main() {
  Configuration configuration;
  Game game;
  game.run(Configuration::_frameRate);
  return 0;
}
