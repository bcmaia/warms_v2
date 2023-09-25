// #pragma once


// #include <vector>

// #include "Sim/sim_constants.hpp"
// #include "Sim/Board/Cell.hpp"
// #include "utils/Vec.hpp"
// #include "utils/VecPlus.hpp"
// #include "utils/types.hpp"


// namespace board {
//     using Cell = cell::Cell;
//     using cell_vector = std::vector<Cell>;
//     using cell_matrix = std::vector<cell_vector>;

//     class Board {
//         public:
//             Board () {}

//             Board (const IVec2 board_dimensions, const uint64_t seed) : 
//                 dimensions(board_dimensions),
//                 length(board_dimensions.x() * board_dimensions.y()),
//                 rng_gen(seed)
//             {
//                 matrix = cell_matrix(
//                     dimensions.y(), 
//                     cell_vector(dimensions.x(), Cell::Empty())
//                 );
                
//                 procedural_obstacles(5);
//                 make_board_grate_again();
//             }

//             Board (unsigned board_width, unsigned board_height, uint64_t seed) 
//                 : Board(IVec2(board_width, board_height), seed)
//             {}

//             ~Board () {}

//             void foward () {
//                 add_food();
//             }

//             void procedural_obstacles (short amount = 5) {
//                 for (short a = 0; a < amount; a++) {
//                     cell_matrix temp = cell_matrix(
//                         dimensions.y(), 
//                         cell_vector(dimensions.x(), Cell::Empty())
//                     );

//                     IVec2 p;
//                     std::uniform_real_distribution<float> dist (0.0, 1.0);
//                     for (p.y() = 0; p.y() < dimensions.y(); p.y() += 1) {
//                         for (p.x() = 0; p.x() < dimensions.x(); p.x() += 1) {
//                             short i = sorrounding_walls(p);

//                             float v;
//                             switch (i) {
//                                 case 0: v = sim::PROCEDURAL_WALLS_FACTOR_0; break;
//                                 case 1: v = sim::PROCEDURAL_WALLS_FACTOR_1; break;
//                                 case 2: v = sim::PROCEDURAL_WALLS_FACTOR_2; break;
//                                 case 3: v = sim::PROCEDURAL_WALLS_FACTOR_3; break;
//                                 case 4: v = sim::PROCEDURAL_WALLS_FACTOR_4; break;
//                                 default: v = 0.01;
//                             }

//                             if (dist(rng_gen) <= v) temp[p.y()][p.x()] = Cell::Wall();
//                             else temp[p.y()][p.x()] = get_raw(p);
//                         }
//                     }

//                     for (p.y() = 0; p.y() < dimensions.y(); p.y() += 1) {
//                         for (p.x() = 0; p.x() < dimensions.x(); p.x() += 1) {
//                             set(p, temp[p.y()][p.x()]);
//                         }
//                     }

//                 }
//             }

//             void make_board_grate_again() {
//                 for (int32_t y = 0; y < dimensions.y(); y++) {
//                     set_raw(IVec2(0, y), Cell::Wall());
//                     set_raw(IVec2(dimensions.x() - 1, y), Cell::Wall());
//                 }

//                 for (int32_t x = 0; x < dimensions.x(); x++) {
//                     set_raw(IVec2(x, 0), Cell::Wall());
//                     set_raw(IVec2(x, dimensions.y() - 1), Cell::Wall());
//                 }
//             }

//             short sorrounding_walls (const IVec2 p) const {
//                 return (
//                     ( get(p + IVec2::Up()).is_wall() ? 1 : 0)
//                     + ( get(p + IVec2::Right()).is_wall() ? 1 : 0 )
//                     + ( get(p + IVec2::Down()).is_wall() ? 1 : 0 )
//                     + ( get(p + IVec2::Left()).is_wall() ? 1 : 0 )
//                 );
//             }

//             IVec2 to_valid_position (const IVec2 position) const {
//                 return ((position % dimensions) + dimensions) % dimensions;
//             }

//             Cell get (
//                 const IVec2 pivot, 
//                 const IVec2 relative_point,
//                 const types::SimpleDir dir
//             ) const {
//                 IVec2 p = pivot + vec::rotate<int32_t>(relative_point, dir);
//                 return get(p);
//             }

//             Cell get (const IVec2 position) const {
//                 IVec2 p = to_valid_position(position);
//                 return matrix[p.y()][p.x()];
//             }

//             void set (const IVec2 position, const Cell c) {
//                 IVec2 p = to_valid_position(position);
//                 matrix[p.y()][p.x()] = c;
//             }

//             Cell get_raw (const IVec2 position) const {
//                 return matrix[position.y()][position.x()];
//             }

//             void set_raw (const IVec2 position, const Cell c) {
//                 matrix[position.y()][position.x()] = c;
//             }

//             void die_by_id (int32_t id) {
//                 if (0 > id) return;

//                 IVec2 p = IVec2::Zero();
//                 for (p.y() = 0; p.y() < dimensions.y(); p.y() += 1) {
//                     for (p.x() = 0; p.x() < dimensions.x(); p.x() += 1) {
//                         Cell c = get_raw(p);
//                         if (c.is_organism() && c.get_id() == id) {
//                             set_raw(p, Cell::Food());
//                         }
//                     }
//                 }
//             }

//             void clear () {
//                 IVec2 p = IVec2::Zero();
//                 for (p.y() = 0; p.y() < dimensions.y(); p.y() += 1) {
//                     for (p.x() = 0; p.x() < dimensions.x(); p.x() += 1) {
//                         Cell c = get_raw(p);
//                         if (c.is_organism() || c.is_food()) {
//                             set_raw(p, Cell::Empty());
//                         }
//                     }
//                 }
//             }

//             IVec2 get_dimensions () const {return dimensions;}
//             std::mt19937_64 get_rng_gen () const {return rng_gen;}
//             size_t get_length () const {return length;}

//             Board& operator=(const Board& other) {
//                 if (this == &other) {return *this;}

//                 rng_gen = other.get_rng_gen();

//                 IVec2 other_dim = other.get_dimensions();

//                 if ((0 >= matrix.size()) || (dimensions != other_dim)) {
//                     matrix = cell_matrix(
//                         other_dim.y(),
//                         cell_vector(other_dim.x(), Cell::Empty())
//                     );

//                     dimensions = other_dim;
//                     length = other.get_length();
//                 } 

//                 IVec2 p = IVec2::Zero();

//                 for (p.y() = 0; p.y() < other_dim.y(); p.y() += 1) {
//                     for (p.x() = 0; p.x() < other_dim.x(); p.x() += 1) {
//                         set(p, other.get(p));
//                     }
//                 }

//                 return *this;
//             }

//             std::vector<IVec2> get_empty_positions (
//                 size_t number_of_cells, 
//                 bool clear_reserved = true
//             ) {
//                 std::uniform_int_distribution<uint32_t> width_dist (0, dimensions.x() - 1);
//                 std::uniform_int_distribution<uint32_t> height_dist (0, dimensions.y() - 1);

//                 std::vector<IVec2> result;
//                 //result.reserve(number_of_cells);

//                 IVec2 p;
//                 for (size_t i = 0; i < number_of_cells; i++) {
//                     for (uint8_t a = 0; a < sim::MAX_ATEMPTS; a++) {
//                         p.x() = width_dist(rng_gen);
//                         p.y() = height_dist(rng_gen);

//                         if (get(p).is_empty()) {
//                             set(p, Cell::Reserved());
//                             result.push_back(p);
//                             break;
//                         }
//                     }
//                 }

//                 if (clear_reserved) {
//                     for (IVec2& position : result) {
//                         set(position, Cell::Empty());
//                     }
//                 }

//                 return result;
//             }

//         private:
//             std::mt19937_64 rng_gen;
//             IVec2 dimensions = IVec2::Zero();
//             size_t length = 0;
//             cell_matrix matrix;

//             void add_food () {
//                 std::uniform_int_distribution<uint32_t> width_dist (0, dimensions.x() - 1);
//                 std::uniform_int_distribution<uint32_t> height_dist (0, dimensions.y() - 1);

//                 IVec2 p;
//                 for (uint8_t a = 0; a < sim::MAX_ATEMPTS; a++) {
//                     p.x() = width_dist(rng_gen);
//                     p.y() = height_dist(rng_gen);

//                     if (get(p).is_empty()) {
//                         set(p, Cell::Food());
//                         break;
//                     }
//                 }

//                 //set(IVec2(3u, 3u), Cell::Food());
//             }
//     };
// }

#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>
#include <random>

#include "Sim/sim_constants.hpp"
#include "Sim/Board/Cell.hpp"
#include "utils/Vec.hpp"
#include "utils/VecPlus.hpp"
#include "utils/types.hpp"

namespace board {
    using Cell = cell::Cell;
    using cell_vector = std::vector<Cell>;
    using cell_matrix = std::vector<cell_vector>;

    class Board {
        public:
            Board();
            Board(const IVec2 board_dimensions, const uint64_t seed);
            Board(unsigned board_width, unsigned board_height, uint64_t seed);
            ~Board();

            void foward();
            void procedural_obstacles(short amount = 5);
            void make_board_grate_again();
            short sorrounding_walls(const IVec2 p) const;
            IVec2 to_valid_position(const IVec2 position) const;
            Cell get(const IVec2 pivot, const IVec2 relative_point, const types::SimpleDir dir) const;
            Cell get(const IVec2 position) const;
            void set(const IVec2 position, const Cell c);
            Cell get_raw(const IVec2 position) const;
            void set_raw(const IVec2 position, const Cell c);
            void die_by_id(int32_t id);
            void clear();
            IVec2 get_dimensions() const;
            std::mt19937_64 get_rng_gen() const;
            size_t get_length() const;
            Board& operator=(const Board& other);
            std::vector<IVec2> get_empty_positions(size_t number_of_cells, bool clear_reserved = true);

        private:
            std::mt19937_64 rng_gen;
            IVec2 dimensions = IVec2::Zero();
            size_t length = 0;
            cell_matrix matrix;
            void add_food();
    };
}
