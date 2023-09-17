#pragma once


#include "Sim/Board.hpp"
#include "Sim/Population.hpp"
#include "utils/Vec.hpp"
#include "utils/Term.hpp"


namespace petridish {
    using Population = population::Population;
    using Board = board::Board;

    class PetriDish {
        public:
            PetriDish(uint64_t seed) : rng_gen(seed) {
                uint64_t board_seed = rng_gen();

                board = Board(
                    term::Term::instance().get_width() - 2,
                    term::Term::instance().get_height() - 2,
                    board_seed
                );
            }

            ~PetriDish() {}

            void foward () {
                board.foward();
            }

            const Board& get_board () {return board;}

        private:
            std::mt19937_64 rng_gen;
            Board board;
            Population population;
    };
}