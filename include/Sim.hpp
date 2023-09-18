#pragma once


#include <random>
#include <vector>
#include <sstream>
#include <execution>

#include "Sim/sim_types.hpp"
#include "Sim/PetriDish.hpp"
#include "Sim/Printer.hpp"
#include "Sim/Board.hpp"
#include "utils/Term.hpp"
// #include "utils/TermPlus.hpp"
#include "utils/Timer.hpp"


namespace sim {
    using Term = term::Term;
    using PetriDish = petridish::PetriDish;


    class Sim {
        public:
            Sim (uint64_t seed) : rng_gen(seed), term(Term::instance()) {
                for (size_t i = 0; i < number_of_petri_dishes; i++) {
                    uint64_t new_seed = rng_gen();
                    petri_dishes.push_back( PetriDish(new_seed) );
                }
            }
            
            ~Sim () {}

            void process_input () {
                int input = term.input();

                switch (input) {
                    case 'q': // Quit
                        status.running = false; 
                        break;

                    case 'g': // Go fast mode
                        status.syncing = !(status.syncing);
                        break;
                    
                    case 'p': // Paused
                        status.paused = !(status.paused);
                        break;

                    case 'k': // Printing off
                        status.printing = !(status.printing);
                        break;

                    case 'o': // powersave
                        status.powersave = !(status.powersave);
                        break;

                    case '=': // Changing main board
                        main_dish_index = (main_dish_index + 1) % number_of_petri_dishes;
                        break;

                    case '-': // Changing main board
                        main_dish_index = (number_of_petri_dishes + main_dish_index - 1) % number_of_petri_dishes;
                        break;
                }
            }

            void run () {
                auto petri_dish_foward = [&](PetriDish& dish) {
                    dish.foward();
                };

                timer::Timer timer;
                while (status.running) {
                    // Start time measurement
                    timer.start_measurement();
        
                    // User input
                    process_input ();

                    // Simulation
                    if (!status.paused) {
                        if (status.powersave) {
                            petri_dishes[0].foward();
                        } else {
                            std::for_each(
                                std::execution::par, 
                                petri_dishes.begin(), 
                                petri_dishes.end(), 
                                petri_dish_foward
                            );
                        }
                    }

                    // Printing
                    board_printer.print(
                        petri_dishes[main_dish_index].get_board(), 
                        timer, 
                        status, 
                        generation
                    );

                    // End measurement and sync
                    timer.end_measurement();
                    if (status.syncing) timer.sync(target_delta_time);
                }
            }

        private:
            unsigned generation = 0;
            double target_fps = sim::TARGET_FPS;
            double target_delta_time = 1.0 / sim::TARGET_FPS;
            size_t number_of_petri_dishes = sim::NUMBER_OF_PETRI_DISHES;

            uint64_t iteration_counter = 0;

            size_t main_dish_index = 3;

            SimStatus status;

            std::mt19937_64 rng_gen;

            Term& term;
            std::vector<PetriDish> petri_dishes;
            printer::Printer board_printer;
    };
}