#pragma once


#include <cstdint>
#include <sstream>

#include "utils/types.hpp"
#include "Sim/sim_constants.hpp"


namespace cell {
    using Color = types::Color;
    using SimpleDir = types::SimpleDir;


    enum class CellType : uint8_t {
        Empty = 0, Wall, Food, 
        SnakeBody, SnakeHead, SnakeTail,
        Reserved ,
    };

    class Cell {
        public:
            static Cell Empty () {return Cell(CellType::Empty, 0);}
            static Cell Food () {return Cell(CellType::Food, 1);}
            static Cell Reserved () {return Cell(CellType::Reserved, 1);}
            static Cell Wall () {return Cell(CellType::Wall, 2);}

            Cell () {}

            Cell (const CellType cell_type) : type(cell_type) {}

            Cell (const CellType cell_type, const Color cell_color) : 
                type(cell_type), color(cell_color) 
            {}

            Cell (
                const CellType cell_type, 
                const Color cell_color,
                const SimpleDir cell_dir,
                const uint32_t cell_id
            ) : 
                type(cell_type), 
                color(cell_color),
                dir(cell_dir),
                id(cell_id)
            {}

            ~Cell () {}

            CellType get_type () const {return type;}
            Color get_color () const {return color;}
            bool is_empty () const {return CellType::Empty == type;}
            bool is_food () const {return CellType::Food == type;}
            bool is_wall () const {return CellType::Wall == type;}
            bool is_snake_tail () const {return CellType::SnakeTail == type;}
            bool is_snake_head () const {return CellType::SnakeHead == type;}
            bool is_snake_body () const {return CellType::SnakeHead == type;}
            SimpleDir get_dir () const {return dir;}
            int32_t get_id () const {return id;}

            bool is_organism () const {
                return (
                    CellType::SnakeBody == type 
                    || CellType::SnakeHead == type
                    || CellType::SnakeTail == type
                );
            }


            bool is_solid () const {
                return (
                    CellType::Wall == type
                    || CellType::SnakeBody == type
                    || CellType::SnakeHead == type
                    || CellType::SnakeTail == type
                );
            }

            bool is_interesting () const {
                return (
                    is_wall() || is_organism() || is_food()
                );
            }
            
            std::wstring to_str () const {
                if constexpr (sim::PRINT_CELL_AS_ID) {
                    std::wstringstream ss;
                    if (-1 == id) ss << L".";
                    else ss << (id % 10);

                    if (type == CellType::SnakeHead) return L"h";
                    if (type == CellType::Reserved) return L"r";


                    return ss.str();
                }

                switch (type) {
                    case CellType::Empty: return L" ";
                    case CellType::Food: return L"&";
                    case CellType::SnakeBody: 
                        switch (dir) {
                            case SimpleDir::Up: return L"↑";
                            case SimpleDir::Right: return L"→";
                            case SimpleDir::Down: return L"↓";
                            case SimpleDir::Left: return L"←";
                        }
                        return L"*";
                    case CellType::SnakeTail:
                        switch (dir) {
                            case SimpleDir::Up: return L"↟";
                            case SimpleDir::Right: return L"↠";
                            case SimpleDir::Down: return L"↡";
                            case SimpleDir::Left: return L"↞";
                        }
                        return L"+";
                    case CellType::SnakeHead:
                        switch (dir) {
                            case SimpleDir::Up: return L"⇑";
                            case SimpleDir::Right: return L"⇒";
                            case SimpleDir::Down: return L"⇓";
                            case SimpleDir::Left: return L"⇐";
                        }
                        return L"@";
                    case CellType::Reserved: return L"R";
                    case CellType::Wall: return L"#";
                    default: return L" ";
                }
            }

            bool operator!= (const Cell& other) const {
                return (type != other.get_type());
            }

            bool operator== (const Cell& other) const {
                return (type == other.get_type());
            }

        private:
            CellType type = CellType::Empty;
            Color color = 0;
            SimpleDir dir = SimpleDir::Up;
            uint8_t amount = 0;

            int32_t id = -1;
    };
}