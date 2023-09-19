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
#include "Sim/Population/Goats.hpp"


namespace sim {
    using Term = term::Term;
    using PetriDish = petridish::PetriDish;


    class Sim {
        public:
            Sim (uint64_t seed) : rng_gen(seed), term(Term::instance()) {
                for (size_t i = 0; i < settings.number_of_dishes; i++) {
                    uint64_t new_seed = rng_gen();
                    petri_dishes.push_back( PetriDish(new_seed) );
                }
            }
            
            ~Sim () {}

            void process_input () {
                int input = term.input();

                switch (input) {
                    case 'q': // Quit
                        settings.running = false; 
                        break;

                    case 'g': // Go fast mode
                        settings.syncing = !(settings.syncing);
                        break;
                    
                    case 'p': // Paused
                        settings.paused = !(settings.paused);
                        break;

                    case 'k': // Printing off
                        settings.printing = !(settings.printing);
                        break;

                    case 'o': // powersave
                        settings.powersave = !(settings.powersave);
                        break;

                    case '=': // Changing main board
                        settings.main_dish = (settings.main_dish + 1) % settings.number_of_dishes;
                        break;

                    case '-': // Changing main board
                        settings.main_dish = (settings.number_of_dishes + settings.main_dish - 1) % settings.number_of_dishes;
                        break;
                }
            }

            void run () {
                auto petri_dish_foward = [&](PetriDish& dish) {
                    dish.foward();
                };

                timer::Timer timer;
                while (settings.running) {
                    // Start time measurement
                    timer.start_measurement();
        
                    // User input
                    process_input ();

                    // Simulation
                    if (!settings.paused) {
                        if (settings.powersave) {
                            petri_dishes[settings.main_dish].foward();
                        } else {
                            std::for_each(
                                std::execution::par, 
                                petri_dishes.begin(), 
                                petri_dishes.end(), 
                                petri_dish_foward
                            );
                        }
                    }

                    if (check_running()) {
                        next_gen();
                    }

                    // Printing
                    board_printer.print(
                        petri_dishes[settings.main_dish].get_board(), 
                        timer, 
                        settings, 
                        generation
                    );

                    // End measurement and sync
                    timer.end_measurement();
                    if (settings.syncing) timer.sync(target_delta_time);
                }
            }

            bool check_running () {
                if (iteration_counter > sim::MAX_ITERATION_NUMBER) return true;

                bool all_halted = true;

                for (const PetriDish& dish : petri_dishes) {
                    if (!dish.has_halted()) {
                        all_halted = false;
                        break;
                    }
                }

                return all_halted;
            }

            void next_gen() {
                generation++;
                iteration_counter = 0;

                for (PetriDish& dish : petri_dishes) {
                    dish.save_results();
                }

                for (PetriDish& dish : petri_dishes) {
                    dish.next_gen();
                }

                for (PetriDish& dish : petri_dishes) {
                    dish.start_now();
                }

                population::Goats::instance().decay();
            }

        private:
            unsigned generation = 0;
            double target_fps = sim::TARGET_FPS;
            double target_delta_time = 1.0 / sim::TARGET_FPS;

            uint64_t iteration_counter = 0;

            SimSettings settings;

            std::mt19937_64 rng_gen;

            Term& term;
            std::vector<PetriDish> petri_dishes;
            printer::Printer board_printer;
    };
}