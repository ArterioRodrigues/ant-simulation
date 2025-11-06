#include "helper.h"
#include <random>
int randomNumberGenerator(int start, int end) {
  static thread_local std::random_device randomDevice;
  static thread_local std::default_random_engine eng(randomDevice());
  std::uniform_int_distribution<int> dist(start, end);
  return dist(eng);
}

