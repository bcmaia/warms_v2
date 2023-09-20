#pragma once


#include "NeuralNetwork.hpp"
#include "utils/types.hpp"
#include "utils/mymath.hpp"


namespace agent {

    enum class NNOutputFunc {
        Softmax = 1, Greatest = 2, Sharpmax = 3 // NOTE: sharpmax is my definition
    };

    class Genome {
        public:
            Genome () {}
            Genome (const uint64_t seed) : net(seed) {}
            Genome (const Genome& g1, const Genome& g2, const uint64_t seed) {
                std::mt19937_64 rng_gen (seed);

                uint64_t new_seed = rng_gen();
                net = NeuralNetwork(g1.get_net(), g2.get_net(), seed);

                std::bernoulli_distribution choice (0.5);
                color = choice(rng_gen) ? g1.get_color() : g2.get_color();
                func = choice(rng_gen) ? g1.get_out_func() : g2.get_out_func();

                new_seed = rng_gen();
                mutate(new_seed);
            }

            Genome& operator=(const Genome& other) {
                if (&other == this) {
                    return *this;
                }

                net = other.get_net();
                color = other.get_color();

                return *this;
            }

            const NeuralNetwork& get_net () const {return net;}
            types::Color get_color () const {return color;}
            NNOutputFunc get_out_func () const {return func;}

            int32_t think (
                const float inputs[sim::NN_INPUT_SIZE],
                uint64_t seed
            ) const {
                float outputs[sim::NN_OUTPUT_SIZE];
                net.forward(inputs, outputs);

                int32_t decision = 0;
                std::mt19937_64 rng_gen (seed);
                switch (func) {
                    case NNOutputFunc::Softmax: {
                        mymath::softmax<sim::NN_OUTPUT_SIZE>(outputs);
                        uint64_t new_seed = rng_gen();
                        decision = mymath::rand_index<sim::NN_OUTPUT_SIZE>(outputs, new_seed);
                    } break;

                    case NNOutputFunc::Greatest: {
                        float v = outputs[0];
                        decision = 0;
                        for (int j = 1; j < 3; j++) 
                            if (outputs[j] > v) {v = outputs[j]; decision = j;}
                    } break;

                    case NNOutputFunc::Sharpmax: {
                        outputs[0] = outputs[0] < 0.0 ? 0.0 : outputs[0];
                        outputs[1] = outputs[1] < 0.0 ? 0.0 : outputs[1];
                        outputs[2] = outputs[2] < 0.0 ? 0.0 : outputs[2];

                        float v = outputs[0] + outputs[1] + outputs[2];

                        if (0.0 >= v) {decision = 0; break;}
                        
                        v = 1.0 / v;
                        outputs[0] *= v;
                        outputs[1] *= v;
                        outputs[2] *= v;
                        
                        uint64_t new_seed = rng_gen();
                        decision = mymath::rand_index<sim::NN_OUTPUT_SIZE>(outputs, new_seed);
                    } break;

                    default: // error
                        throw std::runtime_error("Default option is not valid at Genome.think().");
                }

                return decision;
            }
            
        private:
            types::Color color = 7;
            NeuralNetwork net;
            NNOutputFunc func = NNOutputFunc::Softmax;

            void mutate (uint64_t seed) {
                std::mt19937_64 rng_gen (seed);
                std::bernoulli_distribution choice (sim::MUTATION_RATE);

                std::uniform_int_distribution<uint8_t> color_dist (2, 31);
                std::uniform_int_distribution<uint8_t> func_dist (1, 3);

                if (choice(rng_gen)) color = static_cast<types::Color>(color_dist(rng_gen));
                if (choice(rng_gen)) func = static_cast<NNOutputFunc>(func_dist(rng_gen));
            }
    };
}