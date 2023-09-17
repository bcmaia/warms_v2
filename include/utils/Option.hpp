#pragma once


template <typename T> 
class Option {
public:
    Option() {}
    Option(T val) : __has_value(true), __value(val) {}

    void set_value (T new_value) {
        __value = new_value;
        __has_value = true;
    }

    void clear () {
        __has_value = false;
    }

    bool has_value () const {
        return __has_value;
    }

    T unwrap() const {
        if (__has_value) {
            return __value;
        } else {
            throw std::runtime_error("Tried to unwrap None");
        }
    }

    T unwrap_or(T alternative) const {
        return __has_value ? __value : alternative;
    }



private:
    bool __has_value = false;
    T __value;

};
