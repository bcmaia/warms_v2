#pragma once


#include <iostream>
#include <vector>
#include <algorithm>
#include <mutex>
#include <random> // for random number generation

#include "Population/Agent.hpp"
#include "Sim/sim_constants.hpp"


namespace population {
    using Agent = agent::Agent;

    class Goats {
        public:
            static Goats& instance() {
                static Goats instance; // Guaranteed to be destroyed and instantiated on the first use.
                return instance;
            }

            void save(const Agent& agent) {
                //std::lock_guard<std::mutex> lock(mutex_);
                save_now(agent);
            }

            void save(const std::vector<Agent>& agents) {
                //std::lock_guard<std::mutex> lock(mutex_);

                for (const Agent& agent : agents) {
                    save_now(agent);
                }
            }

            void decay () {
                //std::lock_guard<std::mutex> lock(mutex_);
                for (Agent& agent : goats) {
                    agent.set_fitness( agent.get_fitness() * (1.0 - sim::GOATS_DECAY) );
                }
                std::stringstream ss;
                ss << "Bests: < " << goats.front().get_fitness() << ", " << goats.back().get_fitness() << "> " ;
                term::Term::instance().printat(1, 2, ss.str());
            }

            const Agent& get_rand_agent(uint64_t seed) const {
                //std::lock_guard<std::mutex> lock(mutex_);

                if (goats.empty()) {
                    throw std::runtime_error("No agents in the goats list.");
                }

                // Generate a random index within the range of available agents.
                std::mt19937 gen(seed);
                std::uniform_int_distribution<size_t> dist(0, goats.size() - 1);
                size_t randomIndex = dist(gen);

                return goats[randomIndex];
            }

            void print_goats() {
                //std::lock_guard<std::mutex> lock(mutex_);

                std::cout << "Top " << max_goats << " Agents:" << std::endl;
                for (const Agent& agent : goats) {
                    std::cout << "Fitness: " << agent.get_fitness() << ", Genome: [ ... ]" << std::endl; // Replace "..." with actual data printing logic.
                }
            }

        private:
            Goats() : max_goats(sim::GOATS_SIZE) { goats.push_back(Agent(0, 0)); }
            Goats(const Goats&) = delete;
            Goats& operator=(const Goats&) = delete;

            std::vector<Agent> goats;
            const size_t max_goats;
            std::mutex mutex_;

            inline void save_now (const Agent& agent) {
                if (goats.size() < max_goats) {
                    goats.push_back(agent);
                    std::sort(goats.begin(), goats.end(), [](const Agent& a, const Agent& b) {
                        return a.get_fitness() > b.get_fitness();
                    });
                } else {
                    if (agent.get_fitness() > goats.back().get_fitness()) {
                        goats.back() = agent;
                        std::sort(goats.begin(), goats.end(), [](const Agent& a, const Agent& b) {
                            return a.get_fitness() > b.get_fitness();
                        });
                    }
                }
            }
    };
}
