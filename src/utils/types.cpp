#include "utils/types.hpp"



types::SimpleDir types::rotate (const types::SimpleDir dir, const bool clockwise = true) {
    if (clockwise) {
        switch (dir) {
            case types::SimpleDir::Up:     return types::SimpleDir::Right;
            case types::SimpleDir::Right:  return types::SimpleDir::Down;
            case types::SimpleDir::Down:   return types::SimpleDir::Left;
            case types::SimpleDir::Left:   return types::SimpleDir::Up;
        }
    } else {
        switch (dir) {
            case types::SimpleDir::Up:     return types::SimpleDir::Left;
            case types::SimpleDir::Right:  return types::SimpleDir::Up;
            case types::SimpleDir::Down:   return types::SimpleDir::Right;
            case types::SimpleDir::Left:   return types::SimpleDir::Down;
        }
    }
    return types::SimpleDir::Up;
}