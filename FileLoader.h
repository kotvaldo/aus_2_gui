#pragma once

#include "Area.h"
#include "GPS.h"
#include "KDTree.h"
#include "Nehnutelnost.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

class FileLoader {
private:

    std::vector<int>&idParcely;
    std::vector<int>&idNehnutelnost;
    std::vector<int>&idAreas;

public:
    FileLoader(std::vector<int>& idNehnutelnost, std::vector<int>& idAreas, vector<int>& idParcely) : idAreas(idAreas),idNehnutelnost(idNehnutelnost), idParcely(idParcely) {}

    ~FileLoader() {
    }



    int getBiggerUIDParcely() {
        if (idParcely.empty()) {
            idParcely.push_back(1);
        } else {
            idParcely.push_back(idParcely.size() + 1);
        }
        return idParcely.size();
    }

    int getBiggerIDNehnutelnosti() {
        if (idNehnutelnost.empty()) {
            idNehnutelnost.push_back(1);
        } else {
            idNehnutelnost.push_back(idNehnutelnost.size() + 1);
        }
        return idNehnutelnost.size();
    }
    int getBiggerIDArea() {
        if (idAreas.empty()) {
            idAreas.push_back(1);
        } else {
            idAreas.push_back(idNehnutelnost.size() + 1);
        }
        return idAreas.size();
    }

    bool loadNehnutelnosti(const std::string& filename,  GeneralKDTree<GPS, Nehnutelnost>& tree_nehnutelnost, GeneralKDTree<GPS, Area>& tree_area) {
        std::ifstream infile(filename);
        if (!infile) {
            std::cerr << "Unable to open file " << filename << std::endl;
            return false;
        }

        std::string line;
        std::getline(infile, line); // Skip header line
        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            std::string uidStr, gpsXStr, gpsYStr, widthStr, lengthStr, supisneCisloStr, description;

            if (!std::getline(iss, uidStr, ';') ||
                !std::getline(iss, gpsXStr, ';') ||
                !std::getline(iss, gpsYStr, ';') ||
                !std::getline(iss, widthStr, ';') ||
                !std::getline(iss, lengthStr, ';') ||
                !std::getline(iss, supisneCisloStr, ';') ||
                !std::getline(iss, description)) {
                std::cerr << "Error reading nehnutelnost from file" << std::endl;
                infile.close();
                return false;
            }

            int uid = getBiggerIDNehnutelnosti();
            double gpsX = std::stod(FileLoader::trim(gpsXStr));
            double gpsY = std::stod(FileLoader::trim(gpsYStr));
            char width = FileLoader::trim(widthStr)[0];
            char length = FileLoader::trim(lengthStr)[0];
            int supisneCislo = std::stoi(FileLoader::trim(supisneCisloStr));
            std::string popis = FileLoader::trim(description);

            GPS* gps = new GPS(gpsX, gpsY, width, length);
            Nehnutelnost* nehnutelnost = new Nehnutelnost(uid, gps, supisneCislo, popis);
            tree_nehnutelnost.insert(nehnutelnost, gps);
            Area* a = new Area(getBiggerIDArea(), gps, nehnutelnost, nullptr);
            tree_area.insert(a, gps);
        }

        infile.close();
        return true;
    }


    bool loadParcely(const std::string& filename,  GeneralKDTree<GPS, Parcela>& tree_parcely, GeneralKDTree<GPS, Area>& tree_area) {
        std::ifstream infile(filename);
        if (!infile) {
            std::cerr << "Unable to open file " << filename << std::endl;
            return false;
        }

        std::string line;
        std::getline(infile, line); // Skip header line
        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            std::string uidStr, gpsXStr, gpsYStr, widthStr, lengthStr, cisloParcelyStr, description;

            if (!std::getline(iss, uidStr, ';') ||
                !std::getline(iss, gpsXStr, ';') ||
                !std::getline(iss, gpsYStr, ';') ||
                !std::getline(iss, widthStr, ';') ||
                !std::getline(iss, lengthStr, ';') ||
                !std::getline(iss, cisloParcelyStr, ';') ||
                !std::getline(iss, description)) {
                std::cerr << "Error reading parcela from file" << std::endl;
                infile.close();
                return false;
            }

            int uid = getBiggerUIDParcely();
            double gpsX = std::stod(FileLoader::trim(gpsXStr));
            double gpsY = std::stod(FileLoader::trim(gpsYStr));
            char width = FileLoader::trim(widthStr)[0];
            char length = FileLoader::trim(lengthStr)[0];
            int cisloParcely = std::stoi(FileLoader::trim(cisloParcelyStr));
            std::string popis = FileLoader::trim(description);

            GPS* gps = new GPS(gpsX, gpsY, width, length);
            Parcela* parcela = new Parcela(uid, gps, cisloParcely, popis);
            tree_parcely.insert(parcela, gps);
            Area* a = new Area(getBiggerIDArea(), gps, nullptr, parcela);
            tree_area.insert(a, gps);
        }

        infile.close();
        return true;
    }


    static inline std::string trim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
        return s;
    }

};



