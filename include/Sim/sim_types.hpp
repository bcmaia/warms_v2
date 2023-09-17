#pragma once


namespace sim {
    class SimStatus {
    public:
        bool running = true;
        bool syncing = true;
        bool printing = true;
        bool paused = false;
        bool powersave = false;

        bool operator==(const SimStatus& other) const {
            return (
                running == other.running
                && syncing == other.syncing
                && printing == other.printing
                && paused == other.paused
                && powersave == other.powersave
            );
        }

        bool operator!=(const SimStatus& other) const {return !(*this == other);}

        SimStatus operator=(const SimStatus& other) {
            running = other.running;
            syncing = other.syncing;
            printing = other.printing;
            paused = other.paused;
            powersave = other.powersave;

            return *this;
        }
    };
}