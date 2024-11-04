#pragma once
#include "FileLoader.h"
#include "KDTree.h"
#include "Models.h"

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

    void addNehnutelnost(int x1, int y1, int x2, int y2, int supisneCislo, const std::string& description) {
        GPS* gps1 = new GPS(x1, y1);
        GPS* gps2 = new GPS(x2, y2);

        Nehnutelnost* nehnutelnost1 = new Nehnutelnost(this->getBiggerIDNehnutelnosti(), gps1, supisneCislo, description);
        Nehnutelnost* nehnutelnost2 = new Nehnutelnost(this->getBiggerIDNehnutelnosti(), gps2, supisneCislo, description);

        nehnutelnosti.push_back(nehnutelnost1);
        nehnutelnosti.push_back(nehnutelnost2);

        tree_nehnutelnost.insert(nehnutelnost1, gps1);
        tree_nehnutelnost.insert(nehnutelnost2, gps2);

        Area* area1 = new Area{getBiggerUIDArea(), gps1,nehnutelnost1, nullptr};
        Area* area2 = new Area{getBiggerUIDArea(), gps2, nehnutelnost2, nullptr};

        areas.push_back(area1);
        areas.push_back(area2);

        tree_area.insert(area1, gps1);
        tree_area.insert(area2, gps2);
    }

    void addParcela(int x1, int y1, int x2, int y2, int cisloParcely, const std::string& description) {
        GPS* gps1 = new GPS(x1, y1);
        GPS* gps2 = new GPS(x2, y2);

        Parcela* parcela1 = new Parcela(getBiggerUIDParcely(), gps1, cisloParcely, description);
        Parcela* parcela2 = new Parcela(getBiggerUIDParcely(), gps2, cisloParcely, description);

        parcely.push_back(parcela1);
        parcely.push_back(parcela2);

        tree_parcela.insert(parcela1, gps1);
        tree_parcela.insert(parcela2, gps2);

        Area* area1 = new Area{getBiggerUIDArea(), gps1, nullptr, parcela1};
        Area* area2 = new Area{getBiggerUIDArea(), gps2, nullptr, parcela2};

        areas.push_back(area1);
        areas.push_back(area2);

        tree_area.insert(area1, gps1);
        tree_area.insert(area2, gps2);
    }


    void reloadAllData() {
        tree_area.clear();
        tree_nehnutelnost.clear();
        tree_parcela.clear();

        for (Nehnutelnost* nehnutelnost : nehnutelnosti) {
            delete nehnutelnost;
        }
        nehnutelnosti.clear();

        for (Parcela* parcela : parcely) {
            delete parcela;
        }
        parcely.clear();

        for (Area* area : areas) {
            delete area;
        }
        areas.clear();

        idArea.clear();
        idNehnutelnost.clear();
        idParcely.clear();

        fileloader.loadNehnutelnosti(nehnutelnostiFile);
        fileloader.loadParcely(parcelyFile);

        for(Parcela* k : parcely) {
            tree_parcela.insert(k, k->gps);
            Area* a = new Area(this->getBiggerUIDArea(), k->gps, nullptr, k);
            areas.push_back(a);
            tree_area.insert(a, a->gps);
        }

        for(Nehnutelnost* k : nehnutelnosti) {
            tree_nehnutelnost.insert(k, k->gps);
            Area* a = new Area(this->getBiggerUIDArea(), k->gps, k, nullptr);
            areas.push_back(a);
            tree_area.insert(a, a->gps);
        }
    }


    bool deleteAreaRecord(int id) {
        auto it = std::find_if(areas.begin(), areas.end(),
                               [id](Area* area) { return (area->nehnutelnost && area->nehnutelnost->uid == id) ||
                                                         (area->parcela && area->parcela->uid == id); });

        if (it == areas.end()) {
            return false;
        }

        Area* area = *it;

        tree_area.removeNode(area);

        // Kontrola a odstránenie z nehnutelností alebo parciel podľa pripojenej entity
        if (area->nehnutelnost) {
            deleteNehnutelnostRecord(area->nehnutelnost->uid);
        } else if (area->parcela) {
            deleteParcelaRecord(area->parcela->uid);
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

        delete nehnutelnost;
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


        delete parcela;
        parcely.erase(it);

        return true;
    }

    bool editNehnutelnost(int id, int newX1, int newY1, int newX2, int newY2, int newSupisneCislo, const std::string& newDescription) {

        auto it = std::find_if(nehnutelnosti.begin(), nehnutelnosti.end(),
                               [id](Nehnutelnost* nehnutelnost) { return nehnutelnost->uid == id; });

        if (it == nehnutelnosti.end()) {
            return false;
        }

        Nehnutelnost* oldNehnutelnost = *it;

        tree_nehnutelnost.removeNode(oldNehnutelnost);

        for (auto areaIt = areas.begin(); areaIt != areas.end(); ) {
            Area* area = *areaIt;
            if (area->nehnutelnost && area->nehnutelnost->equals(*oldNehnutelnost)) {
                tree_area.removeNode(area);
                delete area;
                areaIt = areas.erase(areaIt);
            } else {
                ++areaIt;
            }
        }

        delete oldNehnutelnost;
        nehnutelnosti.erase(it);

        addNehnutelnost(newX1, newY1, newX2, newY2, newSupisneCislo, newDescription);

        return true;
    }

    bool editParcela(int id, int newX1, int newY1, int newX2, int newY2, int newCisloParcely, const std::string& newDescription) {
        auto it = std::find_if(parcely.begin(), parcely.end(),
                               [id](Parcela* parcela) { return parcela->uid == id; });

        if (it == parcely.end()) {
            return false;
        }

        Parcela* oldParcela = *it;

        tree_parcela.removeNode(oldParcela);

        for (auto areaIt = areas.begin(); areaIt != areas.end(); ) {
            Area* area = *areaIt;
            if (area->parcela && area->parcela->equals(*oldParcela)) {
                tree_area.removeNode(area);
                delete area;
                areaIt = areas.erase(areaIt);
            } else {
                ++areaIt;
            }
        }

        delete oldParcela;
        parcely.erase(it);

        addParcela(newX1, newY1, newX2, newY2, newCisloParcely, newDescription);

        return true;
    }


    std::vector<Nehnutelnost*> findNehnutelnosti(int x1, int y1, int x2, int y2) {
        GPS gps1(x1, y1);
        GPS gps2(x2, y2);

        std::vector<Nehnutelnost*> result1 = tree_nehnutelnost.find(&gps1);
        std::vector<Nehnutelnost*> result2 = tree_nehnutelnost.find(&gps2);

        result1.insert(result1.end(), result2.begin(), result2.end());
        return result1;
    }

    std::vector<Parcela*> findParcely(int x1, int y1, int x2, int y2) {
        GPS gps1(x1, y1);
        GPS gps2(x2, y2);

        std::vector<Parcela*> result1 = tree_parcela.find(&gps1);
        std::vector<Parcela*> result2 = tree_parcela.find(&gps2);

        result1.insert(result1.end(), result2.begin(), result2.end());
        return result1;
    }

    std::vector<Area*> findAreas(int x1, int y1, int x2, int y2) {
        GPS gps1(x1, y1);
        GPS gps2(x2, y2);

        std::vector<Area*> result1 = tree_area.find(&gps1);
        std::vector<Area*> result2 = tree_area.find(&gps2);

        result1.insert(result1.end(), result2.begin(), result2.end());
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

        nehnutelnostiOut << "UID;GPS_X;GPS_Y;SupisneCislo;Popis\n";

        tree_nehnutelnost.levelOrderTraversal([&nehnutelnostiOut](auto node) {
            const Nehnutelnost* nehnutelnost = node->_data;
            nehnutelnostiOut << nehnutelnost->uid << ";"
                             << nehnutelnost->gps->x << ";"
                             << nehnutelnost->gps->y << ";"
                             << nehnutelnost->supisneCislo << ";"
                             << nehnutelnost->popis << "\n";
        });

        parcelyOut << "UID;GPS_X;GPS_Y;CisloParcely;Popis\n";

        tree_parcela.levelOrderTraversal([&parcelyOut](auto node) {
            const Parcela* parcela = node->_data;
            parcelyOut << parcela->uid << ";"
                       << parcela->gps->x << ";"
                       << parcela->gps->y << ";"
                       << parcela->cisloParcely << ";"
                       << parcela->popis << "\n";
        });


        nehnutelnostiOut.close();
        parcelyOut.close();

        return true;
    }


};



inline TreeDatabase::TreeDatabase(string nehnutelnostiFile, string parcelyFile) :  nehnutelnostiFile(nehnutelnostiFile), parcelyFile(parcelyFile) ,tree_nehnutelnost(2), tree_parcela(2), tree_area(2), fileloader(idNehnutelnost, idParcely, nehnutelnosti, parcely)
{
    fileloader.loadNehnutelnosti(nehnutelnostiFile);
    fileloader.loadParcely(parcelyFile);

    for(Parcela* k : parcely) {
        tree_parcela.insert(k, k->gps);
        Area* a = new Area(this->getBiggerUIDArea(), k->gps, nullptr, k);
        areas.push_back(a);
        tree_area.insert(a, a->gps);
    }

    for(Nehnutelnost* k : nehnutelnosti) {
        tree_nehnutelnost.insert(k, k->gps);
        Area* a = new Area(this->getBiggerUIDArea(), k->gps, k, nullptr);
        areas.push_back(a);
        tree_area.insert(a, a->gps);
    }

}



