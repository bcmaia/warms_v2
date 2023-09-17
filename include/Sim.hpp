#pragma once


#include <random>
#include <vector>
#include <sstream>

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
                petri_dishes.push_back( PetriDish(seed) );
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
                }
            }

            void run () {

                timer::Timer timer;

                const board::Board& main_board = petri_dishes[0].get_board();

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
                            for (PetriDish& dish : petri_dishes) {
                                dish.foward();
                            }
                        }
                    }

                    // Printing
                    board_printer.print(main_board, timer, status, generation);

                    // End measurement and sync
                    timer.end_measurement();
                    if (status.syncing) timer.sync(target_delta_time);
                }
            }

        private:
            unsigned generation = 0;
            double target_fps = 4.0;
            double target_delta_time = 1.0 / 4.0;

            SimStatus status;

            std::mt19937_64 rng_gen;

            Term& term;
            std::vector<PetriDish> petri_dishes;
            printer::Printer board_printer;
    };
}