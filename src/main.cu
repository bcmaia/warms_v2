#include "Sim.hpp"

int main () {
    uint64_t seed = 1029384756;
    sim::Sim simulation (seed);
    simulation.run();

    return 0;
}