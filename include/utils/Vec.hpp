#pragma once


#include <stdexcept>
#include <cmath>
#include <sstream>
#include <cstdint>

//#include "types.hpp"


template <size_t D, typename T> class Vec;

// Overload the << operator for Vec
template <size_t D, typename T>
std::ostream& operator<<(std::ostream& os, const Vec<D, T>& vec) {
    std::stringstream ss;
    ss << "(" << vec[0];
    for (size_t i = 1; i < D; i++) {
        ss << ", " << vec[i];
    }
    ss << ")";
    return os << ss.str();
}



template <size_t D, typename T = float>
class Vec {
public:
    static Vec<D, T> Zero () {
        T a = static_cast<T>(0);
        return Vec<D, T>(a);
    }

    static Vec<D, T> Ones () {
        T a = static_cast<T>(1);
        return Vec<D, T>(a);
    }

    static Vec<D, T> Up () {
        T arr[] = {static_cast<T>(0), static_cast<T>(-1)};
        return Vec<D, T>(arr);
    }

    static Vec<D, T> Down () {
        T arr[] = {static_cast<T>(0), static_cast<T>(1)};
        return Vec<D, T>(arr);
    }

    static Vec<D, T> Left () {
        T arr[] = {static_cast<T>(-1), static_cast<T>(0)};
        return Vec<D, T>(arr);
    }

    static Vec<D, T> Right () {
        T arr[] = {static_cast<T>(1), static_cast<T>(0)};
        return Vec<D, T>(arr);
    }

    Vec() {}

    // Vec(const Direction dir) {
    //     if constexpr (2 < D) throw std::out_of_range("Vector dimension is less than 2");

    //     switch (dir) {
    //         case Direction::Up:     *this = Vec<D, T>::Up;
    //         case Direction::Right:  *this = Vec<D, T>::Right;
    //         case Direction::Down:   *this = Vec<D, T>::Down;
    //         case Direction::Left:   *this = Vec<D, T>::Left;
    //         default:                *this = Vec<D, T>::Zero;
    //     }
    // }

    Vec(const T val) {
        for (size_t i = 0; i < D; i++) {
            v[i] = val;
        }
    }

    // Initialize the vector with a set of values
    Vec(const T (&values)[D]) {
        for (size_t i = 0; i < D; i++) {
            v[i] = values[i];
        }
    }
    
    Vec(size_t len, const T* values) {
        for (size_t i = 0; i < D; i++) {
            v[i] = i < len ? values[i] : 0;
        }
    }

    // constructor for Vec<2, T>
    Vec(T v1, T v2) {
        if constexpr (D < 2) {
            throw std::out_of_range("Vector dimension is less than 2");
        }

        v[0] = v1;
        v[1] = v2;
    }

    ~Vec() {}

    // Accessors for individual components
    T x() const {
        if constexpr (D < 1) throw std::out_of_range("Vector dimension is less than 1");
        return v[0];
    }

    T y() const {
        if constexpr (D < 2) throw std::out_of_range("Vector dimension is less than 2");
        return v[1];
    }

    T z() const {
        if constexpr (D < 3) throw std::out_of_range("Vector dimension is less than 3");
        return v[2];
    }

    T w() const {
        if constexpr (D < 4) throw std::out_of_range("Vector dimension is less than 4");
        return v[3];
    }

    T& x() {
        if constexpr (D < 1) throw std::out_of_range("Vector dimension is less than 1");
        return v[0];
    }

    T& y() {
        if constexpr (D < 2) throw std::out_of_range("Vector dimension is less than 2");
        return v[1];
    }

    T& z() {
        if constexpr (D < 3) throw std::out_of_range("Vector dimension is less than 3");
        return v[2];
    }

    T& w() {
        if constexpr (D < 4) throw std::out_of_range("Vector dimension is less than 4");
        return v[3];
    }

    std::string str () const {
        std::stringstream ss;
        ss << "(";
        for (size_t i = 0; i < D; ++i) {
            ss << v[i];
            if (i < D - 1) {
                ss << ", ";
            }
        }
        ss << ")";
        return ss.str();
    }

    // Dot product of two vectors
    T dot(const Vec<D, T>& other) const {
        T result = 0;
        for (size_t i = 0; i < D; ++i) {
            result += v[i] * other.v[i];
        }
        return result;
    }

    // Magnitude (length) of the vector
    T length() const {
        T result = 0;
        for (size_t i = 0; i < D; ++i) {
            result.v[i] += v[i] * v[i];
        }
        return std::sqrt(result);
    }

    // Normalize the vector (make it have a length of 1)
    Vec<D, T> normalized() const {
        T len = length();

        if (0 == len) throw std::domain_error("Cannot normalize a zero-length vector");
 
        Vec<D, T> result;
        for (size_t i = 0; i < D; ++i) {
            result.v[i] = v[i] / len;
        }

        return result;
    }

    // Addition operator
    Vec<D, T> operator+(const Vec<D, T>& other) const {
        Vec<D, T> result;
        for (size_t i = 0; i < D; ++i) {
            result.v[i] = v[i] + other.v[i];
        }
        return result;
    }

    // Subtraction operator
    Vec<D, T> operator-(const Vec<D, T>& other) const {
        Vec<D, T> result;
        for (size_t i = 0; i < D; ++i) {
            result.v[i] = v[i] - other.v[i];
        }
        return result;
    }

    // Scalar multiplication
    Vec<D, T> operator*(T scalar) const {
        // Vec<D, T> result;

        // constexpr if (2 == D) {
        //     result[0] = v[0] * scalar;
        //     result[1] = v[1] * scalar;
        //     return result;
        // } else {}

        Vec<D, T> result;
        for (size_t i = 0; i < D; ++i) {
            result[i] = v[i] * scalar;
        }
        return result;
    }

    Vec<D, T> operator/(T scalar) const {
        Vec<D, T> result;
        for (size_t i = 0; i < D; ++i) {
            result.v[i] = v[i] / scalar;
        }
        return result;
    }

    T operator[] (size_t i) const {
        return v[i];
    }

    T& operator[] (size_t i) {
        return v[i];
    }

    // Modulo operator (%) for integral types
    // NOTE: Look, i know this template shit is weird, but it does the job.
    template <typename U = T, typename std::enable_if<std::is_integral<U>::value, int>::type = 0>
    Vec<D, T> operator%(const Vec<D, T>& other) const {
        Vec<D, T> result;
        for (size_t i = 0; i < D; ++i) {
            result.v[i] = v[i] % other.v[i];
        }
        return result;
    }

    // Modulo operator (%) for integral types
    template <typename U = T, typename std::enable_if<std::is_integral<U>::value, int>::type = 0>
    Vec<D, T> operator%(const T other) const {
        Vec<D, T> result;
        for (size_t i = 0; i < D; ++i) {
            result.v[i] = v[i] % other;
        }
        return result;
    }

    // Modulo operator (%) for floating-point types
    template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value, int>::type = 0>
    Vec<D, T> operator%(const Vec<D, T>& other) const {
        Vec<D, T> result;
        for (size_t i = 0; i < D; ++i) {
            result.v[i] = std::fmod(v[i], other.v[i]);
        }
        return result;
    }

    // Modulo operator (%) for floating-point types
    template <typename U = T, typename std::enable_if<std::is_floating_point<U>::value, int>::type = 0>
    Vec<D, T> operator%(const T other) const {
        Vec<D, T> result;
        for (size_t i = 0; i < D; ++i) {
            result.v[i] = std::fmod(v[i], other);
        }
        return result;
    }

    // Vec<D, T> operator+(const Direction other) const {
    //     if constexpr (2 < D) throw std::out_of_range("Vector dimension is less than 2");

    //     Vec<D, T> result = *this;
        
    //     switch (other) {
    //         case Direction::Up:     result.y() -= 1; break;
    //         case Direction::Right:  result.x() += 1; break;
    //         case Direction::Down:   result.y() -= 1; break;
    //         case Direction::Left:   result.x() -= 1; break;
    //     }

    //     return result;
    // }

    Vec<D, T> operator=(const Vec<D, T> other) {
        if constexpr (1 == D) {
            v[0] = other[0];
        } else if constexpr (2 == D) {
            v[0] = other[0];
            v[1] = other[1];
        } else if constexpr (3 == D) {
            v[0] = other[0];
            v[1] = other[1];
            v[2] = other[2];
        } else if constexpr (4 == D) {
            v[0] = other[0];
            v[1] = other[1];
            v[2] = other[2];
            v[3] = other[3];
        } else {
            for (size_t i = 0; i < D; i++) v[i] = other[i];
        }

        return *this;
    }

    bool operator==(const Vec<D, T> other) const {
        if constexpr (1 == D) {
            return v[0] == other[0];
        } else if constexpr (2 == D) {
            return v[0] == other[0]
                && v[1] == other[1];
        } else if constexpr (3 == D) {
            return v[0] == other[0]
                && v[1] == other[1]
                && v[2] == other[2];
        } else if constexpr (4 == D) {
            return v[0] == other[0]
                && v[1] == other[1]
                && v[2] == other[2]
                && v[3] == other[3];
        } else {
            for (size_t i = 0; i < D; i++) 
                if (v[i] != other[i]) 
                    return false;
            return true;

        }
    }

    // NOTE: There is space for futher optimization here
    bool operator!=(const Vec<D, T> other) const {
        return !(*this == other);
    }

    Vec<D, T> operator+=(const Vec<D, T> other) {
        *this = *this + other; 
        return *this;
    }

    Vec<D, T> operator-=(const Vec<D, T> other) {
        *this = *this - other; 
    }

    Vec<D, T> operator-() const {
        Vec<D, T> result = *this;

        if constexpr (1 == D) {
            result[0] = v[0];
        } else if constexpr (2 == D) {
            result[0] = v[0];
            result[1] = v[1];
        } else if constexpr (3 == D) {
            result[0] = v[0];
            result[1] = v[1];
            result[2] = v[2];
        } else if constexpr (4 == D) {
            result[0] = v[0];
            result[1] = v[1];
            result[2] = v[2];
            result[3] = v[3];
        } else {
            for (size_t i = 0; i < D; i++) result[i] = v[i];
        }

        return result;
    }

    template <typename TARGET_TYPE>
    explicit operator Vec<D, TARGET_TYPE> () {
        Vec<D, uint32_t> result;

        if constexpr (1 == D) {
            result[0] = static_cast<uint32_t>(v[0]);
        } else if constexpr (2 == D) {
            result[0] = static_cast<uint32_t>(v[0]);
            result[1] = static_cast<uint32_t>(v[1]);
        } else if constexpr (3 == D) {
            result[0] = static_cast<uint32_t>(v[0]);
            result[1] = static_cast<uint32_t>(v[1]);
            result[2] = static_cast<uint32_t>(v[2]);
        } else if constexpr (4 == D) {
            result[0] = static_cast<uint32_t>(v[0]);
            result[1] = static_cast<uint32_t>(v[1]);
            result[2] = static_cast<uint32_t>(v[2]);
            result[3] = static_cast<uint32_t>(v[3]);
        } else {
            for (size_t i = 0; i < D / 2; i += 2) {
                result[i] = static_cast<uint32_t>(v[i]);
                result[i + 1] = static_cast<uint32_t>(v[i + 1]);
            }
            if constexpr (D % 2) result[D - 1] = static_cast<uint32_t>(v[D - 1]);
        }

    return result;
}

    T v[D];
};

// Scalar multiplication (reverse order)
template <size_t D, typename T>
Vec<D, T> operator*(T scalar, const Vec<D, T>& vec) {
    return vec * scalar;
}

using Vec2 = Vec<2, float>;
using IVec2 = Vec<2, int32_t>;
using UVec2 = Vec<2, uint32_t>;

using Vec3 = Vec<3, float>;
using IVec3 = Vec<3, int32_t>;
using UVec3 = Vec<3, uint32_t>;