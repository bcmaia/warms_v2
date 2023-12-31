#pragma once


#include <vector>

#include "Sim/sim_constants.hpp"
#include "Sim/Board/Cell.hpp"
#include "utils/Vec.hpp"


namespace board {
    using Cell = cell::Cell;
    using cell_vector = std::vector<Cell>;
    using cell_matrix = std::vector<cell_vector>;

    class Board {
        public:
            Board () {}

            Board (const UVec2 board_dimensions, const uint64_t seed) : 
                dimensions(board_dimensions),
                length(board_dimensions.x() * board_dimensions.y()),
                rng_gen(seed)
            {
                matrix = cell_matrix(
                    dimensions.y(), 
                    cell_vector(dimensions.x(), Cell::Empty())
                );
            }

            Board (unsigned board_width, unsigned board_height, uint64_t seed) 
                : Board(UVec2(board_width, board_height), seed)
            {}

            ~Board () {}

            void foward () {
                add_food();
            }

            Cell get (const UVec2 position) const {
                UVec2 p = position % dimensions;
                return matrix[p.y()][p.x()];
            }

            void set (const UVec2 position, const Cell c) {
                UVec2 p = position % dimensions;
                matrix[p.y()][p.x()] = c;
            }

            Cell get_raw (const UVec2 position) const {
                return matrix[position.y()][position.x()];
            }

            void set_raw (const UVec2 position, const Cell c) {
                matrix[position.y()][position.x()] = c;
            }

            UVec2 get_dimensions () const {return dimensions;}
            std::mt19937_64 get_rng_gen () const {return rng_gen;}
            size_t get_length () const {return length;}

            Board& operator=(const Board& other) {
                if (this == &other) {return *this;}

                rng_gen = other.get_rng_gen();

                UVec2 other_dim = other.get_dimensions();

                if ((0 >= matrix.size()) || (dimensions != other_dim)) {
                    matrix = cell_matrix(
                        other_dim.y(),
                        cell_vector(other_dim.x(), Cell::Empty())
                    );

                    dimensions = other_dim;
                } 

                UVec2 p = UVec2(0u, 0u);

                for (p.y() = 0; p.y() < other_dim.y(); p.y() += 1) {
                    for (p.x() = 0; p.x() < other_dim.x(); p.x() += 1) {
                        set(p, other.get(p));
                    }
                }

                return *this;
            }

        private:
            std::mt19937_64 rng_gen;
            UVec2 dimensions = UVec2::Zero();
            size_t length = 0;
            cell_matrix matrix;

            void add_food () {
                std::uniform_int_distribution<uint32_t> width_dist (0, dimensions.x() - 1);
                std::uniform_int_distribution<uint32_t> height_dist (0, dimensions.y() - 1);

                UVec2 p;
                for (uint8_t a = 0; a < sim::MAX_ATEMPTS; a++) {
                    p.x() = width_dist(rng_gen);
                    p.y() = height_dist(rng_gen);

                    if (get(p).is_empty()) {
                        set(p, Cell::Food());
                        break;
                    }
                }

                set(UVec2(3u, 3u), Cell::Food());
            }
    };
}