#pragma once

#include "Sim/sim_constants.hpp"

#include <random>
#include <cmath> // Include the <cmath> header for the tanh function
#include <vector>
#include <stdexcept>

#include <sstream>
#include "Term.hpp"

// typedef float nn_input_t [NN_INPUT_SIZE];

constexpr uint8_t NUMBER_OF_ACT_FUNCTIONS = 6;

enum class ActivationFunction : uint8_t {
    None = 0, Tanh, Binary, Ternary, Dead, ConstBias
};

template <size_t W, size_t H>
class NeuralLayer {
public:
    NeuralLayer() {}

    NeuralLayer (const NeuralLayer<W, H>& parent1, const NeuralLayer<W, H>& parent2, uint64_t seed) {
        std::mt19937_64 gen(seed);
        std::bernoulli_distribution choice(0.5);

        for (size_t y = 0; y < H; y++) {
            for (size_t x = 0; x < W; x++) {
                coefficients[y][x] = choice(gen) 
                    ? parent1.get_coef(x, y) 
                    : parent2.get_coef(x, y);
                // coefficients[y][x] = 0.0;
            }

            biases[y] = choice(gen) ? parent1.get_bias(y) : parent2.get_bias(y);
            functions[y] = choice(gen) ? parent1.get_fun(y) : parent2.get_fun(y);
        }

        mutate(seed);
    }

    NeuralLayer (const NeuralLayer<W, H>& other) {
        for (size_t y = 0; y < H; y++) {
            for (size_t x = 0; x < W; x++) {
                coefficients[y][x] = other.get_coef(x, y);
                // coefficients[y][x] = 0.0;
            }

            biases[y] = other.get_bias(y) ;
            functions[y] = other.get_fun(y);
        }
    }

    // Constructor that initializes coefficients and biases with random values
    NeuralLayer(uint64_t seed) {
        std::mt19937_64 generator(seed);
        std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
        std::uniform_real_distribution<float> bias_dist (-0.5, 0.5);
        std::uniform_int_distribution<int> func_dist (0, NUMBER_OF_ACT_FUNCTIONS - 1);

        for (size_t y = 0; y < H; y++) {
            for (size_t x = 0; x < W; x++) {
                coefficients[y][x] = dist(generator);
            }
            biases[y] = bias_dist(generator);
            functions[y] = static_cast<ActivationFunction>(func_dist(generator));
        }
    }
    

    // Forward one function
    inline void forward_one (
        const float inputs [W], 
        float results [H], 
        const size_t index
    ) const {
        float output = biases[index];

        for (size_t j = 0; j < W; ++j) {
            output += coefficients[index][j] * inputs[j];
        }

        results[index] = output; //apply_activation_function(functions[index], output);
    }

    // Forward one function
    // void forward (const float inputs [W], float results [H]) const {
    //     // for (size_t i = 0; i < H; i++) {
    //     //     forward_one (inputs, results, i);
    //     // }

    //     for (size_t i = 0; i < H; i++) {
    //         results [i] = 0.0;
    //         for (size_t j = 0; j < W; j++) {
    //             results [i] += coefficients[i][j] * inputs[j];
    //         }
    //         //results [i] += biases[i];
    //     }
    // }

    void forward(const float inputs[W], float results[H]) const {
        for (size_t y = 0; y < H; y++) {
            results[y] = 0.0;
            for (size_t x = 0; x < W; x++) {
                // Check for invalid values in coefficients
                if (std::isnan(coefficients[y][x]) || std::isinf(coefficients[y][x]) || coefficients[y][x] < -1000000000.0 || coefficients[y][x] > 1000000000.0) {
                    throw std::runtime_error("Invalid value in coefficients.");
                }

                // Check for invalid values in inputs
                if (std::isnan(inputs[x]) || std::isinf(inputs[x]) || inputs[x] < -1000000000.0 || inputs[x] > 1000000000.0) {
                    std::stringstream ss;
                    ss << "Invalid value in inputs: " << "<i: " << x << ", v: " << inputs[x] << ">";
                    throw std::runtime_error(ss.str());
                }

                results[y] += coefficients[y][x] * inputs[x];
            }

            results[y] += biases[y];

            // Check for invalid value in biases
            if (std::isnan(biases[y]) || std::isinf(biases[y]) || biases[y] < -1000000000.0 || biases[y] > 1000000000.0) {
                throw std::runtime_error("Invalid value in biases.");
            }
        }
    }

    // Getter for coefficients
    inline float get_coef (size_t x, size_t y) const {
        #ifndef RELEASE
        if (y >= H || x >= W) {
            throw std::out_of_range("Invalid index for coefficients.");
        }
        #endif

        return coefficients[y][x];
    }

    // Getter for biases
    inline float get_bias (size_t i) const {
        #ifndef RELEASE
        if (i >= H) {
            throw std::out_of_range("Invalid index for biases.");
        }
        #endif

        return biases[i];

    }

     // Getter for biases
    inline ActivationFunction get_fun (size_t i) const {
        #ifndef RELEASE
        if (i >= H) {
            throw std::out_of_range("Invalid index for biases.");
        }
        #endif

        return functions[i];
    }

    NeuralLayer<H, W>& operator=(const NeuralLayer<H, W>& other) {
        if (this == &other) {
            return *this; // Self-assignment guard
        }

        // Copy coefficients and biases
        for (size_t y = 0; y < H; y++) {
            for (size_t x = 0; x < W; x++) {
                coefficients[y][x] = other.get_coef(x, y);
            }
            biases[y] = other.get_bias(y);
            functions[y] = other.get_fun(y);
        }

        return *this;
    }

private:
    float coefficients[H][W] = {};
    float biases[H] = {};
    ActivationFunction functions[H];

    static inline float apply_activation_function (ActivationFunction f, float x) {
        switch (f) {
            case ActivationFunction::Tanh:
                return tanh(x);

            case ActivationFunction::Binary: 
                return 0.5 < x ? 1.0 : 0.0;

            case ActivationFunction::Ternary:
                return -0.5 > x ? -1.0 : +0.5 < x ? 1.0 : 0.0 ;

            case ActivationFunction::Dead:
                return 0.0;

            case ActivationFunction::ConstBias:
                return 1.0;

            default:
                return x;
        }
    }

    void mutate (uint64_t seed) {
        std::mt19937_64 gen(seed);
        std::bernoulli_distribution mutation (sim::MUTATION_RATE);
        std::uniform_real_distribution<float> mutation_dist (-1.0, 1.0);
        std::uniform_real_distribution<float> bias_dist (-0.5, 0.5);
        std::uniform_int_distribution<uint8_t> act_fun_dist (0, NUMBER_OF_ACT_FUNCTIONS - 1);

        for (size_t i = 0; i < H; ++i) {
            for (size_t j = 0; j < W; ++j) {
                if (mutation(gen)) coefficients[i][j] = mutation_dist(gen);
            }

            if (mutation(gen)) biases[i] = bias_dist(gen);
            if (mutation(gen)) functions[i] = static_cast<ActivationFunction>(act_fun_dist(gen));

            functions[i] = ActivationFunction::Tanh;
        }
    }
};

// template <size_t H, size_t W>
// __global__ void nn_foward_kernel (
//     const nn_input_t* input, 
//     const NeuralLayer<H, W>* layer
// );


// template <size_t H, size_t W, size_t InputSize>
// __global__ void nn_forward_kernel(
//     const NeuralLayer<InputSize, H>* input, 
//     const NeuralLayer<H, W>* layer
// ) {}