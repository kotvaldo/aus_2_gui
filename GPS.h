#pragma once

#include "IComparable.h"
#include "IPrototype.h"

#include <iostream>
#include <memory>

class GPS : public IComparable<GPS>, public IPrototype
{
private:
    double x;
    double y;
    char width;
    char length;

public:
    // Konštruktor
    GPS(double x = 0, double y = 0, char width = 'N', char length = 'W')
        : x(x), y(y), width(width), length(length) {}

    // Kopírovací konštruktor
    GPS(const GPS& other)
        : x(other.x), y(other.y), width(other.width), length(other.length) {}

    // Porovnávanie
    int compare(const GPS& other, int cur_level) const override {
        if (cur_level % 4 == 0) {
            return (this->width < other.width) ? -1 : (this->width > other.width ? 1 : 0);
        } else if (cur_level % 4 == 1) {
            return (this->x < other.x) ? -1 : (this->x > other.x ? 1 : 0);
        } else if (cur_level % 4 == 2) {
            return (this->length < other.length) ? -1 : (this->length > other.length ? 1 : 0);
        } else if (cur_level % 4 == 3) {
            return (this->y < other.y) ? -1 : (this->y > other.y ? 1 : 0);
        }
        return 0;
    }

    bool equals(const GPS& other) const override {
        return this->x == other.x && this->y == other.y && this->width == other.width
               && this->length == other.length;
    }

    bool equalsByKeys(const GPS& other) const override {
        return equals(other);
    }

    friend std::ostream& operator<<(std::ostream& os, const GPS& gps) {
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

    // Klonovanie (Prototype Pattern)
    std::shared_ptr<IPrototype> clone() override {
        return std::make_shared<GPS>(*this);
    }
};
