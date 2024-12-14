#pragma once


#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <sstream>
#include <iomanip>

class Util {
private:
    static std::random_device rd;
    static std::mt19937 rng;
    static std::vector<int> idGenerators;
public:
    template <typename T>
    static T randomNumber(T min, T max) {
        if constexpr (std::is_integral<T>::value) {
            std::uniform_int_distribution<T> dist(min, max);
            return dist(rng);
        } else if constexpr (std::is_floating_point<T>::value) {
            std::uniform_real_distribution<T> dist(min, max);
            return dist(rng);
        }
    }

    static char randomChar(const std::vector<char>& allowedChars = {'A', 'B', 'C', 'D'}) {
        if (allowedChars.empty()) return 'A';

        std::random_device rd;
        std::mt19937 engine(rd());
        std::uniform_int_distribution<size_t> dist(0, allowedChars.size() - 1);

        return allowedChars[dist(engine)];
    }

    static std::string trim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                    return !std::isspace(ch);
                }));
        s.erase(std::find_if(s.rbegin(),
                             s.rend(),
                             [](unsigned char ch) { return !std::isspace(ch); })
                    .base(),
                s.end());
        return s;
    }

    static std::string randomString(int length) {
        const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        std::uniform_int_distribution<> dist(0, chars.size() - 1);
        std::string result;
        for (int i = 0; i < length; ++i) {
            result += chars[dist(rng)];
        }
        return result;
    }

    static int CompareDoubles(double value1, double value2) {
        if (value1 < value2) return -1;
        if (value1 > value2) return 1;
        return 0;
    }

    static int CompareStrings(const std::string& value1, const std::string& value2) {
        if (value1 < value2) return -1;
        if (value1 > value2) return 1;
        return 0;
    }

    static std::string FormatDoubleForExport(double number) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << number;
        return ss.str();
    }

    static std::string FormatDoubleForImport(const std::string& number) {
        std::string result = number;
        std::replace(result.begin(), result.end(), '.', ',');
        return result;
    }




};


std::random_device Util::rd;
std::mt19937 Util::rng(Util::rd());
