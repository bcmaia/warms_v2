#pragma once


#include <cstdint>
#include <sstream>

#include "utils/types.hpp"
#include "Sim/sim_constants.hpp"


namespace cell {
    using Color = types::Color;
    using SimpleDir = types::SimpleDir;

    /**
     * @brief Enum representing the type of a cell.
     */
    enum class CellType : uint8_t {
        Empty = 0, ///< Empty cell
        Wall,      ///< Wall cell
        Food,      ///< Food cell
        SnakeBody, ///< Snake body cell
        SnakeHead, ///< Snake head cell
        SnakeTail, ///< Snake tail cell
        Reserved,  ///< Reserved cell
    };

    /**
     * @brief Class representing a cell in a grid.
     */
    class Cell {
    public:
        static Cell Empty();
        static Cell Food();
        static Cell Reserved();
        static Cell Wall();

        Cell();
        Cell(const CellType cell_type);
        Cell(const CellType cell_type, const Color cell_color);
        Cell(const CellType cell_type, const Color cell_color, const SimpleDir cell_dir, const uint32_t cell_id);
        ~Cell();

        CellType get_type() const;
        Color get_color() const;
        SimpleDir get_dir() const;
        int32_t get_id() const;

        bool is_empty() const;
        bool is_organism() const;
        bool is_solid() const;
        bool is_interesting() const;
        bool is_food () const;
        bool is_wall () const;
        bool is_snake_tail () const;
        bool is_snake_head () const;
        bool is_snake_body () const;

        std::wstring to_str() const;
        
        bool operator!=(const Cell& other) const;
        bool operator==(const Cell& other) const;

    private:
        CellType type = CellType::Empty;
        Color color = 0;
        SimpleDir dir = SimpleDir::Up;
        uint8_t amount = 0;
        int32_t id = -1;
    };
}
