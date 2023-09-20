#pragma once


#include <memory>

#include "utils/types.hpp"
#include "Sim/sim_constants.hpp"
#include "NeuralLayer.hpp"

#include <sstream>
#include "Term.hpp"


// Quick Types
using Layer1 = NeuralLayer<sim::NN_INPUT_SIZE, sim::NN_L1_SIZE>;
using Layer2 = NeuralLayer<sim::NN_L1_SIZE, sim::NN_L2_SIZE>;
using Layer3 = NeuralLayer<sim::NN_L2_SIZE, sim::NN_L3_SIZE>;
using Layer4 = NeuralLayer<sim::NN_L3_SIZE, sim::NN_L4_SIZE>;
using Layer5 = NeuralLayer<sim::NN_L4_SIZE, sim::NN_L5_SIZE>;
using OutputLayer = NeuralLayer<sim::NN_L5_SIZE, sim::NN_OUTPUT_SIZE>;
// using OutputLayer = NeuralLayer<sim::NN_INPUT_SIZE, sim::NN_OUTPUT_SIZE>;



class NeuralNetwork {
public:
    NeuralNetwork () {}

    NeuralNetwork (const NeuralNetwork& g1, const NeuralNetwork& g2, uint64_t seed) {
        layer_1 = Layer1(g1.get_layer_1(), g2.get_layer_1(), seed);
        layer_2 = Layer2(g1.get_layer_2(), g2.get_layer_2(), seed);
        layer_3 = Layer3(g1.get_layer_3(), g2.get_layer_3(), seed);
        layer_4 = Layer4(g1.get_layer_4(), g2.get_layer_4(), seed);
        layer_5 = Layer5(g1.get_layer_5(), g2.get_layer_5(), seed);
        output_layer = OutputLayer(g1.get_output_layer(), g2.get_output_layer(), seed);
    }

    NeuralNetwork (uint64_t seed) :
        layer_1(seed),
        layer_2(seed),
        layer_3(seed),
        layer_4(seed),
        layer_5(seed),
        output_layer(seed)
    {}
    
    ~NeuralNetwork () {}

    // void forward(const float inputs[sim::NN_INPUT_SIZE], float outputs[sim::NN_OUTPUT_SIZE]) const {
    //     //std::unique_ptr<float[]> aux_a(new float[sim::NN_L1_SIZE]);

    //     float layer_1_output[sim::NN_L1_SIZE] = {};
    //     layer_1.forward(inputs, layer_1_output);

    //     // std::stringstream ss;
    //     // ss << "<" << aux_a[0] << ", " << aux_a[1] << ", " << aux_a[2] << ">";
    //     // term::Term::instance().printat(2, 2, ss.str());

    //     // std::unique_ptr<float[]> aux_b(new float[sim::NN_L2_SIZE]);
    //     float layer_2_output[sim::NN_L1_SIZE] = {};
    //     // float layer_2_input
    //     layer_2.forward(layer_1_output, layer_2_output);

    //     // // Reuse aux_a
    //     // aux_a.reset(new float[NN_L3_SIZE]);
    //     // layer_3.forward(aux_b.get(), aux_a.get());

    //     output_layer.forward(layer_2_output, outputs);
    // }
    void forward(const float inputs[sim::NN_INPUT_SIZE], float outputs[sim::NN_OUTPUT_SIZE]) const {
        float layer_1_output[sim::NN_L1_SIZE] = {};
        layer_1.forward(inputs, layer_1_output);
        
        float layer_2_output[sim::NN_L2_SIZE] = {};
        layer_2.forward(layer_1_output, layer_2_output);

        float layer_3_output[sim::NN_L3_SIZE] = {};
        layer_3.forward(layer_2_output, layer_3_output);

        float layer_4_output[sim::NN_L4_SIZE] = {};
        layer_4.forward(layer_3_output, layer_4_output);

        float layer_5_output[sim::NN_L5_SIZE] = {};
        layer_5.forward(layer_4_output, layer_5_output);
        
        output_layer.forward(layer_5_output, outputs);
    }

    int checkValidOutput(const float output[], size_t size) const {
        for (size_t i = 0; i < size; ++i) {
            if (std::isnan(output[i]) || std::isinf(output[i]) || output[i] < -10.0f || output[i] > 10.0f) {
                //return i;
            }
        }
        return -1;
    }

    const Layer1& get_layer_1 () const {return layer_1;}
    const Layer2& get_layer_2 () const {return layer_2;}
    const Layer3& get_layer_3 () const {return layer_3;}
    const Layer4& get_layer_4 () const {return layer_4;}
    const Layer5& get_layer_5 () const {return layer_5;}
    const OutputLayer& get_output_layer () const {return output_layer;}

    NeuralNetwork& operator= (const NeuralNetwork& other) {
        layer_1 = other.get_layer_1();
        layer_2 = other.get_layer_2();
        layer_3 = other.get_layer_3();
        output_layer = other.get_output_layer();
        return *this;
    }

private:
    Layer1 layer_1;
    Layer2 layer_2;
    Layer3 layer_3;
    Layer4 layer_4;
    Layer5 layer_5;
    OutputLayer output_layer;
};