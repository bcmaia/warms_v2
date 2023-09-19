#pragma once


#include "NeuralNetwork.hpp"
#include "utils/types.hpp"


namespace agent {
    class Genome {
        public:
            Genome () {}
            Genome (const uint64_t seed) : net(seed) {}
            Genome (const Genome& g1, const Genome& g2, const uint64_t seed) : 
                net(g1.get_net(), g2.get_net(), seed)
            {}

            Genome& operator=(const Genome& other) {
                if (&other == this) {
                    return *this;
                }

                return *this;
            }

            const NeuralNetwork& get_net () const {return net;}

            void think (
                const float inputs[sim::NN_INPUT_SIZE], 
                float outputs[sim::NN_OUTPUT_SIZE]
            ) const {
                net.forward(inputs, outputs);
            }
            
        private:
            NeuralNetwork net;
    };
}