#pragma once

#include "IComparable.h"
#include "Prototype.h"

#include <iostream>

class GPS : public IComparable<GPS>, public IPrototype
{
private:
    double x;
    double y;
    char width;
    char length;

public:
    GPS(double x = 0, double y = 0, char width = 'N', char length = 'W')
        : x(x)
        , y(y)
        , width(width)
        , length(length)
    {}

    GPS(const GPS &other)
        : x(other.x)
        , y(other.y)
        , width(other.width)
        , length(other.length)
    {}

    int compare(const GPS &other, int cur_level) const override
    {
        if (cur_level % 4 == 0) {
            if (this->width < other.width)
                return -1;
            if (this->width > other.width)
                return 1;
        } else if (cur_level % 4 == 1) {
            if (this->x < other.x)
                return -1;
            if (this->x > other.x)
                return 1;
        } else if (cur_level % 4 == 2) {
            if (this->length < other.length)
                return -1;
            if (this->length > other.length)
                return 1;
        } else if (cur_level % 4 == 3) {
            if (this->y < other.y)
                return -1;
            if (this->y > other.y)
                return 1;
        }
        return 0;
    }

    bool equals(const GPS &other) const override
    {
        return this->x == other.x && this->y == other.y && this->width == other.width
               && this->length == other.length;
    }

    bool equalsByKeys(const GPS &other) const override
    {
        return this->x == other.x && this->y == other.y && this->width == other.width
               && this->length == other.length;
    }

    friend std::ostream &operator<<(std::ostream &os, const GPS &gps)
    {
        os << "(x: " << gps.x << ", y: " << gps.y << ", width: " << gps.width
           << ", length: " << gps.length << ")";
        return os;
    }

    // Getters
    double getX() const { return x; }
    double getY() const { return y; }
    char getWidth() const { return width; }
    char getLength() const { return length; }

    // Setters
    void setX(double newX) { x = newX; }
    void setY(double newY) { y = newY; }
    void setWidth(char newWidth) { width = newWidth; }
    void setLength(char newLength) { length = newLength; }

    IPrototype *clone() override {
        return new GPS(*this);
    }
};
