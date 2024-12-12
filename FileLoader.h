#pragma once

#include "Area.h"
#include "GPS.h"
#include "KDTree.h"
#include "Nehnutelnost.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>



template <typename KeyType, typename DataType>
class FileLoader
{
private:
    GeneralKDTree<KeyType, DataType>& tree;
    vector<int>& idManager;
    string filename;
public:
    FileLoader() {}

    ~FileLoader() {}



    bool loadData()
    {
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

            if (!std::getline(iss, uidStr, ';') || !std::getline(iss, gpsXStr, ';')
                || !std::getline(iss, gpsYStr, ';') || !std::getline(iss, widthStr, ';')
                || !std::getline(iss, lengthStr, ';') || !std::getline(iss, supisneCisloStr, ';')
                || !std::getline(iss, description)) {
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

            GPS *gps = new GPS(gpsX, gpsY, width, length);
            Nehnutelnost *nehnutelnost = new Nehnutelnost(uid, gps, supisneCislo, popis);
            tree_nehnutelnost.insert(nehnutelnost, gps);
            vector<Parcela *> parcelyPrekryv = tree_parcely.find(gps);
            for (Parcela *p : parcelyPrekryv) {
                if (p->getGps()->equalsByKeys(
                        *gps)) { // Predpokladáme, že `isWithin` určuje, či sa GPS nachádza v rámci parcely
                    nehnutelnost->addParcela(p);
                    p->addNehnutelnost(nehnutelnost);
                }
            }

            GPS *gps_copy = new GPS(*gps);
            Area *a = new Area(getBiggerIDArea(), gps_copy, nehnutelnost, nullptr);
            tree_area.insert(a, gps_copy);
        }

        infile.close();
        return true;
    }

    bool loadParcely(const std::string &filename,
                     GeneralKDTree<GPS, Parcela> &tree_parcely,
                     GeneralKDTree<GPS, Area> &tree_area)
    {
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

            if (!std::getline(iss, uidStr, ';') || !std::getline(iss, gpsXStr, ';')
                || !std::getline(iss, gpsYStr, ';') || !std::getline(iss, widthStr, ';')
                || !std::getline(iss, lengthStr, ';') || !std::getline(iss, cisloParcelyStr, ';')
                || !std::getline(iss, description)) {
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

            GPS *gps = new GPS(gpsX, gpsY, width, length);
            Parcela *parcela = new Parcela(uid, gps, cisloParcely, popis);
            tree_parcely.insert(parcela, gps);

            GPS *gps_copy = new GPS(*gps);
            Area *a = new Area(getBiggerIDArea(), gps_copy, nullptr, parcela);
            tree_area.insert(a, gps_copy);
        }

        infile.close();
        return true;
    }



    void setIdManager(const vector<int> &newIdManager) {
        idManager = newIdManager;
    }
};

