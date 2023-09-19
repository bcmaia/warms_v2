#pragma once


#include <memory>

#include "utils/types.hpp"
#include "Sim/sim_constants.hpp"
#include "NeuralLayer.hpp"

#include <sstream>
#include "Term.hpp"


// Quick Types
using Layer1 = NeuralLayer<sim::NN_L1_SIZE, sim::NN_INPUT_SIZE>;
using Layer2 = NeuralLayer<sim::NN_L2_SIZE, sim::NN_L1_SIZE>;
// using Layer3 = NeuralLayer<NN_L3_SIZE, NN_L2_SIZE>;
using OutputLayer = NeuralLayer<sim::NN_OUTPUT_SIZE, sim::NN_L2_SIZE>;


class NeuralNetwork {
public:
    NeuralNetwork () {}

    NeuralNetwork (const NeuralNetwork& g1, const NeuralNetwork& g2, uint64_t seed) {
        layer_1 = Layer1(g1.get_layer_1(), g2.get_layer_1(), seed);
        layer_2 = Layer2(g1.get_layer_2(), g2.get_layer_2(), seed);
        // layer_3 = Layer3(g1.get_layer_3(), g2.get_layer_3(), seed);
        output_layer = OutputLayer(g1.get_output_layer(), g2.get_output_layer(), seed);
    }

    NeuralNetwork (uint64_t seed) :
        layer_1(seed),
        layer_2(seed),
        //layer_3(seed),
        output_layer(seed)
    {}
    
    ~NeuralNetwork () {}

    void forward(const float inputs[sim::NN_INPUT_SIZE], float outputs[sim::NN_OUTPUT_SIZE]) const {
        std::unique_ptr<float[]> aux_a(new float[sim::NN_L1_SIZE]);
        layer_1.forward(inputs, aux_a.get());

        // std::stringstream ss;
        // ss << "<" << aux_a[0] << ", " << aux_a[1] << ", " << aux_a[2] << ">";
        // term::Term::instance().printat(2, 2, ss.str());

        std::unique_ptr<float[]> aux_b(new float[sim::NN_L2_SIZE]);
        layer_2.forward(aux_a.get(), aux_b.get());

        // // Reuse aux_a
        // aux_a.reset(new float[NN_L3_SIZE]);
        // layer_3.forward(aux_b.get(), aux_a.get());

        output_layer.forward(aux_b.get(), outputs);
    }

    const Layer1& get_layer_1 () const {return layer_1;}
    const Layer2& get_layer_2 () const {return layer_2;}
    //const Layer3& get_layer_3 () const {return layer_3;}
    const OutputLayer& get_output_layer () const {return output_layer;}

    NeuralNetwork& operator= (const NeuralNetwork& other) {
        layer_1 = other.get_layer_1();
        layer_2 = other.get_layer_2();
        //layer_3 = other.get_layer_3();
        output_layer = other.get_output_layer();
        return *this;
    }

private:
    Layer1 layer_1;
    Layer2 layer_2;
    //Layer3 layer_3;
    OutputLayer output_layer;
};