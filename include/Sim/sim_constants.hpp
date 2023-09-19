#pragma once


#include <cstdint>


namespace sim {
    constexpr float PROCEDURAL_WALLS_FACTOR = 0.03;

    constexpr size_t GOATS_SIZE = 16;
    constexpr float GOATS_DECAY = 0.001;
    constexpr float GET_FROM_GOATS_CHANCE = 0.2;

    // This constant determines how many times a random atempt can be executed
    //  without success.
    constexpr uint8_t MAX_ATEMPTS = 16;

    constexpr float TARGET_FPS = 10.0;

    constexpr size_t NUMBER_OF_PETRI_DISHES = 16;
    constexpr size_t POPULATION_SIZE = 32;

    constexpr float FOOD_ENERGY = 16.0;
    constexpr float ENERGY_PER_SNAKE_CELL = 16.0;
    constexpr uint32_t MIN_SNAKE_LENGTH = 2;
    constexpr uint64_t MAX_ITERATION_NUMBER = 1000;

    constexpr size_t FAR_SIGHT_MAX_DISTANCE = 16;
    constexpr size_t SENSOR_PER_CELL = 4;
    constexpr size_t SHORT_SIGHT_RANGE = 3;
    constexpr size_t SHORT_SIGHT_DIAMETER = (2 * SHORT_SIGHT_RANGE) + 1;
    constexpr size_t SHORT_SIGHT = SHORT_SIGHT_DIAMETER * SHORT_SIGHT_DIAMETER - 1;
    constexpr size_t FAR_SIGHT = 8 - 1;
    
    constexpr size_t SENSORIAL_DATA = 224; /*(
        SENSOR_PER_CELL * SHORT_SIGHT
        + SENSOR_PER_CELL * FAR_SIGHT
    );*/

    constexpr size_t NN_INPUT_SIZE = 224;
    constexpr size_t NN_OUTPUT_SIZE = 3;
    constexpr size_t NN_L1_SIZE = 15;
    constexpr size_t NN_L2_SIZE = 7;
    constexpr size_t NN_L3_SIZE = 7;

    constexpr float MUTATION_RATE = 0.01;
}