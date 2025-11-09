#include "helper.h"
#include <random>

int randomNumberGenerator(int start, int end) { return ThreadSafeRandom::generate(start, end); }

float randomNumberGenerator(float start, float end) { return ThreadSafeRandom::generate(start, end); }

int weightedRandomGenerator(const std::vector<uint8_t> &weights) { return ThreadSafeRandom::weightedGenerate(weights); }

int ThreadSafeRandom::generate(int start, int end) {
  thread_local std::random_device rd;
  thread_local std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(start, end);
  return dist(gen);
}

int ThreadSafeRandom::weightedGenerate(const std::vector<uint8_t> &weights) {
  thread_local std::random_device rd;
  thread_local std::mt19937 gen(rd());
  std::discrete_distribution<int> dist(weights.begin(), weights.end());
  return dist(gen);
}
