#pragma once


#include <cstdint>

#include "Sim/sim_constants.hpp"
#include "Sim/Board.hpp"
#include "Sim/Population.hpp"
#include "utils/Vec.hpp"
#include "utils/Term.hpp"


namespace petridish {
    using Population = population::Population;
    using Board = board::Board;

    class PetriDish {
        public:
            PetriDish(uint64_t seed, size_t pop_size = sim::POPULATION_SIZE) : 
                rng_gen(seed) 
            {
                uint64_t board_seed = rng_gen();

                board = Board(
                    term::Term::instance().get_width() - 2,
                    term::Term::instance().get_height() - 2,
                    board_seed
                );

                uint64_t population_seed = rng_gen();
                population = Population(pop_size, population_seed, board);
            }

            ~PetriDish() {}

            void start_now () {halt = false;}
            void halt_now () {halt = true;}

            void foward () {
                if (halt) return;

                board.foward();
                population.foward(board);

                if (0 >= population.get_live_pop_size()) halt_now ();
            }

            const Board& get_board () {return board;}

        private:
            bool halt = false;
            std::mt19937_64 rng_gen;
            Board board;
            Population population;
    };
}