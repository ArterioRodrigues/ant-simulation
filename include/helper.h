#include "pch.h"
int randomNumberGenerator(int start, int end);
int weightedRandomGenerator(const std::vector<uint8_t>& weights);
class ThreadSafeRandom {
public:
    static int generate(int start, int end);
    static int weightedGenerate(const std::vector<uint8_t>& weights);
};
