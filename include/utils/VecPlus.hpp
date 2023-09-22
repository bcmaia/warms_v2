#pragma once


#include "utils/Vec.hpp"
#include "utils/types.hpp"


namespace vec {
    template <size_t D, typename T>
    Vec<D, T> vec_from_dir (const types::SimpleDir dir) {
        if constexpr (2 < D) throw std::out_of_range("Vector dimension is less than 2");

        switch (dir) {
            case types::SimpleDir::Up:      return Vec<D, T>::Up();
            case types::SimpleDir::Right:   return Vec<D, T>::Right();
            case types::SimpleDir::Down:    return Vec<D, T>::Down();
            case types::SimpleDir::Left:    return Vec<D, T>::Left();
            default:                        return Vec<D, T>::Zero();
        }
    }

    template <size_t D, typename T>
    Vec<D, T> vec_extrapolate (
        const Vec<D, T> point, 
        const types::SimpleDir dir, 
        const T amount = static_cast<T>(1)
    ) {
        if constexpr (2 < D) throw std::out_of_range("Vector dimension is less than 2");

        return point + (amount * vec::vec_from_dir<D, T>(dir));
    }

    template <typename T>
    Vec<2, T> rotate (const Vec<2, T> point, const types::SimpleDir dir) {
        Vec<2, T> result;
        switch (dir) {
            case types::SimpleDir::Up:      
                return point;
            
            case types::SimpleDir::Right:   
                result.x() = -point.y(); // -
                result.y() = point.x();
                return result;

            case types::SimpleDir::Down:
                result.x() = -point.x();
                result.y() = -point.y();
                return result;

            case types::SimpleDir::Left:
                result.x() = point.y();
                result.y() = -point.x();
                return result;

            default:
                return point;
        }
    }
}