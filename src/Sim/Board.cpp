#include "Sim/Board.hpp"


namespace board {

    // Default constructor
    Board::Board() {}

    // Constructor with board dimensions and seed
    Board::Board(const IVec2 board_dimensions, const uint64_t seed) :
        rng_gen(seed),
        dimensions(board_dimensions),
        length(board_dimensions.x() * board_dimensions.y())
    {
        matrix = cell_matrix(
            dimensions.y(), 
            cell_vector(dimensions.x(), Cell::Empty())
        );
        
        procedural_obstacles(5);
        make_board_grate_again();
    }

    // Constructor with board width, height, and seed
    Board::Board(
        unsigned board_width, 
        unsigned board_height, 
        uint64_t seed
    ) :
        Board(IVec2(board_width, board_height), seed)
    {}

    // Destructor
    Board::~Board() {}

    // Function to advance the board
    void Board::foward() {
        add_food();
    }

    // Function to generate procedural obstacles
    void Board::procedural_obstacles(short amount) {
        for (short a = 0; a < amount; a++) {
            cell_matrix temp = cell_matrix(
                dimensions.y(), 
                cell_vector(dimensions.x(), Cell::Empty())
            );

            IVec2 p;
            std::uniform_real_distribution<float> dist (0.0, 1.0);
            for (p.y() = 0; p.y() < dimensions.y(); p.y() += 1) {
                for (p.x() = 0; p.x() < dimensions.x(); p.x() += 1) {
                    short i = sorrounding_walls(p);

                    float v;
                    switch (i) {
                        case 0: v = sim::PROCEDURAL_WALLS_FACTOR_0; break;
                        case 1: v = sim::PROCEDURAL_WALLS_FACTOR_1; break;
                        case 2: v = sim::PROCEDURAL_WALLS_FACTOR_2; break;
                        case 3: v = sim::PROCEDURAL_WALLS_FACTOR_3; break;
                        case 4: v = sim::PROCEDURAL_WALLS_FACTOR_4; break;
                        default: v = 0.01;
                    }

                    if (dist(rng_gen) <= v) temp[p.y()][p.x()] = Cell::Wall();
                    else temp[p.y()][p.x()] = get_raw(p);
                }
            }

            for (p.y() = 0; p.y() < dimensions.y(); p.y() += 1) {
                for (p.x() = 0; p.x() < dimensions.x(); p.x() += 1) {
                    set(p, temp[p.y()][p.x()]);
                }
            }

        }
    }

    // Function to make the board "great" again
    void Board::make_board_grate_again() {
        for (int32_t y = 0; y < dimensions.y(); y++) {
            set_raw(IVec2(0, y), Cell::Wall());
            set_raw(IVec2(dimensions.x() - 1, y), Cell::Wall());
        }

        for (int32_t x = 0; x < dimensions.x(); x++) {
            set_raw(IVec2(x, 0), Cell::Wall());
            set_raw(IVec2(x, dimensions.y() - 1), Cell::Wall());
        }
    }

    // Function to count surrounding walls
    short Board::sorrounding_walls(const IVec2 p) const {
        return (
            ( get(p + IVec2::Up()).is_wall() ? 1 : 0)
            + ( get(p + IVec2::Right()).is_wall() ? 1 : 0 )
            + ( get(p + IVec2::Down()).is_wall() ? 1 : 0 )
            + ( get(p + IVec2::Left()).is_wall() ? 1 : 0 )
        );
    }

    // Function to convert a position to a valid position
    IVec2 Board::to_valid_position(const IVec2 position) const {
        return ((position % dimensions) + dimensions) % dimensions;
    }

    // Function to get a cell at a relative position and direction
    Cell Board::get(
        const IVec2 pivot, 
        const IVec2 relative_point, 
        const types::SimpleDir dir
    ) const {
        IVec2 p = pivot + vec::rotate<int32_t>(relative_point, dir);
        return get(p);
    }

    // Function to get a cell at a position
    Cell Board::get(const IVec2 position) const {
        IVec2 p = to_valid_position(position);
        return matrix[p.y()][p.x()];
    }

    // Function to set a cell at a position
    void Board::set(const IVec2 position, const Cell c) {
        IVec2 p = to_valid_position(position);
        matrix[p.y()][p.x()] = c;
    }

    // Function to get a cell at a raw position
    Cell Board::get_raw(const IVec2 position) const {
        return matrix[position.y()][position.x()];
    }

    // Function to set a cell at a raw position
    void Board::set_raw(const IVec2 position, const Cell c) {
        matrix[position.y()][position.x()] = c;
    }

    // Function to remove cells with a specific ID
    void Board::die_by_id(int32_t id) {
        if (0 > id) return;

        IVec2 p = IVec2::Zero();
        for (p.y() = 0; p.y() < dimensions.y(); p.y() += 1) {
            for (p.x() = 0; p.x() < dimensions.x(); p.x() += 1) {
                Cell c = get_raw(p);
                if (c.is_organism() && c.get_id() == id) {
                    set_raw(p, Cell::Food());
                }
            }
        }
    }

    // Function to clear the board
    void Board::clear() {
        IVec2 p = IVec2::Zero();
        for (p.y() = 0; p.y() < dimensions.y(); p.y() += 1) {
            for (p.x() = 0; p.x() < dimensions.x(); p.x() += 1) {
                Cell c = get_raw(p);
                if (c.is_organism() || c.is_food()) {
                    set_raw(p, Cell::Empty());
                }
            }
        }
    }

    // Function to get board dimensions
    IVec2 Board::get_dimensions() const {return dimensions;}

    // Function to get the random number generator
    std::mt19937_64 Board::get_rng_gen() const {return rng_gen;}

    // Function to get the board length
    size_t Board::get_length() const {return length;}

    // Assignment operator
    Board& Board::operator=(const Board& other) {
        if (this == &other) {return *this;}

        rng_gen = other.get_rng_gen();

        IVec2 other_dim = other.get_dimensions();

        if ((0 >= matrix.size()) || (dimensions != other_dim)) {
            matrix = cell_matrix(
                other_dim.y(),
                cell_vector(other_dim.x(), Cell::Empty())
            );

            dimensions = other_dim;
            length = other.get_length();
        } 

        IVec2 p = IVec2::Zero();

        for (p.y() = 0; p.y() < other_dim.y(); p.y() += 1) {
            for (p.x() = 0; p.x() < other_dim.x(); p.x() += 1) {
                set(p, other.get(p));
            }
        }

        return *this;
    }

    // Function to get empty positions
    std::vector<IVec2> Board::get_empty_positions(
        size_t number_of_cells, 
        bool clear_reserved
    ) {
        std::uniform_int_distribution<uint32_t> width_dist (0, dimensions.x() - 1);
        std::uniform_int_distribution<uint32_t> height_dist (0, dimensions.y() - 1);

        std::vector<IVec2> result;
        //result.reserve(number_of_cells);

        IVec2 p;
        for (size_t i = 0; i < number_of_cells; i++) {
            for (uint8_t a = 0; a < sim::MAX_ATEMPTS; a++) {
                p.x() = width_dist(rng_gen);
                p.y() = height_dist(rng_gen);

                if (get(p).is_empty()) {
                    set(p, Cell::Reserved());
                    result.push_back(p);
                    break;
                }
            }
        }

        if (clear_reserved) {
            for (IVec2& position : result) {
                set(position, Cell::Empty());
            }
        }

        return result;
    }

    // Private function to add food to the board
    void Board::add_food() {
        std::uniform_int_distribution<uint32_t> width_dist (0, dimensions.x() - 1);
        std::uniform_int_distribution<uint32_t> height_dist (0, dimensions.y() - 1);

        IVec2 p;
        for (uint8_t a = 0; a < sim::MAX_ATEMPTS; a++) {
            p.x() = width_dist(rng_gen);
            p.y() = height_dist(rng_gen);

            if (get(p).is_empty()) {
                set(p, Cell::Food());
                break;
            }
        }
    }
}
