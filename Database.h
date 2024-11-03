#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include "Models.h"
#include "Parcela.h"

class Database {
private:
    std::vector<std::unique_ptr<Nehnutelnost>> nehnutelnosti;
    std::vector<std::unique_ptr<Parcela>> parcely;

public:
    // Načítanie zo súboru
    bool loadFromFile(const std::string& nehnutelnostiFile, const std::string& parcelyFile) {
        return loadNehnutelnosti(nehnutelnostiFile) && loadParcely(parcelyFile);
    }

    // Uloženie do súboru
    bool saveToFile(const std::string& nehnutelnostiFile, const std::string& parcelyFile) {
        return saveNehnutelnosti(nehnutelnostiFile) && saveParcely(parcelyFile);
    }

    // Pridanie nehnuteľnosti
    void addNehnutelnost(std::unique_ptr<Nehnutelnost> nehnutelnost) {
        nehnutelnosti.push_back(std::move(nehnutelnost));
    }

    // Pridanie parcely
    void addParcela(std::unique_ptr<Parcela> parcela) {
        parcely.push_back(std::move(parcela));
    }

    // Získanie všetkých nehnuteľností
    const std::vector<std::unique_ptr<Nehnutelnost>>& getNehnutelnosti() const {
        return nehnutelnosti;
    }

    // Získanie všetkých parciel
    const std::vector<std::unique_ptr<Parcela>>& getParcely() const {
        return parcely;
    }

private:
    // Načítanie nehnuteľností zo súboru
    bool loadNehnutelnosti(const std::string& filename) {
        std::ifstream infile(filename);
        if (!infile) {
            std::cerr << "Unable to open file " << filename << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            int uid, gpsX, gpsY;
            std::string description;
            if (!(iss >> uid >> gpsX >> gpsY >> std::ws && std::getline(iss, description))) {
                std::cerr << "Error reading nehnutelnost from file" << std::endl;
                return false;
            }
            auto gps = std::make_unique<GPS>(gpsX, gpsY);
            auto nehnutelnost = std::make_unique<Nehnutelnost>(uid, gps.get(), description);
            nehnutelnosti.push_back(std::move(nehnutelnost));
        }

        return true;
    }

    // Načítanie parciel zo súboru
    bool loadParcely(const std::string& filename) {
        std::ifstream infile(filename);
        if (!infile) {
            std::cerr << "Unable to open file " << filename << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            int uid, gpsX, gpsY;
            std::string description;
            if (!(iss >> uid >> gpsX >> gpsY >> std::ws && std::getline(iss, description))) {
                std::cerr << "Error reading parcela from file" << std::endl;
                return false;
            }
            auto gps = std::make_unique<GPS>(gpsX, gpsY);
            auto parcela = std::make_unique<Parcela>(uid, gps.get(), description);
            parcely.push_back(std::move(parcela));
        }

        return true;
    }

    // Uloženie nehnuteľností do súboru
    bool saveNehnutelnosti(const std::string& filename) {
        std::ofstream outfile(filename);
        if (!outfile) {
            std::cerr << "Unable to open file " << filename << std::endl;
            return false;
        }

        for (const auto& nehnutelnost : nehnutelnosti) {
            outfile << nehnutelnost->uid << " "
                    << nehnutelnost->gps->x << " "
                    << nehnutelnost->gps->y << " "
                    << nehnutelnost->popis << std::endl;
        }

        return true;
    }

    // Uloženie parciel do súboru
    bool saveParcely(const std::string& filename) {
        std::ofstream outfile(filename);
        if (!outfile) {
            std::cerr << "Unable to open file " << filename << std::endl;
            return false;
        }

        for (const auto& parcela : parcely) {
            outfile << parcela->uid << " "
                    << parcela->gps->x << " "
                    << parcela->gps->y << " "
                    << parcela->popis << std::endl;
        }

        return true;
    }
};
