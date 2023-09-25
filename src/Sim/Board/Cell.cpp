#include "Sim/Board/Cell.hpp"


namespace cell {

    Cell Cell::Empty() {
        return Cell(CellType::Empty, 0);
    }

    Cell Cell::Food() {
        return Cell(CellType::Food, 1);
    }

    Cell Cell::Reserved() {
        return Cell(CellType::Reserved, 1);
    }

    Cell Cell::Wall() {
        return Cell(CellType::Wall, 2);
    }

    Cell::Cell() {}

    Cell::Cell(const CellType cell_type) : type(cell_type) {}

    Cell::Cell(
        const CellType cell_type, 
        const Color cell_color
    ) : type(cell_type), color(cell_color) {}

    Cell::Cell(
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

    Cell::~Cell() {}

    CellType Cell::get_type() const {
        return type;
    }

    Color Cell::get_color() const {
        return color;
    }

    bool Cell::is_empty() const {
        return CellType::Empty == type;
    }

    // Implement similar member functions for other cell types...

    SimpleDir Cell::get_dir() const {
        return dir;
    }

    int32_t Cell::get_id() const {
        return id;
    }

    // Implement other member functions...

    bool Cell::is_organism() const {
        return (
            CellType::SnakeBody == type 
            || CellType::SnakeHead == type 
            || CellType::SnakeTail == type
        );
    }

    bool Cell::is_solid() const {
        return (
            CellType::Wall == type 
            || CellType::SnakeBody == type 
            || CellType::SnakeHead == type 
            || CellType::SnakeTail == type
        );
    }

    bool Cell::is_interesting() const {
        return (is_wall() || is_organism() || is_food());
    }

    bool Cell::is_food () const {
        return CellType::Food == type;
    }

    bool Cell::is_wall () const {
        return CellType::Wall == type;
    }

    bool Cell::is_snake_tail () const {
        return CellType::SnakeTail == type;
    }

    bool Cell::is_snake_head () const {
        return CellType::SnakeHead == type;
    }

    bool Cell::is_snake_body () const {
        return CellType::SnakeHead == type;
    }

    std::wstring Cell::to_str() const {
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

    bool Cell::operator!=(const Cell& other) const {
        return (type != other.get_type());
    }

    bool Cell::operator==(const Cell& other) const {
        return (type == other.get_type());
    }

} // namespace cell
