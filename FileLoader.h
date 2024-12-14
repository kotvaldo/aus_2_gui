#pragma once

#include "GPS.h"
#include "KDTree.h"
#include "Models.h"
#include "Util.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>



class BaseFileLoader {
protected:
    BaseFileLoader() = default;
    BaseFileLoader(const BaseFileLoader&) = delete;
    BaseFileLoader& operator=(const BaseFileLoader&) = delete;

public:
    virtual ~BaseFileLoader() = default;

    virtual bool loadFromFile(const std::string& filename) = 0;
    virtual bool saveToFile(const std::string& filename) const = 0;
};


class NehnutelnostFileLoader : public BaseFileLoader {
private:
    GeneralKDTree<GPS, Nehnutelnost>* tree;


    NehnutelnostFileLoader() {}

public:
    // Statická metóda pre získanie Singleton inštancie
    static NehnutelnostFileLoader& getInstance() {
        static NehnutelnostFileLoader instance;
        return instance;
    }

    bool loadFromFile(const std::string& filename) override {
        std::ifstream infile(filename);
        if (!infile) {
            std::cerr << "Unable to open file: " << filename << std::endl;
            return false;
        }

        std::string line;
        std::getline(infile, line); // Preskoč hlavičku

        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            std::vector<std::string> fields;
            std::string field;

            while (std::getline(iss, field, ';')) {
                fields.push_back(Util::trim(field));
            }

            if (fields.size() < 7) {
                std::cerr << "Error processing line: " << line << std::endl;
                continue;
            }

            int uid = std::stoi(fields[0]);
            double gpsX = std::stod(fields[1]);
            double gpsY = std::stod(fields[2]);
            char width = fields[3][0];
            char length = fields[4][0];
            int supisneCislo = std::stoi(fields[5]);
            std::string description = fields[6];

            GPS* gps = new GPS(gpsX, gpsY, width, length);
            Nehnutelnost* nehnutelnost = new Nehnutelnost(uid, gps, supisneCislo, description);

            tree->insert(nehnutelnost, gps);
        }

        infile.close();
        return true;
    }

    bool saveToFile(const std::string& filename) const override {
        std::ofstream outfile(filename);
        if (!outfile) {
            std::cerr << "Unable to open file: " << filename << std::endl;
            return false;
        }

        outfile << "UID;GPS_X;GPS_Y;WIDTH;LENGTH;SUPISNE_CISLO;DESCRIPTION\n";

        for (auto it = tree->beginInOrder(); it != tree->endInOrder(); ++it) {
            Nehnutelnost* item = *it;
            if (item) {
                IPrototype* key = item->getKey();
                GPS* gps = dynamic_cast<GPS*>(key);
                if (gps) {
                    outfile << item->getUid() << ";"
                            << gps->getX() << ";"
                            << gps->getY() << ";"
                            << gps->getWidth() << ";"
                            << gps->getLength() << ";"
                            << item->getSupisneCislo() << ";"
                            << item->getPopis() << "\n";
                } else {
                    std::cerr << "Error: Key is not of type GPS." << std::endl;
                }
            }
        }

        outfile.close();
        return true;
    }



    void setTree(GeneralKDTree<GPS, Nehnutelnost>* newTree) {
        tree = newTree;
    }
};


class ParcelaFileLoader : public BaseFileLoader {
private:
    GeneralKDTree<GPS, Parcela>* tree;

    ParcelaFileLoader() {}

public:
    static ParcelaFileLoader& getInstance() {
        static ParcelaFileLoader instance;
        return instance;
    }

    bool loadFromFile(const std::string& filename) override {
        std::ifstream infile(filename);
        if (!infile) {
            std::cerr << "Unable to open file: " << filename << std::endl;
            return false;
        }

        std::string line;
        std::getline(infile, line);

        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            std::vector<std::string> fields;
            std::string field;

            while (std::getline(iss, field, ';')) {
                fields.push_back(Util::trim(field));
            }

            if (fields.size() < 7) {
                std::cerr << "Error processing line: " << line << std::endl;
                continue;
            }

            int uid = std::stoi(fields[0]);
            double gpsX = std::stod(fields[1]);
            double gpsY = std::stod(fields[2]);
            char width = fields[3][0];
            char length = fields[4][0];
            int cisloParcely = std::stoi(fields[5]);
            std::string description = fields[6];

            GPS* gps = new GPS(gpsX, gpsY, width, length);
            Parcela* parcela = new Parcela(uid, gps, cisloParcely, description);

            tree->insert(parcela, gps);
        }

        infile.close();
        return true;
    }

    bool saveToFile(const std::string& filename) const override {
        std::ofstream outfile(filename);
        if (!outfile) {
            std::cerr << "Unable to open file: " << filename << std::endl;
            return false;
        }

        outfile << "UID;GPS_X;GPS_Y;WIDTH;LENGTH;CISLO_PARCELY;DESCRIPTION\n";

        for (auto it = tree->beginInOrder(); it != tree->endInOrder(); ++it) {
            Parcela* item = *it; // Získame položku zo stromu
            if (item) {
                // Získame kľúč ako IPrototype* a spravíme dynamic_cast na GPS*
                IPrototype* key = item->getKey();
                GPS* gps = dynamic_cast<GPS*>(key);
                if (gps) {
                    outfile << item->getUid() << ";"
                            << gps->getX() << ";"
                            << gps->getY() << ";"
                            << gps->getWidth() << ";"
                            << gps->getLength() << ";"
                            << item->getCisloParcely() << ";"
                            << item->getPopis() << "\n";
                } else {
                    std::cerr << "Error: Key is not of type GPS." << std::endl;
                }
            }
        }

        outfile.close();
        return true;
    }
    void setTree(GeneralKDTree<GPS, Parcela>* newTree) {
        tree = newTree;
    }
};


