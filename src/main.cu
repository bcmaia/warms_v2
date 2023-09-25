#include <random>
#include "Sim.hpp"


int main () {
    std::random_device rd;
    std::mt19937_64 rng(rd());

    uint64_t seed = rng();
    sim::Sim simulation (seed);
    simulation.run();

    return 0;
}