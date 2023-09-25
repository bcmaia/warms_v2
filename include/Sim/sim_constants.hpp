#pragma once


#include <cstdint>


namespace sim {
    constexpr bool LIVE_TAIL = false;

    constexpr float PROCEDURAL_WALLS_FACTOR_0 = 0.01;
    constexpr float PROCEDURAL_WALLS_FACTOR_1 = 0.08;
    constexpr float PROCEDURAL_WALLS_FACTOR_2 = 0.32;
    constexpr float PROCEDURAL_WALLS_FACTOR_3 = 0.64;
    constexpr float PROCEDURAL_WALLS_FACTOR_4 = 0.72;

    constexpr size_t GOATS_SIZE = 16;
    constexpr float GOATS_DECAY = 0.001;
    constexpr float GET_FROM_GOATS_CHANCE = 1.0 / 8.0;

    // This constant determines how many times a random atempt can be executed
    //  without success.
    constexpr uint8_t MAX_ATEMPTS = 8;

    constexpr float TARGET_FPS = 1.0;

    constexpr size_t NUMBER_OF_PETRI_DISHES = 16;
    constexpr size_t POPULATION_SIZE = 64;

    constexpr float FOOD_ENERGY = 16.0;
    constexpr float ENERGY_PER_SNAKE_CELL = 16.0;
    constexpr uint32_t MIN_SNAKE_LENGTH = 2;
    constexpr uint64_t MAX_ITERATION_NUMBER = 1000;

    constexpr double FOOD_FITNESS_REWARD = 1.0;
    constexpr double SURVIVAL_FITNESS_REWARD = 1.0 / 15.0;

    constexpr size_t FAR_SIGHT_MAX_DISTANCE = 16;
    constexpr size_t SENSOR_PER_CELL = 6;
    constexpr size_t SHORT_SIGHT_RANGE = 3;
    constexpr size_t SHORT_SIGHT_DIAMETER = (2 * SHORT_SIGHT_RANGE) + 1;
    constexpr size_t SHORT_SIGHT = SHORT_SIGHT_DIAMETER * SHORT_SIGHT_DIAMETER - 1;
    constexpr size_t FAR_SIGHT = 8;
    
    constexpr size_t SENSORIAL_DATA = SENSOR_PER_CELL * (SHORT_SIGHT + FAR_SIGHT);

    constexpr size_t NN_INPUT_SIZE = SENSORIAL_DATA;
    constexpr size_t NN_OUTPUT_SIZE = 3;
    constexpr size_t NN_L1_SIZE = 71;
    constexpr size_t NN_L2_SIZE = 127;
    constexpr size_t NN_L3_SIZE = 31;
    constexpr size_t NN_L4_SIZE = 63;
    constexpr size_t NN_L5_SIZE = 15;

    constexpr bool USE_STD_VEC_FOR_INPUTS = false;

    constexpr float MUTATION_RATE = 0.03;
    // constexpr float COLOR_MUTATION_RATE = 0.05;
}
