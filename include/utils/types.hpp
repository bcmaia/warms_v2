#pragma once


#include <cstdint>


namespace types {
    enum class SimpleDir : uint8_t {
        Up = 1, Right = 2, Down = 3, Left = 4
    };

    types::SimpleDir rotate (const types::SimpleDir dir, const bool clockwise);

    using Color = uint8_t;
}

