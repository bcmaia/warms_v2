#pragma once


#include "utils/types.hpp"


namespace cell {
    using Color = types::Color;
    using SimpleDir = types::SimpleDir;


    enum class CellType : uint8_t {
        Empty = 0, Wall = 1, Food = 2, Organism = 3
    };


    class Cell {
        public:
            static Cell Empty () {return Cell(CellType::Empty, 0);}
            static Cell Food () {return Cell(CellType::Food, 1);}

            Cell () {}
            Cell (const CellType cell_type) : type(cell_type) {}
            Cell (const CellType cell_type, const Color cell_color) : 
                type(cell_type), color(cell_color) 
            {}
            ~Cell () {}

            CellType get_type () const {return type;}
            Color get_color () const {return color;}
            bool is_empty () const {return CellType::Empty == type;}
            
            std::wstring to_str () {
                switch (type) {
                    case CellType::Empty: return L" ";
                    case CellType::Food: return L"&";
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
    };
}