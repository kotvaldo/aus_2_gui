#pragma once

#include "Models.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

class FileLoader {
private:

    vector<Nehnutelnost*>& nehnutelnosti;
    vector<Parcela*>& parcely;
    std::vector<int>&idParcely;
    std::vector<int>&idNehnutelnost;

public:
    FileLoader(std::vector<int>& idNehnutelnost,  std::vector<int>& idParcely, vector<Nehnutelnost*>& nehnutelnosti, vector<Parcela*>& parcely) : idNehnutelnost(idNehnutelnost), idParcely(idParcely),nehnutelnosti(nehnutelnosti), parcely(parcely) {}


    ~FileLoader() {
    }







    int getBiggerUIDParcely() {
        if (idParcely.empty()) {
            idParcely.push_back(1);
        } else {
            idParcely.push_back(idParcely.size() + 1);
        }
        return idParcely.size() + 1;
    }

    int getBiggerIDNehnutelnosti() {
        if (idNehnutelnost.empty()) {
            idNehnutelnost.push_back(1);
        } else {
            idNehnutelnost.push_back(idNehnutelnost.size() + 1);
        }
        return idNehnutelnost.size() + 1;
    }


    bool loadNehnutelnosti(const std::string& filename) {
        std::ifstream infile(filename);
        if (!infile) {
            std::cerr << "Unable to open file " << filename << std::endl;
            return false;
        }

        std::string line;
        std::getline(infile, line);
        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            std::string uidStr, gpsXStr, gpsYStr, supisneCisloStr, description;

            if (!std::getline(iss, uidStr, ';') ||
                !std::getline(iss, gpsXStr, ';') ||
                !std::getline(iss, gpsYStr, ';') ||
                !std::getline(iss, supisneCisloStr, ';') ||
                !std::getline(iss, description)) {
                std::cerr << "Error reading nehnutelnost from file" << std::endl;
                infile.close();
                return false;
            }

            int uid = getBiggerIDNehnutelnosti();
            int gpsX = std::stoi(FileLoader::trim(gpsXStr));
            int gpsY = std::stoi(FileLoader::trim(gpsYStr));
            int supisneCislo = std::stoi(FileLoader::trim(supisneCisloStr));
            string popis = FileLoader::trim(description);

            GPS* gps = new GPS(gpsX, gpsY);
            Nehnutelnost* nehnutelnost = new Nehnutelnost(uid, gps, supisneCislo, popis);
            nehnutelnosti.push_back(nehnutelnost);
        }

        infile.close();
        return true;
    }

    bool loadParcely(const std::string& filename) {
        std::ifstream infile(filename);
        if (!infile) {
            std::cerr << "Unable to open file " << filename << std::endl;
            return false;
        }

        std::string line;
        std::getline(infile, line);
        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            std::string uidStr, gpsXStr, gpsYStr, cisloParcelyStr, description;

            if (!std::getline(iss, uidStr, ';') ||
                !std::getline(iss, gpsXStr, ';') ||
                !std::getline(iss, gpsYStr, ';') ||
                !std::getline(iss, cisloParcelyStr, ';') ||
                !std::getline(iss, description)) {
                std::cerr << "Error reading parcela from file" << std::endl;
                infile.close();
                return false;
            }

            int uid = getBiggerUIDParcely();
            int gpsX = std::stoi(FileLoader::trim(gpsXStr));
            int gpsY = std::stoi(FileLoader::trim(gpsYStr));
            int cisloParcely = std::stoi(FileLoader::trim(cisloParcelyStr));

            GPS* gps = new GPS(gpsX, gpsY);
            Parcela* parcela = new Parcela(uid, gps, cisloParcely, FileLoader::trim(description));
            parcely.push_back(parcela);
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



