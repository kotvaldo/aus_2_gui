#include "KDTree.h"
#include "Models.h"
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
        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            int gpsX, gpsY, supisneCislo;
            std::string description;
            if (!(iss >> gpsX >> gpsY >> supisneCislo >> std::ws && std::getline(iss, description))) {
                std::cerr << "Error reading nehnutelnost from file" << std::endl;
                return false;
            }

            int uid = getBiggerIDNehnutelnosti();  // Generovanie UID na základe sharedUIDs.size()
            GPS* gps = new GPS(gpsX, gpsY);
            Nehnutelnost* nehnutelnost = new Nehnutelnost(uid, gps, supisneCislo, description);
            nehnutelnosti.push_back(nehnutelnost);
        }

        return true;
    }

    bool loadParcely(const std::string& filename) {
        std::ifstream infile(filename);
        if (!infile) {
            std::cerr << "Unable to open file " << filename << std::endl;
            return false;
        }


        std::string line;
        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            int gpsX, gpsY, cisloParcely;
            std::string description;
            if (!(iss >> gpsX >> gpsY >> cisloParcely >> std::ws && std::getline(iss, description))) {
                std::cerr << "Error reading parcela from file" << std::endl;
                return false;
            }

            int uid = getBiggerUIDParcely();  // Generovanie UID na základe sharedUIDs.size()
            GPS* gps = new GPS(gpsX, gpsY);
            Parcela* parcela = new Parcela(uid, gps, cisloParcely, description);
            parcely.push_back(parcela);
        }

        return true;
    }


    vector<Nehnutelnost *>& getNehnutelnosti();
    vector<Parcela *>& getParcely();
};

inline vector<Parcela *>& FileLoader::getParcely()
{
    return parcely;
}

inline vector<Nehnutelnost *>& FileLoader::getNehnutelnosti()
{
    return nehnutelnosti;
}
