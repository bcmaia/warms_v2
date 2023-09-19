#pragma once


#include <cmath>
#include <random>


namespace mymath {
    template <size_t S>
    void softmax(float v[S]) {
        float max_val = v[0];
        for (size_t i = 1; i < S; ++i) {
            if (v[i] > max_val) {
                max_val = v[i];
            }
        }

        float sum = 0.0;
        for (size_t i = 0; i < S; ++i) {
            v[i] = std::exp(v[i] - max_val);
            sum += v[i];
        }

        for (size_t i = 0; i < S; ++i) {
            v[i] /= sum;
        }
    }

    template <size_t S>
    size_t rand_index (const float v[S], const uint64_t seed) {
        std::mt19937_64 rng_gen (seed);
        std::uniform_real_distribution<float> dist (0.0, 1.0);

        float amount = dist(rng_gen);

        size_t i;
        for (i = 0; i < S; i++) {
            amount -= v[i];
            if (0.0 >= amount) break;
        }

        if (i >= S) i = S - 1;

        return i;
    }
}