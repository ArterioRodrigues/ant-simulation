#include "helper.h"
#include <random>

int randomNumberGenerator(int start, int end) {

  std::random_device randomDevice;
  std::default_random_engine eng(randomDevice());
  std::uniform_int_distribution<int> dist(start, end);

  return dist(eng);
}

int weightedRandomGenerator(const std::vector<uint8_t>& weights) {
  static std::random_device randomDevice;
  static std::default_random_engine eng(randomDevice());
  std::discrete_distribution<int> dist(weights.begin(), weights.end());

  return dist(eng);
}
