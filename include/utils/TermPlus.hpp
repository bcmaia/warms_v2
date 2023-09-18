#pragma once


#include <ncurses.h>
//#include <locale.h>
#include <wchar.h>
#include <string>
#include <cstring>
#include <stdexcept>
//#include <cstdio>
#include <cmath>

#include "Term.hpp"
#include "Vec.hpp"


namespace term {
    class TermPlus {
        public:
            static TermPlus& instance() {
                static TermPlus instance;
                return instance;
            }

            inline void refresh () const {term.refresh();}
            IVec2 get_dimensions () const {return dimensions;}

            void printat (
                const IVec2 p, 
                const std::wstring s, 
                const int color_pair = 0
            ) {
                term.printat(p.x(), p.y(), s, color_pair);
            }

            void printat (
                const IVec2 p, 
                const std::string s, 
                const int color_pair = 0
            ) {
                term.printat(p.x(), p.y(), s, color_pair);
            }

            void printat (
                const IVec2 p, 
                const char* s, 
                const int color_pair = 0
            ) {
                term.printat(p.x(), p.y(), s, color_pair);
            }

            void printat (
                const IVec2 p, 
                const wchar_t* s, 
                const int color_pair = 0
            ) {
                term.printat(p.x(), p.y(), s, color_pair);
            }

            // Function to draw a square outline
            void square_outline (const IVec2 point, const int size) const {
                int x = point.x();
                int y = point.y();

                // Draw the top side
                mvhline(y, x, ACS_HLINE, size);

                // Draw the bottom side
                mvhline(y + size, x, ACS_HLINE, size);

                // Draw the left side
                mvvline(y, x, ACS_VLINE, size);

                // Draw the right side
                mvvline(y, x + size, ACS_VLINE, size);

                // Draw the corners
                mvaddch(y, x, ACS_ULCORNER);
                mvaddch(y, x + size, ACS_URCORNER);
                mvaddch(y + size, x, ACS_LLCORNER);
                mvaddch(y + size, x + size, ACS_LRCORNER);
            }

            void rect_outline (const IVec2 p1, const IVec2 p2) {
                int width = std::abs(
                    static_cast<int>(p1.x()) - static_cast<int>(p2.x())
                );

                int height = std::abs(
                    static_cast<int>(p1.y()) - static_cast<int>(p2.y())
                );

                int x_min = static_cast<int>(p1.x() < p2.x() ? p1.x() : p2.x());
                int y_min = static_cast<int>(p1.y() < p2.y() ? p1.y() : p2.y());

                int x_max = static_cast<int>(p1.x() >= p2.x() ? p1.x() : p2.x());
                int y_max = static_cast<int>(p1.y() >= p2.y() ? p1.y() : p2.y());

                // Draw the top side
                mvhline(y_min, x_min, ACS_HLINE, width);

                // Draw the bottom side
                mvhline(y_max, x_min, ACS_HLINE, width);

                // Draw the left side
                mvvline(y_min, x_min, ACS_VLINE, height);

                // Draw the right side
                mvvline(y_min, x_max, ACS_VLINE, height);

                // Draw the corners
                mvaddch(y_min, x_min, ACS_ULCORNER);
                mvaddch(y_min, x_max, ACS_URCORNER);
                mvaddch(y_max, x_min, ACS_LLCORNER);
                mvaddch(y_max, x_max, ACS_LRCORNER);
            }

        private:
            Term& term;
            IVec2 dimensions;

            TermPlus () : 
                term(Term::instance()),
                dimensions(IVec2(
                    static_cast<uint32_t>(Term::instance().get_width()),
                    static_cast<uint32_t>(Term::instance().get_height())
                ))
            {}
    };
}
