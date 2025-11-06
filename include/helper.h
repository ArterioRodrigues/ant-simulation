#pragma once
#include "pch.h"
int randomNumberGenerator(int start, int end);

template<typename T>
int weightedRandomGenerator(const std::vector<T>& weights) {
  static std::random_device randomDevice;
  static std::default_random_engine eng(randomDevice());
  std::discrete_distribution<int> dist(weights.begin(), weights.end());

  return dist(eng);
}
