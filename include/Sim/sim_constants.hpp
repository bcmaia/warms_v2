#pragma once


#include <cstdint>


namespace sim {
    //constexpr float PROCEDURAL_WALLS_FACTOR = 0.01;

    // This constant determines how many times a random atempt can be executed
    //  without success.
    constexpr uint8_t MAX_ATEMPTS = 16;

    constexpr float TARGET_FPS = 10.0;

    constexpr size_t NUMBER_OF_PETRI_DISHES = 256;
    constexpr size_t POPULATION_SIZE = 32;

    constexpr float FOOD_ENERGY = 16.0;
    constexpr float ENERGY_PER_SNAKE_CELL = 16.0;
    constexpr uint32_t MIN_SNAKE_LENGTH = 2;
    constexpr uint64_t MAX_ITERATION_NUMBER = 1000000000;

    constexpr size_t FAR_SIGHT_MAX_DISTANCE = 16;
    constexpr size_t SENSOR_PER_CELL = 4;
    constexpr size_t SHORT_SIGHT = 7 * 7 - 1;
    constexpr size_t FAR_SIGHT = 8 - 1;
    
    constexpr size_t SENSORIAL_DATA = (
        SENSOR_PER_CELL * SHORT_SIGHT
        + SENSOR_PER_CELL * FAR_SIGHT
    );
}