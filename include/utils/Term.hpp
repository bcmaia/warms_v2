#pragma once


#include <ncurses.h>
//#include <locale.h>
#include <wchar.h>
#include <string>
#include <cstring>
#include <stdexcept>
//#include <cstdio>


namespace term {
    /**
    * @brief A singleton class for managing terminal output and input using ncurses.
    */
    class Term {
        public:
            /**
            * @brief Get the singleton instance of the Term class.
            * @return A reference to the Term instance.
            */
            static Term& instance() {
                static Term instance;
                return instance;
            }

            /**
            * @brief Get the width of the terminal window.
            * @return The width of the terminal window in columns.
            */
            int get_width() const {
                return width;
            }

            /**
            * @brief Get the height of the terminal window.
            * @return The height of the terminal window in rows.
            */
            int get_height() const {
                return height;
            }

            /**
            * @brief Print a wide character string at a specified position on the terminal.
            * @param x The X-coordinate (column) of the position.
            * @param y The Y-coordinate (row) of the position.
            * @param text The wide character string to print.
            * @param color_pair The color pair to use for printing (default is 0).
            */
            void printat(int x, int y, const wchar_t* text, int color_pair = 0) const {
                #ifdef NO_CURSES
                    wprintf(L"%ls\n", text);
                    return;
                #endif

                // If the desired (x, y) coords are inside the terminal.
                if (x >= 0 && x < width && y >= 0 && y < height) {
                    setlocale(LC_ALL, "");
                    attron(COLOR_PAIR(color_pair));
                    mvaddwstr(y, x, text);
                    attroff(COLOR_PAIR(color_pair));
                } else {
                    // Handle out-of-bounds error
                    throw std::runtime_error("Can not print: position is out of bounds!\n");
                }
            }

            /**
            * @brief Print a string at a specified position on the terminal.
            * @param x The X-coordinate (column) of the position.
            * @param y The Y-coordinate (row) of the position.
            * @param text The string to print.
            * @param color_pair The color pair to use for printing (default is 0).
            */
            void printat(int x, int y, const std::string& text, int color_pair = 0) const {
                // Convert the std::string to a std::wstring
                std::wstring wtext(text.begin(), text.end());
                printat(x, y, wtext.c_str(), color_pair);
            }

            /**
            * @brief Print a wide character string at a specified position on the terminal.
            * @param x The X-coordinate (column) of the position.
            * @param y The Y-coordinate (row) of the position.
            * @param text The wide character string to print.
            * @param color_pair The color pair to use for printing (default is 0).
            */
            void printat(int x, int y, std::wstring text, int color_pair = 0) const {
                printat(x, y, text.c_str(), color_pair);
            }

            /**
            * @brief Print a C-style string at a specified position on the terminal.
            * @param x The X-coordinate (column) of the position.
            * @param y The Y-coordinate (row) of the position.
            * @param text The C-style string to print.
            * @param color_pair The color pair to use for printing (default is 0).
            */
            void printat(int x, int y, const char* text, int color_pair = 0) const {
                // Convert the C-style string to a std::wstring
                std::wstring wtext;
                wtext.resize(std::strlen(text));
                std::mbstowcs(&wtext[0], text, wtext.size());

                printat(x, y, wtext.c_str(), color_pair);
            }

            /**
            * @brief Refresh the terminal screen.
            */
            void refresh () const {refresh();}

            /**
            * @brief Get a character from the terminal input.
            * @return The character read from the terminal input.
            */
            int input () const {
                #ifdef NO_CURSES
                    return getchar();
                #endif

                return getch();
            }

            /**
            * @brief Cleanup function to be called at program exit.
            */
            static void cleanup() {
                endwin();
            }

        private:
            /**
            * @brief Private constructor to initialize the ncurses environment.
            */
            Term() {
                #ifdef NO_CURSES
                    return;
                #endif

                // Set the LANG environment variable to use UTF-8 encoding
                setenv("LANG", "en_US.UTF-8", 1);

                // Set the locale to UTF-8
                setlocale(LC_ALL, "");
                setlocale(LC_CTYPE, "");  // Set the CTYPE locale

                // Initialize ncurses
                initscr();
                cbreak();
                noecho();
                keypad(stdscr, TRUE);
                curs_set(FALSE);
                nodelay(stdscr, TRUE);

                // Initialize color support and color pairs
                start_color();
                init_colors();
                init_pair(1, COLOR_GREEN, COLOR_BLACK);

                // Get terminal dimensions
                getmaxyx(stdscr, height, width);

                // Register cleanup function to be called at program exit
                atexit(cleanup);
            }

            /**
            * @brief Private destructor to clean up the ncurses environment.
            */
            ~Term() { cleanup(); }

            /**
            * @brief Initialize the color pairs.
            */
            static void init_colors() {
                if (has_colors() && COLOR_PAIRS >= 16) {
                    for (int i = 0; i < 32; ++i) {
                        init_pair(i + 1, COLOR_BLACK, i);  // Set foreground color to i and background to black
                    }
                } else {
                    throw std::runtime_error("Not enough color support!\n");
                }
            }

        private:
            int width;
            int height;
    };
}
