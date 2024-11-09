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


        tree_nehnutelnost.insert(nehnutelnost1, gps1);
        tree_nehnutelnost.insert(nehnutelnost2, gps2);

        Area* area1 = new Area(getBiggerUIDArea(), gps1, nehnutelnost1, nullptr);
        Area* area2 = new Area(getBiggerUIDArea(), gps2, nehnutelnost2, nullptr);



        tree_area.insert(area1, gps1);
        tree_area.insert(area2, gps2);
    }

    void addSingleNehnutelnost(const GPSParameters& coords, const NehnutelnostParameters& params) {
        GPS* gps1 = new GPS(coords.x, coords.y, coords.width, coords.length);
        Nehnutelnost* nehnutelnost1 = new Nehnutelnost(getBiggerIDNehnutelnosti(), gps1, params.supisneCislo, params.description);

        tree_nehnutelnost.insert(nehnutelnost1, gps1);

        Area* area1 = new Area(getBiggerUIDArea(), gps1, nehnutelnost1, nullptr);
        tree_area.insert(area1, gps1);
    }

    void addParcela(const BoundingBox& bbox, const ParcelaParameters& params) {
        GPS* gps1 = new GPS(bbox.point1.x, bbox.point1.y, bbox.point1.width, bbox.point1.length);
        GPS* gps2 = new GPS(bbox.point2.x, bbox.point2.y, bbox.point2.width, bbox.point2.length);

        Parcela* parcela1 = new Parcela(getBiggerUIDParcely(), gps1, params.cisloParcely, params.description);
        Parcela* parcela2 = new Parcela(getBiggerUIDParcely(), gps2, params.cisloParcely, params.description);


        tree_parcela.insert(parcela1, gps1);
        tree_parcela.insert(parcela2, gps2);

        Area* area1 = new Area(getBiggerUIDArea(), gps1, nullptr, parcela1);
        Area* area2 = new Area(getBiggerUIDArea(), gps2, nullptr, parcela2);


        tree_area.insert(area1, gps1);
        tree_area.insert(area2, gps2);
    }

    void addSingleParcela(const GPSParameters& coords, const ParcelaParameters& params) {
        GPS* gps1 = new GPS(coords.x, coords.y, coords.width, coords.length);
        Parcela* parcela1 = new Parcela(getBiggerUIDParcely(), gps1, params.cisloParcely, params.description);

        tree_parcela.insert(parcela1, gps1);

        Area* area1 = new Area(getBiggerUIDArea(), gps1, nullptr, parcela1);
        tree_area.insert(area1, gps1);
    }

    bool editNehnutelnost(int id, const GPSParameters& oldCoords, const GPSParameters& newCoords, const NehnutelnostParameters& params) {
        GPS oldGPS(oldCoords.x, oldCoords.y, oldCoords.width, oldCoords.length);
        GPS* newGPS = new GPS(newCoords.x, newCoords.y, newCoords.width, newCoords.length);

        vector<Nehnutelnost*> nehnutelnost_dup = tree_nehnutelnost.find(&oldGPS);
        Nehnutelnost* oldNehnutelnost = nullptr;
        for (Nehnutelnost* n : nehnutelnost_dup) {
            if (n->uid == id) {
                oldNehnutelnost = n;
                break;
            }
        }





        vector<Area*> areasDup = tree_area.find(&oldGPS);
        Area* oldArea = nullptr;
        for (Area* a : areasDup) {
            if (a->nehnutelnost == oldNehnutelnost) {
                oldArea = a;
                break;
            }
        }

        if(!oldArea) return false;

        Nehnutelnost* newNehnutelnost = new Nehnutelnost(id, newGPS, params.supisneCislo, params.description);
        tree_nehnutelnost.updateNode(oldNehnutelnost, &oldGPS, newNehnutelnost, newGPS);

        Area* newArea = new Area(oldArea->uid, newGPS, newNehnutelnost, nullptr);
        tree_area.updateNode(oldArea, &oldGPS, newArea, newGPS);



        return true;
    }

    bool editParcela(int id, const GPSParameters& oldCoords, const GPSParameters& newCoords, const ParcelaParameters& params) {
        GPS oldGPS(oldCoords.x, oldCoords.y, oldCoords.width, oldCoords.length);
        GPS* newGPS = new GPS(newCoords.x, newCoords.y, newCoords.width, newCoords.length);

        vector<Parcela*> parcela_dup = tree_parcela.find(&oldGPS);
        Parcela* oldParcela = nullptr;
        for (Parcela* p : parcela_dup) {
            if (p->uid == id) {
                oldParcela = p;
                break;
            }
        }

        if (oldParcela == nullptr) {
            return false;
        }


        vector<Area*> areasDup = tree_area.find(&oldGPS);
        Area* oldArea = nullptr;
        for (Area* a : areasDup) {
            if (a->parcela == oldParcela) {
                oldArea = a;
                break;
            }
        }

        if(!oldArea) return false;

        Parcela* newParcela = new Parcela(id, newGPS, params.cisloParcely, params.description);
        tree_parcela.updateNode(oldParcela, &oldGPS, newParcela, newGPS);

        Area* newArea = new Area(oldArea->uid, newGPS, nullptr, newParcela);
        tree_area.updateNode(oldArea, &oldGPS, newArea, newGPS);



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

    bool deleteAreaRecord(int id, const GPSParameters& coords) {
        GPS gps(coords.x, coords.y, coords.width, coords.length);

        std::cout << "Searching Area with GPS: (x: " << coords.x << ", y: " << coords.y
                  << ", width: " << coords.width << ", length: " << coords.length << ")" << std::endl;

        vector<Area*> areasDup = tree_area.find(&gps);

        for (Area* a : areasDup) {
            std::cout << "Found Area with UID: " << a->uid << ", GPS: (x: "
                      << a->gps->x << ", y: " << a->gps->y
                      << ", width: " << a->gps->width
                      << ", length: " << a->gps->length << ")" << std::endl;
        }

        Area* area = nullptr;
        for (Area* a : areasDup) {
            if (a->uid == id) {
                area = a;
                break;
            }
        }

        if (area == nullptr) {
            std::cout << "Error: Area with ID " << id << " not found." << std::endl;
            return false;
        }

        std::cout << "Deleting Area with ID " << id << std::endl;

        if (area->nehnutelnost) {
            std::cout << "Area has associated Nehnutelnost UID: " << area->nehnutelnost->uid << std::endl;
            tree_nehnutelnost.removeNode(area->nehnutelnost);
            std::cout << "Removed Nehnutelnost from tree with UID: " << area->nehnutelnost->uid << std::endl;

        } else if (area->parcela) {
            std::cout << "Area has associated Parcela UID: " << area->parcela->uid << std::endl;
            tree_parcela.removeNode(area->parcela);
            std::cout << "Removed Parcela from tree with UID: " << area->parcela->uid << std::endl;
        }

        tree_area.removeNode(area);
        std::cout << "Removed Area from tree with ID: " << id << std::endl;

        return true;
    }

    bool deleteNehnutelnostRecord(int id, const GPSParameters& coords) {
        GPS gps(coords.x, coords.y, coords.width, coords.length);

        std::cout << "Searching Nehnutelnost with GPS: (x: " << coords.x << ", y: " << coords.y
                  << ", width: " << coords.width << ", length: " << coords.length << ")" << std::endl;

        vector<Nehnutelnost*> nehnutelnost_dup = tree_nehnutelnost.find(&gps);
        vector<Area*> areasDup = tree_area.find(&gps);

        for (Nehnutelnost* n : nehnutelnost_dup) {
            std::cout << "Found Nehnutelnost with UID: " << n->uid << ", GPS: (x: "
                      << n->gps->x << ", y: " << n->gps->y
                      << ", width: " << n->gps->width
                      << ", length: " << n->gps->length << ")" << std::endl;
        }

        Nehnutelnost* nehnutelnost = nullptr;
        for (Nehnutelnost* n : nehnutelnost_dup) {
            if (n->uid == id) {
                nehnutelnost = n;
                break;
            }
        }

        if (nehnutelnost == nullptr) {
            std::cout << "Error: Nehnutelnost with ID " << id << " not found." << std::endl;
            return false;
        }

        std::cout << "Deleting Nehnutelnost with ID " << id << std::endl;

        Area* area = nullptr;
        for (Area* a : areasDup) {
            if (a->nehnutelnost == nehnutelnost) {
                area = a;
                break;
            }
        }

        tree_nehnutelnost.removeNode(nehnutelnost);
        std::cout << "Removed Nehnutelnost from tree with UID: " << id << std::endl;

        if (area) {
            tree_area.removeNode(area);
            std::cout << "Removed associated Area from tree with ID: " << area->uid << std::endl;
        } else {
            std::cout << "No associated Area found for Nehnutelnost with ID: " << id << std::endl;
        }

        return true;
    }

    bool deleteParcelaRecord(int id, const GPSParameters& coords) {
        GPS gps(coords.x, coords.y, coords.width, coords.length);

        std::cout << "Searching Parcela with GPS: (x: " << coords.x << ", y: " << coords.y
                  << ", width: " << coords.width << ", length: " << coords.length << ")" << std::endl;

        vector<Parcela*> parcela_dup = tree_parcela.find(&gps);
        vector<Area*> areasDup = tree_area.find(&gps);

        for (Parcela* p : parcela_dup) {
            std::cout << "Found Parcela with UID: " << p->uid << ", GPS: (x: "
                      << p->gps->x << ", y: " << p->gps->y
                      << ", width: " << p->gps->width
                      << ", length: " << p->gps->length << ")" << std::endl;
        }

        Parcela* parcela = nullptr;
        for (Parcela* p : parcela_dup) {
            if (p->uid == id) {
                parcela = p;
                break;
            }
        }

        if (parcela == nullptr) {
            std::cout << "Error: Parcela with ID " << id << " not found." << std::endl;
            return false;
        }

        std::cout << "Deleting Parcela with ID " << id << std::endl;

        Area* area = nullptr;
        for (Area* a : areasDup) {
            if (a->parcela == parcela) {
                area = a;
                break;
            }
        }

        tree_parcela.removeNode(parcela);
        std::cout << "Removed Parcela from tree with UID: " << id << std::endl;

        if (area) {
            tree_area.removeNode(area);
            std::cout << "Removed associated Area from tree with ID: " << area->uid << std::endl;
        } else {
            std::cout << "No associated Area found for Parcela with ID: " << id << std::endl;
        }

        return true;
    }



    void clearAllData() {
        tree_area.clear();
        tree_nehnutelnost.clear();
        tree_parcela.clear();


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
        }

    }


};



inline TreeDatabase::TreeDatabase(string nehnutelnostiFile, string parcelyFile) :  nehnutelnostiFile(nehnutelnostiFile), parcelyFile(parcelyFile) ,tree_nehnutelnost(4), tree_parcela(4), tree_area(4), fileloader(idNehnutelnost,idArea, idParcely)
{

}
