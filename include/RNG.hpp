#include <random>

class RNG {
public:
    RNG() : seed(std::random_device{}()), gen(seed) {}
    RNG(unsigned _seed) : seed(_seed), gen(seed) {}
    unsigned randu(unsigned a, unsigned b) {
        std::uniform_int_distribution<unsigned> dis(a, b);
        return dis(gen);
    }
    bool happen(double p) {
        std::bernoulli_distribution dis(p);
        return dis(gen);
    }
private:
    unsigned seed;
    std::mt19937 gen;
};
