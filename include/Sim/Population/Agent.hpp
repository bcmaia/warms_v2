#pragma once


#include <cstdint>
#include <random>
#include <sstream>

#include <cmath>
#include <limits>

#include "utils/types.hpp"
#include "utils/Vec.hpp"
// #include "utils/VecPlus.hpp"
#include "Sim/sim_constants.hpp"
#include "Sim/Board.hpp"
#include "Sim/Board/Cell.hpp"
#include "Population/Genome.hpp"

#include "utils/Term.hpp"
#include "utils/mymath.hpp"


namespace agent {
    using SimpleDir = types::SimpleDir;
    using Board = board::Board;


    class Agent {
        public:
            Agent () {}
            Agent (
                const uint64_t seed,
                const int32_t agent_id
            ) : 
                id(agent_id),
                rng_gen(seed)
            {
                std::uniform_int_distribution dir_dist (1, 4);
                SimpleDir dir = static_cast<SimpleDir>( dir_dist(rng_gen) );

                uint64_t new_seed = rng_gen();
                genome = Genome(new_seed);

                head_dir = dir;
                neck_dir = dir;
                tail_dir = dir;
                decided_dir = dir;
            }

            Agent (const uint64_t seed) : genome(seed) {

            } 

            Agent (
                const uint64_t seed,
                const SimpleDir dir,
                const int32_t agent_id
            ) : 
                id(agent_id),
                rng_gen(seed)
            { 
                head_dir = dir;
                neck_dir = dir;
                tail_dir = dir;
                decided_dir = dir;

                uint64_t new_seed = rng_gen();
                genome = Genome(new_seed);
            }

            Agent (
                const Agent& daddy, 
                const Agent& mommy, 
                const uint64_t seed, 
                const int32_t agent_id
            ) :
                id(agent_id),
                rng_gen(seed)
            {
                std::uniform_int_distribution dir_dist (1, 4);
                SimpleDir dir = static_cast<SimpleDir>( dir_dist(rng_gen) );

                uint64_t new_seed = rng_gen();
                genome = Genome(daddy.get_genome(), mommy.get_genome(), new_seed);

                head_dir = dir;
                neck_dir = dir;
                tail_dir = dir;
                decided_dir = dir;
            }

            void put_on_board (Board& board, const IVec2 position) {
                IVec2 temp = position; //% board.get_dimensions();
                head_position = temp;
                tail_position = temp;
                neck_position = temp;

                set_head(board);
            }

            void think () {
                if (!alive || dying) return; 

                // float results [sim::NN_OUTPUT_SIZE];
                uint32_t decision;
                uint64_t new_seed = rng_gen();
                if constexpr (sim::USE_STD_VEC_FOR_INPUTS) {
                    decision = genome.think(sensorial_data_vec.data(), new_seed);
                } else {
                    decision = genome.think(sensorial_data, new_seed);
                }

                // mymath::softmax<sim::NN_OUTPUT_SIZE>(results);

                // uint64_t new_seed = rng_gen();
                // size_t decision = mymath::rand_index<sim::NN_OUTPUT_SIZE>(results, new_seed);

                if (0 == decision) decided_dir = types::rotate(head_dir, true);
                else if (1 == decision) decided_dir = types::rotate(head_dir, false);
                // else if decision == 3 then keep same direction
            }

            void decide_movement (Board& board) {
                if (!alive || dying ) return; 

                neck_position = head_position;
                head_dir = decided_dir;
                neck_dir = decided_dir;

                head_position = board.to_valid_position(
                    vec::vec_extrapolate<2, int32_t>(head_position, head_dir)
                );
            }

            IVec2 next_position (Board& board) {
                return board.to_valid_position(
                    vec::vec_extrapolate<2, int32_t>(head_position, head_dir)
                );
            }

            void move_head (Board& board) {
                if (!alive || dying ) return; 

                if (2 < length) set_neck(board);
                set_head(board);
            }

            void feed (Board& board) {
                if (!alive || dying ) return; 
                if (board.get_raw(head_position).is_food()) {
                    energy += sim::FOOD_ENERGY;
                    fitness += sim::FOOD_FITNESS_REWARD;
                }
            }

            void move_tail (Board& board) {
                if (!alive) return; 
                if (dying) {
                    if (entangled) return;
                    if (0 >= length) {
                        alive = false;
                        dying = false;
                        return;
                    } else {
                        tail_foward(board, cell::Cell::Food());
                        length--;
                    }
                    return;
                }

                // If we need to grow, do not move the tail
                if (potential_length () > length) {length++; return;}

                // move the tail if potential_length() <= length
                tail_foward (board, cell::Cell::Empty());

                // If we need to shrink, move the tail once more
                if (potential_length() < length) {
                    length--;
                    tail_foward (board, cell::Cell::Empty());
                }
            }

            bool check_for_static_colision (Board& board) {
                return board.get_raw(head_position).is_solid();
            }

            bool has_starved () {return 0.0 > energy;}
            void expend_energy () {energy -= 1.0;}

            bool is_alive () const {return alive;}
            bool is_entangled () const {return entangled;}
            bool is_dying () const {return dying;}
            IVec2 get_head_position () const {return head_position;}
            int32_t get_id () const {return id;}
            void start_dying () {dying = true;}
            int32_t get_entangled_with () const {return entangled_with;}
            float get_fitness () const {return fitness;}
            const Genome& get_genome () const {return genome;}

            void set_fitness (const float new_fitness) {fitness = new_fitness;}
            void inc_fitness (const float increment) {fitness += increment;}

            void survival_reward () {
                if (!alive || dying ) return; 
                fitness += sim::SURVIVAL_FITNESS_REWARD;
            }


            uint32_t potential_length () const {
                return (
                    sim::MIN_SNAKE_LENGTH 
                    + static_cast<uint32_t>(energy / sim::ENERGY_PER_SNAKE_CELL)
                );
            }

            void set_dead () {
                dying = false;
                alive = false;
            }

            void feel (const Board& board) {
                if (!alive || dying ) return; 

                if constexpr (sim::USE_STD_VEC_FOR_INPUTS) sensorial_data_vec.clear();
                else sensorial_index = 0;

                short_sight(board);
                far_sight(board);

                std::stringstream ss;

                if constexpr (sim::USE_STD_VEC_FOR_INPUTS) {
                    ss << "Vector Size: < " << sensorial_data_vec.size() << " >";
                    term::Term::instance().printat(1, 1, ss.str());
                }
            }

            void short_sight (const Board& board) {
                for (int32_t i = -3; i <= 3; i++) {
                    for (int32_t j = -3; j <= 3; j++) {
                        if (0 == i && 0 == j) continue;
                        cell::Cell c = board.get(head_position, IVec2(j, i), head_dir);
                        fell_cell(c);
                    }
                }
            }

            void far_sight (const Board& board) {
                for (int32_t i = -1; i <= +1; i++) { // 'outer_loop
                    for (int32_t j = -1; j <= +1; j++) { // 'mid_loop
                        if (0 == i && 0 == j) continue;

                        for (int32_t range = 1; range < 16; range++) { // 'inner_loop
                            cell::Cell c = board.get(head_position, range * IVec2(j, i), head_dir);
                            if (c.is_interesting()) {
                                fell_cell(c);
                                goto continue_mid_loop;
                            }
                        }

                        {
                            cell::Cell c = board.get(head_position, 16 * IVec2(j, i), head_dir);
                            fell_cell(c);
                        }

                        continue_mid_loop: continue;
                    }
                }
            }

            void fell_cell (const cell::Cell c) {
                if constexpr (sim::USE_STD_VEC_FOR_INPUTS) {
                    sensorial_data_vec.push_back(c.is_snake_head() ? 1.0 : 0.0);
                    sensorial_data_vec.push_back(c.is_snake_body() ? 1.0 : 0.0);
                    sensorial_data_vec.push_back(c.is_snake_tail() ? 1.0 : 0.0);
                    sensorial_data_vec.push_back(c.is_food() ? 1.0 : 0.0);
                    sensorial_data_vec.push_back(c.is_wall() ? 1.0 : 0.0);
                    sensorial_data_vec.push_back(c.is_solid() ? 1.0 : 0.0);
                } else {
                    sensorial_data[sensorial_index + 0] = c.is_snake_head() ? 1.0 : 0.0;
                    sensorial_data[sensorial_index + 1] = c.is_snake_body() ? 1.0 : 0.0;
                    sensorial_data[sensorial_index + 2] = c.is_snake_tail() ? 1.0 : 0.0;
                    sensorial_data[sensorial_index + 3] = c.is_wall() ? 1.0 : 0.0;
                    sensorial_data[sensorial_index + 4] = c.is_wall() ? 1.0 : 0.0;
                    sensorial_data[sensorial_index + 5] = c.is_solid() ? 1.0 : 0.0;
                    sensorial_index += 6;
                }
            }

            void die (Board& board) {
                if (!alive) return; // You can only die once
                alive = false;
                dying = false;
                
                // NOTE: if we checked all the cells, then we should probabily
                //  stop because an error has ocurred. That's why we are comparing
                //  i to board.get_length().
                for (; 0 >= length; length--) {
                    cell::Cell c = board.get_raw(tail_position);
                    tail_dir = c.get_dir();

                    if ( c.get_id() != id || !(c.is_organism()) ) {
                        if (0 >= length) break;
                        // else
                        alive = true;
                        dying = true;
                        break;
                    }

                    put_meat (board, tail_position);

                    tail_position = board.to_valid_position(
                        vec::vec_extrapolate(tail_position, tail_dir)
                    );
                }
            }

        private:
            std::mt19937_64 rng_gen;
            Genome genome;

            size_t sensorial_index;
            float sensorial_data [sim::SENSORIAL_DATA] = {};
            std::vector<float> sensorial_data_vec;

            bool alive = true;
            bool dying = false;
            bool entangled = false;
            int32_t entangled_with = -1;

            int32_t id = 0;

            uint32_t length = 1;
            float energy = 5.0 * 16.0;
            float fitness = 0;

            SimpleDir decided_dir = SimpleDir::Up;

            SimpleDir head_dir = SimpleDir::Up;
            SimpleDir neck_dir = SimpleDir::Up;
            SimpleDir tail_dir = SimpleDir::Up;

            IVec2 head_position;
            IVec2 neck_position;
            IVec2 tail_position;

            void set_neck (Board& board) const {
                board.set_raw(
                    neck_position, 
                    cell::Cell(cell::CellType::SnakeBody, genome.get_color(), neck_dir, id)
                );
            }

            void set_head (Board& board) const {
                board.set_raw(
                    head_position, 
                    cell::Cell(cell::CellType::SnakeHead, genome.get_color(), head_dir, id)
                );
            }

            void tail_foward (Board& board, const cell::Cell leftover) {
                // set_tail (board, tail_position);

                cell::Cell c = board.get_raw(tail_position);
                tail_dir = c.get_dir();
        
                if (!(c.is_organism()) || id != c.get_id()) {
                    if (0 >= length) entangled = true;
                    return;
                }

                board.set_raw(tail_position, leftover);
                tail_position = board.to_valid_position(
                    vec::vec_extrapolate<2, int32_t>(tail_position, tail_dir)
                );

                c = board.get_raw(tail_position);

                if (c.is_organism() && c.get_id() == id) {
                    tail_dir = c.get_dir();
                    board.set_raw(tail_position, cell::Cell(cell::CellType::SnakeTail, genome.get_color(), tail_dir, id) );
                } else if (0 < length) {
                    entangled = true;
                    entangled_with = c.get_id();
                }
            }

            // void die_foward (Board& board) {
            //     // set_tail (board, tail_position);

            //     board.set_raw(tail_position, cell::Cell::Food());
            //     tail_position = board.to_valid_position(
            //         vec::vec_extrapolate<2, int32_t>(tail_position, tail_dir)
            //     );

            //     cell::Cell c = board.get_raw(tail_position);

            //     if (c.get_id() == id) {
            //         tail_dir = c.get_dir();
            //         board.set_raw(tail_position, cell::Cell(cell::CellType::SnakeTail, genome.get_color(), tail_dir, id) );
            //     } else if (0 < length) {
            //         entangled = true;
            //         entangled_with = c.get_id();
            //     }
            // }

            // // TODO: 
            // void recalculate_length () {

            // }

            // // HOT FIX
            // // NOTE: Optimise later
            // void search_for_tail (Board& board) {
            //     for (int32_t range = 1; range < sim::MAX_ATEMPTS; range++) {
            //         for (int32_t y = -range; y < range; y++) {
            //             for (int32_t x = -range; x < range; x++) {
            //                 IVec2 p = board.to_valid_position(IVec2(x, y));
            //                 cell::Cell c = board.get_raw(p);
            //                 if (c.get_id() == id) {
            //                     tail_position = p;
            //                     tail_dir = c.get_dir();
            //                     return;
            //                 }
            //             }
            //         }
            //     }
            // }

            void set_tail (Board& board, const IVec2 position) const {
                board.set_raw(
                    position, 
                    cell::Cell::Empty()
                );
            }

            void put_meat (Board& board, const IVec2 position) const {
                board.set_raw(
                    position, 
                    cell::Cell::Food()
                );
            }
    };
}
