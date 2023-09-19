#pragma once


#include "Sim/sim_constants.hpp"


namespace sim {
    class SimSettings {
    public:
        bool running = true;
        bool syncing = true;
        bool printing = true;
        bool paused = false;
        bool powersave = false;
        unsigned main_dish = 0;
        unsigned number_of_dishes = sim::NUMBER_OF_PETRI_DISHES;

        bool operator==(const SimSettings& other) const {
            return (
                running == other.running
                && syncing == other.syncing
                && printing == other.printing
                && paused == other.paused
                && powersave == other.powersave
            );
        }

        bool operator!=(const SimSettings& other) const {return !(*this == other);}

        SimSettings operator=(const SimSettings& other) {
            running = other.running;
            syncing = other.syncing;
            printing = other.printing;
            paused = other.paused;
            powersave = other.powersave;

            return *this;
        }
    };
}