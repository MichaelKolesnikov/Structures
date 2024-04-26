#pragma once
#include <string>

struct MathPointVector2DOnInt {
private:
	int x, y;
public:
	MathPointVector2DOnInt() : x(0), y(0) {}
	MathPointVector2DOnInt(const int& x, const int& y) : x(x), y(y) {}

    std::string toString() const {
        return "{" + std::to_string(x) + ", " + std::to_string(x) + "}";
    }

    MathPointVector2DOnInt operator+(const MathPointVector2DOnInt& other) const {
        return MathPointVector2DOnInt(x + other.x, y + other.y);
    }
    MathPointVector2DOnInt operator-(const MathPointVector2DOnInt& other) const {
        return MathPointVector2DOnInt(x - other.x, y - other.y);
    }
    MathPointVector2DOnInt operator*(int factor) const {
        return { this->x * factor, this->y * factor };
    }
    MathPointVector2DOnInt operator/(int scalar) const {
        return { this->x / scalar, this->y / scalar };
    }
    MathPointVector2DOnInt operator-() const {
        return { -this->x, -this->y };
    }

    bool operator==(const MathPointVector2DOnInt& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const MathPointVector2DOnInt& other) const {
        return !(*this == other);
    }

    int get_x() const {
        return x;
    }
    int get_y() const {
        return y;
    }
    int& get_x() {
        return x;
    }
    int& get_y() {
        return y;
    }

    struct Hash {
        std::size_t operator()(const MathPointVector2DOnInt& p) const {
            std::size_t hash = 2166136261u;

            hash ^= std::hash<int>{}(p.x)+0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= std::hash<int>{}(p.y)+0x9e3779b9 + (hash << 6) + (hash >> 2);

            return hash;
        }
    };
};

namespace std {
    template <>
    struct hash<MathPointVector2DOnInt> {
        std::size_t operator()(const MathPointVector2DOnInt& p) const {
            return MathPointVector2DOnInt::Hash{}(p);
        }
    };
};
