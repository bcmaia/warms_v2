#pragma once


#include <vector>
#include <mutex>
#include <random>
#include <stdexcept>

#include "Population/Agent.hpp"
#include "Population/Goats.hpp"
#include "Sim/sim_constants.hpp"


namespace population {
    using Goats = population::Goats;

    class Creator {
        public:
            static Creator& instance () {
                static Creator creator;
                return creator;
            }

            void clear () {
                //std::lock_guard<std::mutex> lock(mutex_);
                memory.clear();
                memory = goats.get_goats();
            }

            void save (const std::vector<Agent>& agents) {
                //std::lock_guard<std::mutex> lock(mutex_);

                goats.save(agents);

                // Append agents to memory using insert
                memory.insert(memory.end(), agents.begin(), agents.end());
            }

            const Agent& get_rand_agent (const uint64_t seed) const {
                #ifndef RELEASE
                    if (0 >= memory.size()) throw std::out_of_range("Memory is empty.");
                #endif

                std::mt19937_64 rng_gen(seed);
                std::uniform_int_distribution<int32_t> dist (0, memory.size() - 1);

                return memory[dist(rng_gen)];
            }

            const Agent& get_parent (const uint64_t seed) const {
                std::mt19937_64 rng_gen(seed);

                uint64_t new_seed = rng_gen();
                const Agent& option_a = get_rand_agent(new_seed);

                new_seed = rng_gen();
                const Agent& option_b = get_rand_agent(new_seed);

                return option_a.get_fitness() > option_b.get_fitness() ? option_a : option_b;
            }

            Agent create (const uint64_t seed, uint32_t id) const {
                std::mt19937_64 rng_gen(seed);

                uint64_t new_seed = rng_gen();
                const Agent& daddy = get_parent(new_seed);

                new_seed = rng_gen();
                const Agent& mommy = get_parent(new_seed);

                new_seed = rng_gen();
                return Agent(daddy, mommy, new_seed, id);
            }

            std::vector<Agent> populate (
                const uint64_t seed,
                const size_t pop_size
            ) const {
                std::vector<Agent> population;
                std::mt19937_64 rng_gen(seed);

                population.clear();

                for (size_t i = 0; i < pop_size; i++) {
                    uint64_t new_seed = rng_gen();
                    population.push_back( create(new_seed, i) );
                }

                return population;
            }
        private:
            Creator () : goats(Goats::instance()) {}
            Goats& goats;
            std::vector<Agent> memory;
            std::mutex mutex_;
    };
}