#pragma once


#include <sstream>

#include "utils/Term.hpp"
#include "utils/TermPlus.hpp"
#include "utils/Timer.hpp"
#include "Sim/sim_types.hpp"
#include "Sim/Board.hpp"


namespace printer {
    using Term = term::Term;
    using TermPlus = term::TermPlus;
    using Board = board::Board;
    using Timer = timer::Timer;
    using SimStatus = sim::SimStatus;

    class Printer {
        public:
            Printer () : term(Term::instance()), term_plus(TermPlus::instance()) {}
            ~Printer () {}

            void print(
                const Board& board, 
                const Timer& timer, 
                const SimStatus& sim_status,
                const unsigned generation
            ) {
                if (sim_status != saved_status) {
                    saved_status = sim_status;

                    print_all(board);
                    print_edges();
                    print_status(sim_status);
                    print_timer(timer);
                    print_generation(generation);

                    if (sim_status.paused) print_paused();
                } else if (sim_status.printing && !sim_status.paused) {
                    bool board_is_of_diff_size = (
                        0 >= state.get_length() 
                        || state.get_dimensions() != board.get_dimensions()
                    );

                    if (board_is_of_diff_size) print_all(board);
                    else print_diff(board);
                    print_edges();
                    print_status(sim_status);
                    print_timer(timer);
                    print_generation(generation);
                } 
            }


        private:
            Term& term;
            TermPlus& term_plus;
            Board state;
            SimStatus saved_status;

            inline void print_all (const Board& board) {
                state = board;

                UVec2 p;
                UVec2 board_dim = board.get_dimensions();
                for (p.y() = 0; p.y() < board_dim.y(); p.y() += 1) {
                    for (p.x() = 0; p.x() < board_dim.x(); p.x() += 1) {
                        cell::Cell c = board.get_raw(p);
                        term.printat(p.x(), p.y(), c.to_str(), c.get_color());
                    }
                }
            }

            inline void print_diff (const Board& board) {
                UVec2 p;
                UVec2 board_dim = board.get_dimensions();
                for (p.y() = 0; p.y() < board_dim.y(); p.y() += 1) {
                    for (p.x() = 0; p.x() < board_dim.x(); p.x() += 1) {
                        cell::Cell c = board.get_raw(p);

                        // If there is no difference, we do not print
                        if (state.get_raw(p) == c) continue;

                        // Save difference
                        state.set(p, c);

                        // Print difference
                        term.printat(p.x(), p.y(), c.to_str(), c.get_color());
                    }
                }
            }

            inline void print_edges () const {
                term_plus.rect_outline(
                    UVec2(0u, 0u), 
                    term_plus.get_dimensions() - UVec2(1u, 1u)
                );
            }

            inline void print_timer (const Timer& timer) const {
                term.printat(
                    term.get_width() - 101, 
                    term.get_height() - 1,
                    timer.to_str()
                );
            }

            inline void print_status (const SimStatus& sim_status) {
                std::wstringstream wss;
                wss << "<" 
                    << "sync: " << (sim_status.syncing ? "ON" : "OFF") << ", "
                    << "powersave: " << (sim_status.powersave ? "ON" : "OFF") << ", "
                    << "ui: " << (sim_status.printing ? "ON" : "OFF") << ", "
                    << ">";

                term.printat(5, 0, wss.str());
            }

            // NOTE: the code might error here due to term index out of bounds
            inline void print_paused () const {
                unsigned x = term.get_width() / 2 - 8;
                unsigned y = term.get_height() / 2;

                term.printat(x, y - 2,  L"              ");
                term.printat(x, y - 1,  L"  ╭────────╮  ");
                term.printat(x, y,      L"  │ PAUSED │  ");
                term.printat(x, y + 1,  L"  ╰────────╯  ");
                term.printat(x, y + 2,  L"              ");
            } 

            inline void print_generation (const unsigned generation) {
                std::wstringstream wss;
                wss << "<Generation: " << generation << ">";
                term.printat(term.get_width() - 24, 0,  wss.str());
            }
    };
}