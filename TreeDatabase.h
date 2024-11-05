#pragma once
#include "FileLoader.h"
#include "KDTree.h"
#include "Models.h"

#include <math.h>

class TreeDatabase {
private:
    GeneralKDTree<GPS, Nehnutelnost> tree_nehnutelnost;
    GeneralKDTree<GPS, Parcela> tree_parcela;
    GeneralKDTree<GPS, Area> tree_area;
    FileLoader fileloader;
    vector<Area*> areas;
    vector<Nehnutelnost*> nehnutelnosti;
    vector<Parcela*> parcely;
    std::vector<int>idArea;
    std::vector<int>idParcely;
    std::vector<int>idNehnutelnost;
    std::string nehnutelnostiFile;
    std::string parcelyFile;
public:
    TreeDatabase(std::string nehnutelnostiFile, std::string parcelyFile);

    int getBiggerUIDArea() {
        if(idArea.empty()) {
            idArea.push_back(1);
        } else {
            idArea.push_back(idArea.size() + 1);
        }
        return idArea.size() + 1;
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

    void addNehnutelnost(double x1, double y1, char width1, char length1,
                         double x2, double y2, char width2, char length2,
                         int supisneCislo, const std::string& description) {
        GPS* gps1 = new GPS(x1, y1, width1, length1);
        GPS* gps2 = new GPS(x2, y2, width2, length2);

        Nehnutelnost* nehnutelnost1 = new Nehnutelnost(getBiggerIDNehnutelnosti(), gps1, supisneCislo, description);
        Nehnutelnost* nehnutelnost2 = new Nehnutelnost(getBiggerIDNehnutelnosti(), gps2, supisneCislo, description);

        nehnutelnosti.push_back(nehnutelnost1);
        nehnutelnosti.push_back(nehnutelnost2);

        tree_nehnutelnost.insert(nehnutelnost1, gps1);
        tree_nehnutelnost.insert(nehnutelnost2, gps2);

        Area* area1 = new Area(getBiggerUIDArea(), gps1, nehnutelnost1, nullptr);
        Area* area2 = new Area(getBiggerUIDArea(), gps2, nehnutelnost2, nullptr);

        areas.push_back(area1);
        areas.push_back(area2);

        tree_area.insert(area1, gps1);
        tree_area.insert(area2, gps2);
    }

    void addNehnutelnost(double x1, double y1, char width1, char length1,
                         int supisneCislo, const std::string& description) {
        GPS* gps1 = new GPS(x1, y1, width1, length1);

        Nehnutelnost* nehnutelnost1 = new Nehnutelnost(getBiggerIDNehnutelnosti(), gps1, supisneCislo, description);

        nehnutelnosti.push_back(nehnutelnost1);

        tree_nehnutelnost.insert(nehnutelnost1, gps1);

        Area* area1 = new Area(getBiggerUIDArea(), gps1, nehnutelnost1, nullptr);

        areas.push_back(area1);

        tree_area.insert(area1, gps1);
    }

    void addParcela(double x1, double y1, char width1, char length1,
                    double x2, double y2, char width2, char length2,
                    int cisloParcely, const std::string& description) {
        GPS* gps1 = new GPS(x1, y1, width1, length1);
        GPS* gps2 = new GPS(x2, y2, width2, length2);

        Parcela* parcela1 = new Parcela(getBiggerUIDParcely(), gps1, cisloParcely, description);
        Parcela* parcela2 = new Parcela(getBiggerUIDParcely(), gps2, cisloParcely, description);

        parcely.push_back(parcela1);
        parcely.push_back(parcela2);

        tree_parcela.insert(parcela1, gps1);
        tree_parcela.insert(parcela2, gps2);

        Area* area1 = new Area(getBiggerUIDArea(), gps1, nullptr, parcela1);
        Area* area2 = new Area(getBiggerUIDArea(), gps2, nullptr, parcela2);

        areas.push_back(area1);
        areas.push_back(area2);

        tree_area.insert(area1, gps1);
        tree_area.insert(area2, gps2);
    }

    void addParcela(double x1, double y1, char width1, char length1,
                    int cisloParcely, const std::string& description) {
        GPS* gps1 = new GPS(x1, y1, width1, length1);

        Parcela* parcela1 = new Parcela(getBiggerUIDParcely(), gps1, cisloParcely, description);

        parcely.push_back(parcela1);

        tree_parcela.insert(parcela1, gps1);

        Area* area1 = new Area(getBiggerUIDArea(), gps1, nullptr, parcela1);

        areas.push_back(area1);

        tree_area.insert(area1, gps1);
    }


    void clearAllData() {
        tree_area.clear();
        tree_nehnutelnost.clear();
        tree_parcela.clear();

        for (Area*& area : areas) {
            if (area != nullptr) {

                if (area->nehnutelnost != nullptr) {
                    if (area->nehnutelnost->gps != nullptr) {
                        delete area->nehnutelnost->gps;
                        area->nehnutelnost->gps = nullptr;
                    }
                    delete area->nehnutelnost;
                    area->nehnutelnost = nullptr;
                }


                if (area->parcela != nullptr) {
                    if (area->parcela->gps != nullptr) {
                        delete area->parcela->gps;
                        area->parcela->gps = nullptr;
                    }
                    delete area->parcela;
                    area->parcela = nullptr;
                }

                delete area;
                area = nullptr;
            }
        }
        areas.clear();
        nehnutelnosti.clear();
        parcely.clear();
        idArea.clear();
        idNehnutelnost.clear();
        idParcely.clear();
    }





    void generateRandomUnits(int countPar, int countNeh, double prekryv) {
        std::vector<GPS*> parcelGPS;

        if(prekryv > 1) {
            prekryv = 1;
        }
        if(prekryv < 0) {
            prekryv = 0;
        }

        srand(static_cast<unsigned>(time(0)));


        for (int i = 0; i < countPar; i++) {
            double x = round((static_cast<double>(rand()) / RAND_MAX * 180 - 90) * 100) / 100;
            double y = round((static_cast<double>(rand()) / RAND_MAX * 360 - 180) * 100) / 100;
            GPS* gps = new GPS(
                x,
                y,
                (x < 0) ? 'N' : 'S',
                (y < 0) ? 'E' : 'W'
                );

            std::cout << "Generated parcel GPS: " << *gps << std::endl;
            parcelGPS.push_back(gps);

            Parcela* p = new Parcela(getBiggerUIDParcely(), gps, 12222222, "Testing parcel");
            Area* a = new Area(getBiggerUIDArea(), gps, nullptr, p);
            tree_parcela.insert(p, gps);
            tree_area.insert(a, gps);
            areas.push_back(a);
            parcely.push_back(p);
        }

        int count_prekryv = static_cast<int>(round(prekryv * countNeh));
        int count_not_prekryv = countNeh - count_prekryv;

        for (int i = 0; i < count_prekryv; i++) {
            GPS* gps = parcelGPS[rand() % parcelGPS.size()];
            std::cout << "Reused parcel GPS for property: " << *gps << std::endl;

            Nehnutelnost* n = new Nehnutelnost(getBiggerIDNehnutelnosti(), gps, rand() % 1000, "Testing nehnutelnost");
            Area* a = new Area(getBiggerUIDArea(), gps, n, nullptr);
            tree_nehnutelnost.insert(n, gps);
            tree_area.insert(a, gps);
            areas.push_back(a);
            nehnutelnosti.push_back(n);
        }

        for (int i = 0; i < count_not_prekryv; i++) {
            double x = round((static_cast<double>(rand()) / RAND_MAX * 180 - 90) * 100) / 100;
            double y = round((static_cast<double>(rand()) / RAND_MAX * 360 - 180) * 100) / 100;
            GPS* gps = new GPS(
                x,
                y,
                (x < 0) ? 'N' : 'S',
                (y < 0) ? 'E' : 'W'
                );
            std::cout << "Generated new property GPS: " << *gps << std::endl;

            Nehnutelnost* n = new Nehnutelnost(getBiggerIDNehnutelnosti(), gps, rand() % 1000, "Testing nehnutelnost");
            Area* a = new Area(getBiggerUIDArea(), gps, n, nullptr);
            tree_nehnutelnost.insert(n, gps);
            tree_area.insert(a, gps);
            areas.push_back(a);
            nehnutelnosti.push_back(n);
        }

    }


    bool deleteAreaRecord(int id) {
        auto it = std::find_if(areas.begin(), areas.end(),
                               [id](Area* area) { return area->uid == id; });

        if (it == areas.end()) {
            return false;
        }

        Area* area = *it;

        tree_area.removeNode(area);

        if (area->nehnutelnost) {
            tree_nehnutelnost.removeNode(area->nehnutelnost);
            auto nehIt = std::find(nehnutelnosti.begin(), nehnutelnosti.end(), area->nehnutelnost);
            if (nehIt != nehnutelnosti.end()) {
                nehnutelnosti.erase(nehIt);
            }
        } else if (area->parcela) {
            tree_parcela.removeNode(area->parcela);
            auto parIt = std::find(parcely.begin(), parcely.end(), area->parcela);
            if (parIt != parcely.end()) {
                parcely.erase(parIt);
            }
        }

        delete area;
        areas.erase(it);

        return true;
    }



    bool deleteNehnutelnostRecord(int id) {
        auto it = std::find_if(nehnutelnosti.begin(), nehnutelnosti.end(),
                               [id](Nehnutelnost* nehnutelnost) { return nehnutelnost->uid == id; });

        if (it == nehnutelnosti.end()) {
            return false;
        }

        Nehnutelnost* nehnutelnost = *it;

        tree_nehnutelnost.removeNode(nehnutelnost);

        for (auto areaIt = areas.begin(); areaIt != areas.end(); ) {
            Area* area = *areaIt;
            if (area->nehnutelnost && area->nehnutelnost->equals(*nehnutelnost)) {
                tree_area.removeNode(area);
                delete area;
                areaIt = areas.erase(areaIt);
            } else {
                ++areaIt;
            }
        }

        nehnutelnosti.erase(it);

        return true;
    }

    bool deleteParcelaRecord(int id) {
        auto it = std::find_if(parcely.begin(), parcely.end(),
                               [id](Parcela* parcela) { return parcela->uid == id; });

        if (it == parcely.end()) {
            return false;
        }

        Parcela* parcela = *it;

        tree_parcela.removeNode(parcela);

        for (auto areaIt = areas.begin(); areaIt != areas.end(); ) {
            Area* area = *areaIt;
            if (area->parcela && area->parcela->equals(*parcela)) {
                tree_area.removeNode(area);
                delete area;
                areaIt = areas.erase(areaIt);
            } else {
                ++areaIt;
            }
        }


        parcely.erase(it);

        return true;
    }
    bool editNehnutelnost(int id, double newX1, double newY1, int newSupisneCislo, const std::string& newDescription) {
        auto it = std::find_if(nehnutelnosti.begin(), nehnutelnosti.end(),
                               [id](Nehnutelnost* nehnutelnost) { return nehnutelnost->uid == id; });

        if (it == nehnutelnosti.end()) {
            return false;
        }

        Nehnutelnost* oldNehnutelnost = *it;

        bool gpsChanged = (oldNehnutelnost->gps->x != newX1 || oldNehnutelnost->gps->y != newY1);

        if (gpsChanged) {
            tree_nehnutelnost.removeNode(oldNehnutelnost);

            for (auto areaIt = areas.begin(); areaIt != areas.end(); ) {
                Area* area = *areaIt;
                if (area->nehnutelnost && area->nehnutelnost->equals(*oldNehnutelnost)) {
                    tree_area.removeNode(area);
                    areaIt = areas.erase(areaIt);
                } else {
                    ++areaIt;
                }
            }

            oldNehnutelnost->gps->x = newX1;
            oldNehnutelnost->gps->y = newY1;

            tree_nehnutelnost.insert(oldNehnutelnost, oldNehnutelnost->gps);

            Area* newArea = new Area(getBiggerUIDArea(), oldNehnutelnost->gps, oldNehnutelnost);
            areas.push_back(newArea);
            tree_area.insert(newArea, oldNehnutelnost->gps);
        }

        oldNehnutelnost->supisneCislo = newSupisneCislo;
        oldNehnutelnost->popis = newDescription;

        return true;
    }

    bool editParcela(int id, double newX1, double newY1, int newCisloParcely, const std::string& newDescription) {
        auto it = std::find_if(parcely.begin(), parcely.end(),
                               [id](Parcela* parcela) { return parcela->uid == id; });

        if (it == parcely.end()) {
            return false;
        }

        Parcela* oldParcela = *it;

        bool gpsChanged = (oldParcela->gps->x != newX1 || oldParcela->gps->y != newY1);

        if (gpsChanged) {
            tree_parcela.removeNode(oldParcela);

            for (auto areaIt = areas.begin(); areaIt != areas.end(); ) {
                Area* area = *areaIt;
                if (area->parcela && area->parcela->equals(*oldParcela)) {
                    tree_area.removeNode(area);
                    areaIt = areas.erase(areaIt);
                } else {
                    ++areaIt;
                }
            }

            oldParcela->gps->x = newX1;
            oldParcela->gps->y = newY1;

            tree_parcela.insert(oldParcela, oldParcela->gps);

            Area* newArea = new Area(getBiggerUIDArea(), oldParcela->gps, nullptr, oldParcela);
            areas.push_back(newArea);
            tree_area.insert(newArea, oldParcela->gps);
        }

        oldParcela->cisloParcely = newCisloParcely;
        oldParcela->popis = newDescription;

        return true;
    }


    std::vector<Nehnutelnost*> findNehnutelnosti(double x1, double y1, double x2, double y2) {
        GPS gps1(x1, y1);
        GPS gps2(x2, y2);

        std::vector<Nehnutelnost*> result1 = tree_nehnutelnost.find(&gps1);
        std::vector<Nehnutelnost*> result2 = tree_nehnutelnost.find(&gps2);

        result1.insert(result1.end(), result2.begin(), result2.end());
        return result1;
    }

    std::vector<Parcela*> findParcely(double x1, double y1, double x2, double y2) {
        GPS gps1(x1, y1);
        GPS gps2(x2, y2);

        std::vector<Parcela*> result1 = tree_parcela.find(&gps1);
        std::vector<Parcela*> result2 = tree_parcela.find(&gps2);

        result1.insert(result1.end(), result2.begin(), result2.end());
        return result1;
    }

    std::vector<Area*> findAreas(double x1, double y1, double x2, double y2) {
        GPS gps1(x1, y1);
        GPS gps2(x2, y2);

        std::vector<Area*> result1 = tree_area.find(&gps1);
        std::vector<Area*> result2 = tree_area.find(&gps2);

        result1.insert(result1.end(), result2.begin(), result2.end());
        return result1;
    }

    std::vector<Nehnutelnost*> findNehnutelnostiOnly (double x1, double y1) {
        GPS gps1(x1, y1);

        std::vector<Nehnutelnost*> result1 = tree_nehnutelnost.find(&gps1);

        return result1;
    }

    std::vector<Parcela*> findParcelyOnly(double x1, double y1) {
        GPS gps1(x1, y1);

        std::vector<Parcela*> result1 = tree_parcela.find(&gps1);
        return result1;
    }

    std::vector<Area*> findAreasOnly(double x1, double y1) {
        GPS gps1(x1, y1);

        std::vector<Area*> result1 = tree_area.find(&gps1);

        return result1;
    }


    std::vector<Nehnutelnost*> allNehnutelnosti() {
        std::vector<Nehnutelnost*> result;
        tree_nehnutelnost.levelOrderTraversal([&result](auto* node) {
            result.push_back(node->_data);
        });
        return result;
    }

    std::vector<Parcela*> allParcely() {
        std::vector<Parcela*> result;
        tree_parcela.levelOrderTraversal([&result](auto* node) {
            result.push_back(node->_data);
        });
        return result;
    }

    std::vector<Area*> allAreas() {
        std::vector<Area*> result;
        tree_area.levelOrderTraversal([&result](auto* node) {
            result.push_back(node->_data);
        });
        return result;
    }


    void LoadDataFromFile() {
        fileloader.loadNehnutelnosti(nehnutelnostiFile, tree_nehnutelnost, tree_area);
        fileloader.loadParcely(parcelyFile, tree_parcela, tree_area);
    }


    bool saveToFiles() {
        std::ofstream nehnutelnostiOut(nehnutelnostiFile);
        if (!nehnutelnostiOut) {
            std::cerr << "Failed to open " << nehnutelnostiFile << " for writing." << std::endl;
            return false;
        }

        std::ofstream parcelyOut(parcelyFile);
        if (!parcelyOut) {
            std::cerr << "Failed to open " << parcelyFile << " for writing." << std::endl;
            return false;
        }

        nehnutelnostiOut << "UID;GPS_X;GPS_Y;Width;Length;SupisneCislo;Popis\n";

        tree_nehnutelnost.levelOrderTraversal([&nehnutelnostiOut](auto node) {
            const Nehnutelnost* nehnutelnost = node->_data;
            nehnutelnostiOut << nehnutelnost->uid << ";"
                             << nehnutelnost->gps->x << ";"
                             << nehnutelnost->gps->y << ";"
                             << nehnutelnost->gps->width << ";"
                             << nehnutelnost->gps->length << ";"
                             << nehnutelnost->supisneCislo << ";"
                             << nehnutelnost->popis << "\n";
        });

        parcelyOut << "UID;GPS_X;GPS_Y;Width;Length;CisloParcely;Popis\n";

        tree_parcela.levelOrderTraversal([&parcelyOut](auto node) {
            const Parcela* parcela = node->_data;
            parcelyOut << parcela->uid << ";"
                       << parcela->gps->x << ";"
                       << parcela->gps->y << ";"
                       << parcela->gps->width << ";"
                       << parcela->gps->length << ";"
                       << parcela->cisloParcely << ";"
                       << parcela->popis << "\n";
        });

        nehnutelnostiOut.close();
        parcelyOut.close();

        return true;
    }


};



inline TreeDatabase::TreeDatabase(string nehnutelnostiFile, string parcelyFile) :  nehnutelnostiFile(nehnutelnostiFile), parcelyFile(parcelyFile) ,tree_nehnutelnost(2), tree_parcela(2), tree_area(2), fileloader(idNehnutelnost,idArea, idParcely, nehnutelnosti, parcely, areas)
{

}



