#include "Models.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

class FileLoader {
private:
    std::vector<Nehnutelnost*> nehnutelnosti;
    std::vector<Parcela*> parcely;
     std::vector<int>& sharedUIDs;

public:
    FileLoader(std::vector<int>& uids) : sharedUIDs(uids) {}


    ~FileLoader() {
        clearData();
    }

    bool loadFromFile(const std::string& nehnutelnostiFile, const std::string& parcelyFile) {
        clearData();
        return loadNehnutelnosti(nehnutelnostiFile) && loadParcely(parcelyFile);
    }

    const std::vector<Nehnutelnost*>& getNehnutelnosti() const {
        return nehnutelnosti;
    }

    const std::vector<Parcela*>& getParcely() const {
        return parcely;
    }

private:
    void clearData() {
        for (auto* nehnutelnost : nehnutelnosti) {
            delete nehnutelnost;
        }
        nehnutelnosti.clear();

        for (auto* parcela : parcely) {
            delete parcela;
        }
        parcely.clear();

        sharedUIDs.clear();
    }

    int getBiggerUID() const {
        return sharedUIDs.size();  // Vracia veľkosť sharedUIDs ako nové UID
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

            int uid = getBiggerUID();  // Generovanie UID na základe sharedUIDs.size()
            GPS* gps = new GPS(gpsX, gpsY);
            Nehnutelnost* nehnutelnost = new Nehnutelnost(uid, gps, supisneCislo, description);
            nehnutelnosti.push_back(nehnutelnost);
            sharedUIDs.push_back(uid);
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

            int uid = getBiggerUID();  // Generovanie UID na základe sharedUIDs.size()
            GPS* gps = new GPS(gpsX, gpsY);
            Parcela* parcela = new Parcela(uid, gps, cisloParcely, description);
            parcely.push_back(parcela);
            sharedUIDs.push_back(uid);
        }

        return true;
    }
};
