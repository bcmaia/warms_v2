#pragma once


#include <cstdint>
#include <random>
#include <sstream>

#include "utils/types.hpp"
#include "utils/Vec.hpp"
#include "utils/VecPlus.hpp"
#include "Sim/sim_constants.hpp"
#include "Sim/Board.hpp"
#include "Sim/Board/Cell.hpp"

#include "utils/Term.hpp"


namespace agent {
    using SimpleDir = types::SimpleDir;
    using Board = board::Board;


    class Agent {
        public:
            Agent () {}
            Agent (
                const uint64_t seed,
                const IVec2 position,
                Board& board,
                const int32_t agent_id
            ) : 
                id(agent_id)
            {
                std::mt19937_64 rng_gen (seed);
                std::uniform_int_distribution dir_dist (1, 4);
                SimpleDir dir = static_cast<SimpleDir>( dir_dist(rng_gen) );
                //dir = SimpleDir::Up;

                head_dir = dir;
                neck_dir = dir;
                tail_dir = dir;
                decided_dir = dir;

                IVec2 temp = position ; //% board.get_dimensions();
                head_position = temp;
                tail_position = temp;
                neck_position = temp;
                set_head(board);
            }

            Agent (
                const uint64_t seed,
                const IVec2 position,
                const SimpleDir dir,
                Board& board,
                const int32_t agent_id
            ) : 
                id(agent_id)
            {
                head_dir = dir;
                neck_dir = dir;
                tail_dir = dir;
                decided_dir = dir;

                IVec2 temp = position ; //% board.get_dimensions();
                head_position = temp;
                tail_position = temp;
                neck_position = temp;
                set_head(board);
            }

            void think () {
                if (!alive) return; 
                //head_dir = 
            }

            void decide_movement (Board& board) {
                if (!alive) return; 

                neck_dir = head_dir;
                neck_position = head_position;
                head_dir = decided_dir;

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
                if (!alive) return; 

                set_neck(board);
                set_head(board);
            }

            void feed (Board& board) {
                if (board.get_raw(head_position).is_food()) 
                    energy += sim::FOOD_ENERGY;
            }

            void move_tail (Board& board) {
                if (!alive) return; 

                // If we need to grow, do not move the tail
                if (potential_length () > length) {length++; return;}

                // move the tail if potential_length() <= length
                tail_foward (board);

                // If we need to shrink, move the tail once more
                if (potential_length() < length) {
                    length--;
                    tail_foward (board);
                }
            }

            bool check_for_static_colision (Board& board) {
                return board.get_raw(head_position).is_solid();
            }

            bool has_starved () {return 0.0 > energy;}
            void expend_energy () {energy -= 1.0;}

            bool is_alive () const {return alive;}
            bool is_dying () const {return dying;}
            IVec2 get_head_position () const {return head_position;}
            int32_t get_id () const {return id;}
            void start_dying () {dying = true;}
            int32_t get_entanglement () const {return tangled_with;}

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
                sensorial_index = 0;
                short_sight(board);
                far_sight(board);
            }

            void short_sight (const Board& board) {
                
            }

            void far_sight (const Board& board) {
            
            }

            void die (Board& board) {
                if (!alive) return; // You can only die once
                alive = false;
                dying = false;
                
                // NOTE: if we checked all the cells, then we should probabily
                //  stop because an error has ocurred. That's why we are comparing
                //  i to board.get_length().
                for (size_t i = 0; i < board.get_length(); i++) {
                    put_meat (board, tail_position);

                    tail_position = board.to_valid_position(
                        vec::vec_extrapolate(tail_position, tail_dir)
                    );

                    cell::Cell c = board.get_raw(tail_position);
                    tail_dir = c.get_dir();
                    if (c.get_id() != id) break;
                }
            }

        private:
            size_t sensorial_index;
            float sensorial_data [sim::SENSORIAL_DATA];

            bool alive = true;
            bool dying = false;
            int32_t tangled_with = -1;

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
                    cell::Cell(cell::CellType::SnakeBody, 1, neck_dir, id)
                );
            }

            void set_head (Board& board) const {
                board.set_raw(
                    head_position, 
                    cell::Cell(cell::CellType::SnakeHead, 1, head_dir, id)
                );
            }

            void tail_foward (Board& board) {
                set_tail (board, tail_position);
                tail_position = board.to_valid_position(
                    vec::vec_extrapolate<2, int32_t>(tail_position, tail_dir)
                );
                cell::Cell c = board.get_raw(tail_position);

                if (c.get_id() == id) {
                    tail_dir = c.get_dir();
                } else {
                    tangled_with = c.get_id();
                }
            }

            // TODO: 
            void recalculate_length () {

            }

            // HOT FIX
            // NOTE: Optimise later
            void search_for_tail (Board& board) {
                for (int32_t range = 1; range < sim::MAX_ATEMPTS; range++) {
                    for (int32_t y = -range; y < range; y++) {
                        for (int32_t x = -range; x < range; x++) {
                            IVec2 p = board.to_valid_position(IVec2(x, y));
                            cell::Cell c = board.get_raw(p);
                            if (c.get_id() == id) {
                                tail_position = p;
                                tail_dir = c.get_dir();
                                return;
                            }
                        }
                    }
                }
            }

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
