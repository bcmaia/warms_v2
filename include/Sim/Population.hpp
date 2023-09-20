#pragma once


#include <vector>
#include <cstdint>
#include <sstream>
#include <execution>

#include "Population/Agent.hpp"
#include "Population/Creator.hpp"
#include "Sim/Board.hpp"
#include "utils/types.hpp"

#include "utils/Term.hpp"


namespace population {
    using Agent = agent::Agent;
    using Board = board::Board;

    class Population {
        public:
            Population () {}

            Population (
                const size_t population_size, 
                const uint64_t seed,
                Board& board
            ) : 
                pop_size(population_size), 
                rnd_gen(seed)
            {
                // NOTE: it might be a good idea to reserve memory here
                std::vector<IVec2> positions = board.get_empty_positions(
                    pop_size,
                    false
                );

                // live_agents.push_back(
                //     Agent(1, IVec2(35+1, 5), types::SimpleDir::Down, board, 0)
                // );

                // live_agents.push_back(
                //     Agent(1, IVec2(30+1, 10+1), types::SimpleDir::Right, board, 1)
                // );
                
                size_t i;
                for (i = 0; i < pop_size && i < positions.size(); i++) {
                    uint64_t new_seed = rnd_gen();
                    live_agents.push_back(
                        Agent(new_seed, i)
                    );
                }

                i = 0;
                for (Agent& a : live_agents) {
                    if (i >= pop_size && i >= positions.size()) break;
                    a.put_on_board(board, positions[i++]);
                }
            }


            size_t get_live_pop_size () const {return live_agents.size();}

            void repopulate (Board& board) {
                dead_agents.clear();
                uint64_t new_seed = rnd_gen();
                live_agents = population::Creator::instance().populate(
                    new_seed,
                    pop_size
                );

                // NOTE: it might be a good idea to reserve memory here
                std::vector<IVec2> positions = board.get_empty_positions(
                    pop_size,
                    false
                );

                size_t i = 0;
                for (Agent& a : live_agents) {
                    if (i >= pop_size && i >= positions.size()) break;
                    a.put_on_board(board, positions[i++]);
                }
            }

            void foward (Board& board) {
                auto phase_1_decision_making = [&](Agent& a) {
                    a.feel(board);
                    a.think();
                    a.decide_movement(board);
                };

                auto phase_2_move_tail = [&](Agent& a) {
                    if constexpr (sim::LIVE_TAIL) return;
                    a.move_tail(board);
                };

                auto phase_3_death_checks = [&](Agent& a) {
                    bool time_to_die = a.is_alive() && (
                        a.has_starved()
                        || a.check_for_static_colision(board)
                        || check_head_on_collision(board, a.get_head_position())
                    );

                    if (time_to_die) {a.start_dying();}
                };

                // auto phase_4_murder_time = [&](Agent& a) {
                //     if (a.is_dying()) a.die(board);
                // };

                auto phase_5_move_head = [&](Agent& a) {
                    a.feed(board);
                    a.expend_energy();
                    a.move_head(board);
                    a.survival_reward();
                };

                std::for_each(
                    std::execution::par, 
                    live_agents.begin(), 
                    live_agents.end(), 
                    phase_1_decision_making
                );

                std::for_each(
                    std::execution::par, 
                    live_agents.begin(), 
                    live_agents.end(), 
                    phase_2_move_tail
                );

                std::for_each(
                    std::execution::par, 
                    live_agents.begin(), 
                    live_agents.end(), 
                    phase_3_death_checks
                );

                // Phase 3.5: entenglement check
                // NOTE: Cannot be done in parallel.
                for (Agent& a : live_agents) {
                    // If a is not entangled
                    if (!(a.is_entangled())) continue;
                    board.die_by_id(a.get_id());
                    a.set_dead();
                    
                    int32_t entangled_with = a.get_entangled_with();
                    if (0 > entangled_with || live_agents.size() <= entangled_with) continue;
                    board.die_by_id(entangled_with);
                    live_agents[entangled_with].set_dead();
                }

                // std::for_each(
                //     std::execution::par, 
                //     live_agents.begin(), 
                //     live_agents.end(), 
                //     phase_4_murder_time
                // );

                std::for_each(
                    std::execution::par, 
                    live_agents.begin(), 
                    live_agents.end(), 
                    phase_5_move_head
                );

                // for (Agent& a : live_agents) {
                //     if (a.is_dying()) {
                //         board.die_by_id(a.get_id());
                //         a.set_dead();
                //     }
                // }

                for (auto it = live_agents.begin(); it != live_agents.end();) {
                    if (!it->is_alive()) {
                        dead_agents.push_back(*it);
                        it = live_agents.erase(it);
                    } else {
                        ++it;
                    }
                }
            }

            size_t get_pop_size () const {return pop_size;}
            const std::mt19937_64& get_rnd_gen () const {return rnd_gen;}
            const std::vector<Agent>& get_live_agents () const {return live_agents;}
            const std::vector<Agent>& get_dead_agents () const {return dead_agents;} 

            Population& operator=(const Population& other) {
                if (&other == this) {return *this;}

                pop_size = other.get_pop_size();
                rnd_gen = other.get_rnd_gen();
                live_agents = other.get_live_agents();
                dead_agents = other.get_dead_agents();

                return *this;
            }

        private:
            size_t pop_size;
            std::mt19937_64 rnd_gen;
            std::vector<Agent> live_agents;
            std::vector<Agent> dead_agents;

            void get_surrounding_ids (
                int32_t ids [4],
                const Board& board, 
                const IVec2 position
            ) {
                ids[0] = board.get(position + IVec2::Up()).get_id();
                ids[1] = board.get(position + IVec2::Right()).get_id();
                ids[2] = board.get(position + IVec2::Down()).get_id();
                ids[3] = board.get(position + IVec2::Left()).get_id();

                if (ids[0] < 0 || ids[0] >= live_agents.size()) ids[0] = -1;
                if (ids[1] < 0 || ids[1] >= live_agents.size()) ids[1] = -1;
                if (ids[2] < 0 || ids[2] >= live_agents.size()) ids[2] = -1;
                if (ids[3] < 0 || ids[3] >= live_agents.size()) ids[3] = -1;
            }

            void get_surrounding_head_positions (
                IVec2 pos [4],
                const Board& board, 
                const IVec2 position, 
                const int32_t ids [4]
            ) {
                for (short i = 0; i < 4; i++) {
                    int32_t id = ids[i];

                    if (-1 == id) {
                        pos[i] = -IVec2::Ones();
                        continue;
                    }

                    pos[i] = live_agents[id].get_head_position();
                    pos[i] = board.to_valid_position(pos[i]);
                }
            }

            void print_surrounding_ids (const Board& board, const IVec2 position, int y) {
                int32_t ids [4];
                get_surrounding_ids(ids, board, position);
                std::stringstream ss;
                ss << "        ";
                ss << "<" << ids[0] << ", " << ids[1] << ", " << ids[2] << ", " << ids[3] << ">";
                ss << "        ";
                term::Term::instance().printat(2, y, ss.str(), 1);
            }

            bool check_head_on_collision (const Board& board, const IVec2 position) {
                if (board.get(position).get_id() != -1) return true;

                int32_t ids [4];
                get_surrounding_ids(ids, board, position);

                IVec2 positions [4];
                get_surrounding_head_positions(positions, board, position, ids);

                for (short i = 0; i < 4; i++) {
                    for (short j = 0; j < i; j++) {
                        if (
                           (-1 != ids[i])
                           && (-1 != ids[j])
                           && (ids[i] != ids[j])
                           && (positions[i] == positions[j])
                        ) return true;
                    }
                }

                return false; // no collision
            }
    };
}