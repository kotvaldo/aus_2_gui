#pragma once
#include "FileLoader.h"
#include "KDTree.h"
#include "Models.h"
#include "Parameters.h"

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
        return idArea.size();
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

    void addNehnutelnost(const BoundingBox& bbox, const NehnutelnostParameters& params) {
        GPS* gps1 = new GPS(bbox.point1.x, bbox.point1.y, bbox.point1.width, bbox.point1.length);
        GPS* gps2 = new GPS(bbox.point2.x, bbox.point2.y, bbox.point2.width, bbox.point2.length);

        Nehnutelnost* nehnutelnost1 = new Nehnutelnost(getBiggerIDNehnutelnosti(), gps1, params.supisneCislo, params.description);
        Nehnutelnost* nehnutelnost2 = new Nehnutelnost(getBiggerIDNehnutelnosti(), gps2, params.supisneCislo, params.description);

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

    void addSingleNehnutelnost(const GPSParameters& coords, const NehnutelnostParameters& params) {
        GPS* gps1 = new GPS(coords.x, coords.y, coords.width, coords.length);
        Nehnutelnost* nehnutelnost1 = new Nehnutelnost(getBiggerIDNehnutelnosti(), gps1, params.supisneCislo, params.description);

        nehnutelnosti.push_back(nehnutelnost1);
        tree_nehnutelnost.insert(nehnutelnost1, gps1);

        Area* area1 = new Area(getBiggerUIDArea(), gps1, nehnutelnost1, nullptr);
        areas.push_back(area1);
        tree_area.insert(area1, gps1);
    }

    void addParcela(const BoundingBox& bbox, const ParcelaParameters& params) {
        GPS* gps1 = new GPS(bbox.point1.x, bbox.point1.y, bbox.point1.width, bbox.point1.length);
        GPS* gps2 = new GPS(bbox.point2.x, bbox.point2.y, bbox.point2.width, bbox.point2.length);

        Parcela* parcela1 = new Parcela(getBiggerUIDParcely(), gps1, params.cisloParcely, params.description);
        Parcela* parcela2 = new Parcela(getBiggerUIDParcely(), gps2, params.cisloParcely, params.description);

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

    void addSingleParcela(const GPSParameters& coords, const ParcelaParameters& params) {
        GPS* gps1 = new GPS(coords.x, coords.y, coords.width, coords.length);
        Parcela* parcela1 = new Parcela(getBiggerUIDParcely(), gps1, params.cisloParcely, params.description);

        parcely.push_back(parcela1);
        tree_parcela.insert(parcela1, gps1);

        Area* area1 = new Area(getBiggerUIDArea(), gps1, nullptr, parcela1);
        areas.push_back(area1);
        tree_area.insert(area1, gps1);
    }

    bool editNehnutelnost(int id, const GPSParameters& newCoords, const NehnutelnostParameters& params) {
        auto it = std::find_if(nehnutelnosti.begin(), nehnutelnosti.end(),
                               [id](Nehnutelnost* nehnutelnost) { return nehnutelnost->uid == id; });

        if (it == nehnutelnosti.end()) {
            return false;
        }

        Nehnutelnost* oldNehnutelnost = *it;
        bool coordsChanged = oldNehnutelnost->gps->x != newCoords.x || oldNehnutelnost->gps->y != newCoords.y ||
                             oldNehnutelnost->gps->width != newCoords.width || oldNehnutelnost->gps->length != newCoords.length;

        if (coordsChanged) {
            GPS* newGPS = new GPS(newCoords.x, newCoords.y, newCoords.width, newCoords.length);


            for (auto areaIt = areas.begin(); areaIt != areas.end();) {
                Area* area = *areaIt;
                if (area->nehnutelnost && area->nehnutelnost->uid == id) {
                    tree_area.removeNode(area);
                    areaIt = areas.erase(areaIt);
                } else {
                    ++areaIt;
                }
            }

            tree_nehnutelnost.removeNode(oldNehnutelnost);
            oldNehnutelnost->gps = newGPS;
            tree_nehnutelnost.insert(oldNehnutelnost, newGPS);


            Area* newArea = new Area(getBiggerUIDArea(), newGPS, oldNehnutelnost, nullptr);
            tree_area.insert(newArea, newGPS);
            areas.push_back(newArea);
        }

        oldNehnutelnost->supisneCislo = params.supisneCislo;
        oldNehnutelnost->popis = params.description;

        return true;
    }

    bool editParcela(int id, const GPSParameters& newCoords, const ParcelaParameters& params) {
        auto it = std::find_if(parcely.begin(), parcely.end(),
                               [id](Parcela* parcela) { return parcela->uid == id; });

        if (it == parcely.end()) {
            return false;
        }

        Parcela* oldParcela = *it;
        bool coordsChanged = oldParcela->gps->x != newCoords.x || oldParcela->gps->y != newCoords.y ||
                             oldParcela->gps->width != newCoords.width || oldParcela->gps->length != newCoords.length;

        if (coordsChanged) {
            GPS* newGPS = new GPS(newCoords.x, newCoords.y, newCoords.width, newCoords.length);

            for (auto areaIt = areas.begin(); areaIt != areas.end();) {
                Area* area = *areaIt;
                if (area->parcela && area->parcela->uid == id) {
                    tree_area.removeNode(area);
                    areaIt = areas.erase(areaIt);
                } else {
                    ++areaIt;
                }
            }

             tree_parcela.removeNode(oldParcela);
            oldParcela->gps = newGPS;
            tree_parcela.insert(oldParcela, newGPS);

            Area* newArea = new Area(getBiggerUIDArea(), newGPS, nullptr, oldParcela);
            tree_area.insert(newArea, newGPS);
            areas.push_back(newArea);
        }

        oldParcela->cisloParcely = params.cisloParcely;
        oldParcela->popis = params.description;

        return true;
    }



    std::vector<Nehnutelnost*> findNehnutelnosti(const BoundingBox& bbox) {
        GPS gps1(bbox.point1.x, bbox.point1.y, bbox.point1.width, bbox.point1.length);
        GPS gps2(bbox.point2.x, bbox.point2.y, bbox.point2.width, bbox.point2.length);

        std::vector<Nehnutelnost*> result1 = tree_nehnutelnost.find(&gps1);
        std::vector<Nehnutelnost*> result2 = tree_nehnutelnost.find(&gps2);

        result1.insert(result1.end(), result2.begin(), result2.end());
        return result1;
    }

    std::vector<Parcela*> findParcely(const BoundingBox& bbox) {
        GPS gps1(bbox.point1.x, bbox.point1.y, bbox.point1.width, bbox.point1.length);
        GPS gps2(bbox.point2.x, bbox.point2.y, bbox.point2.width, bbox.point2.length);

        std::vector<Parcela*> result1 = tree_parcela.find(&gps1);
        std::vector<Parcela*> result2 = tree_parcela.find(&gps2);

        result1.insert(result1.end(), result2.begin(), result2.end());
        return result1;
    }

    std::vector<Area*> findAreas(const BoundingBox& bbox) {
        GPS gps1(bbox.point1.x, bbox.point1.y, bbox.point1.width, bbox.point1.length);
        GPS gps2(bbox.point2.x, bbox.point2.y, bbox.point2.width, bbox.point2.length);

        std::vector<Area*> result1 = tree_area.find(&gps1);
        std::vector<Area*> result2 = tree_area.find(&gps2);

        result1.insert(result1.end(), result2.begin(), result2.end());
        return result1;
    }

    std::vector<Nehnutelnost*> findNehnutelnostiOnly(const GPSParameters& coords) {
        GPS gps1(coords.x, coords.y, coords.width, coords.length);
        return tree_nehnutelnost.find(&gps1);
    }

    std::vector<Parcela*> findParcelyOnly(const GPSParameters& coords) {
        GPS gps1(coords.x, coords.y, coords.width, coords.length);
        return tree_parcela.find(&gps1);
    }


    std::vector<Area*> findAreasOnly(const GPSParameters& coords) {
        GPS gps1(coords.x, coords.y, coords.width, coords.length);
        return tree_area.find(&gps1);
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

    bool deleteAreaRecord(int id) {
        auto it = std::find_if(areas.begin(), areas.end(),
                               [id](Area* area) { return area->uid == id; });

        if (it == areas.end()) {
            std::cout << "Area with ID " << id << " not found in areas list." << std::endl;
            return false;
        }

        Area* area = *it;
        std::cout << "Deleting Area with ID " << id << std::endl;

         if (area->nehnutelnost) {
            std::cout << "Area has associated Nehnutelnost UID: " << area->nehnutelnost->uid << std::endl;

            tree_nehnutelnost.removeNode(area->nehnutelnost);
            std::cout << "Removed Nehnutelnost from tree with UID: " << area->nehnutelnost->uid << std::endl;

            auto nehIt = std::find(nehnutelnosti.begin(), nehnutelnosti.end(), area->nehnutelnost);
            if (nehIt != nehnutelnosti.end()) {
                nehnutelnosti.erase(nehIt);
                std::cout << "Removed Nehnutelnost from vector with UID: " << area->nehnutelnost->uid << std::endl;
            } else {
                std::cout << "Nehnutelnost not found in vector, UID: " << area->nehnutelnost->uid << std::endl;
            }
        } else if (area->parcela) {
            std::cout << "Area has associated Parcela UID: " << area->parcela->uid << std::endl;

            tree_parcela.removeNode(area->parcela);
            std::cout << "Removed Parcela from tree with UID: " << area->parcela->uid << std::endl;

            auto parIt = std::find(parcely.begin(), parcely.end(), area->parcela);
            if (parIt != parcely.end()) {
                parcely.erase(parIt);
                std::cout << "Removed Parcela from vector with UID: " << area->parcela->uid << std::endl;
            } else {
                std::cout << "Parcela not found in vector, UID: " << area->parcela->uid << std::endl;
            }
        }

        tree_area.removeNode(area);
        std::cout << "Removed Area from tree with ID: " << id << std::endl;

        delete area;
        areas.erase(it);
        std::cout << "Deleted Area with ID: " << id << " from vector." << std::endl;

        return true;
    }



    bool deleteNehnutelnostRecord(int id) {
        auto it = std::find_if(nehnutelnosti.begin(), nehnutelnosti.end(),
                               [id](Nehnutelnost* nehnutelnost) { return nehnutelnost->uid == id; });

        if (it == nehnutelnosti.end()) {
            return false;
        }
///
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


};



inline TreeDatabase::TreeDatabase(string nehnutelnostiFile, string parcelyFile) :  nehnutelnostiFile(nehnutelnostiFile), parcelyFile(parcelyFile) ,tree_nehnutelnost(4), tree_parcela(4), tree_area(4), fileloader(idNehnutelnost,idArea, idParcely, nehnutelnosti, parcely, areas)
{

}
