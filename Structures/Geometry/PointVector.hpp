#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <cassert>

template <class T>
class PointVector {
private:
    std::vector<T> coordinates;

public:
    PointVector() {}
    PointVector(const std::vector<T>& coordinates) : coordinates(coordinates) {}
    PointVector(const PointVector<T>& other)
        : coordinates(other.coordinates) {}

    std::string toString() const {
        std::string result = "PointVector{";
        for (const auto& coord : coordinates) {
            result += std::to_string(coord) + ", ";
        }
        result.pop_back();
        result.pop_back();
        result += "}";
        return result;
    }

    std::size_t size() const {
        return coordinates.size();
    }

    T operator[](std::size_t index) const {
        return coordinates[index];
    }

    PointVector operator+(const PointVector& other) const {
        if (size() != other.size()) {
            throw std::invalid_argument("Dimensions of vectors do not match");
        }
        PointVector<T> result;
        for (std::size_t i = 0; i < size(); ++i) {
            result.coordinates.push_back(coordinates[i] + other.coordinates[i]);
        }
        return result;
    }

    PointVector<T> operator-(const PointVector<T>& other) const {
        if (size() != other.size()) {
            throw std::invalid_argument("Dimensions of vectors do not match");
        }
        PointVector<T> result;
        for (std::size_t i = 0; i < size(); ++i) {
            result.coordinates.push_back(coordinates[i] - other.coordinates[i]);
        }
        return result;
    }

    auto operator*(T factor) const {
        if constexpr (std::is_arithmetic_v<T>) {
            PointVector<T> result;
            for (const auto& coord : coordinates) {
                result.coordinates.push_back(coord * factor);
            }
            return result;
        }
        else {
            T sum = 0;
            for (std::size_t i = 0; i < size(); ++i) {
                sum += coordinates[i] * factor[i];
            }
            return sum;
        }
    }

    friend PointVector<T> operator*(T scalar, const PointVector<T>& vec) {
        return vec * scalar;
    }

    PointVector<T> operator/(T scalar) const {
        PointVector<T> result;
        for (const auto& coord : coordinates) {
            result.coordinates.push_back(coord / scalar);
        }
        return result;
    }

    bool operator==(const PointVector<T>& other) const {
        return coordinates == other.coordinates;
    }

    bool operator!=(const PointVector<T>& other) const {
        return !(*this == other);
    }

    T x() const {
        return coordinates.at(0);
    }

    T y() const {
        return coordinates.at(1);
    }

    T z() const {
        return coordinates.at(2);
    }

    T& x() {
        return coordinates.at(0);
    }

    T& y() {
        return coordinates.at(1);
    }

    T& z() {
        return coordinates.at(2);
    }

    PointVector<T> operator-() const {
        PointVector<T> result;
        for (const auto& coord : coordinates) {
            result.coordinates.push_back(-coord);
        }
        return result;
    }

    T magnitude() const {
        T sum = 0;
        for (const auto& coord : coordinates) {
            sum += coord * coord;
        }
        return std::sqrt(sum);
    }

    PointVector<T> normalize() const {
        T mag = magnitude();
        PointVector<T> result;
        for (const auto& coord : coordinates) {
            result.coordinates.push_back(coord / mag);
        }
        return result;
    }

    T dotProduct(const PointVector<T>& other) const {
        if (size() != other.size()) {
            throw std::invalid_argument("Dimensions of vectors do not match");
        }
        T sum = 0;
        for (std::size_t i = 0; i < size(); ++i) {
            sum += coordinates[i] * other.coordinates[i];
        }
        return sum;
    }

    PointVector<T> crossProduct(const PointVector<T>& other) const {
        if (size() != 3 || other.size() != 3) {
            throw std::invalid_argument("Cross product is defined only for 3-dimensional vectors");
        }
        T x = coordinates[1] * other.coordinates[2] - coordinates[2] * other.coordinates[1];
        T y = coordinates[2] * other.coordinates[0] - coordinates[0] * other.coordinates[2];
        T z = coordinates[0] * other.coordinates[1] - coordinates[1] * other.coordinates[0];
        return PointVector<T>({ x, y, z });
    }

    T distance(const PointVector<T>& other) const {
        PointVector<T> diff = *this - other;
        return diff.magnitude();
    }

    PointVector<T> getRotated(T angle) const {
        assert(size() > 1);
        T x = coordinates[0];
        T y = coordinates[1];
        T cos_ = std::cos(angle);
        T sin_ = std::sin(angle);
        return PointVector<T>({ x * cos_ - y * sin_, x * sin_ + y * cos_ });
    }

    std::vector<T> getCoordinates() const {
        return this->coordinates;
    }
};

template <class T>
struct PointVectorHasher {
    std::size_t operator()(const PointVector<T>& pv) const {
        size_t hash = 0;
        for (T coord : pv.getCoordinates()) {
            hash ^= std::hash<T>{}(coord)+0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

template <class T>
PointVector<T> getMaxPoint(const PointVector<T>& point1, const PointVector<T>& point2) {
    std::vector<T> coordinates;
    for (std::size_t i = 0; i < point1.size(); ++i) {
        coordinates.push_back(std::max(point1[i], point2[i]));
    }
    return PointVector(coordinates.begin(), coordinates.end());
}

template <class T>
PointVector<T> getMinPoint(const PointVector<T>& point1, const PointVector<T>& point2) {
    std::vector<T> coordinates;
    for (std::size_t i = 0; i < point1.size(); ++i) {
        coordinates.push_back(std::min(point1[i], point2[i]));
    }
    return PointVector(coordinates.begin(), coordinates.end());
}
