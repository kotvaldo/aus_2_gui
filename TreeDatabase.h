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
public:
    TreeDatabase(const std::string& nehnutelnostiFile, const std::string& parcelyFile);

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


    void clearAllData() {
        tree_area.clear();
        tree_nehnutelnost.clear();
        tree_parcela.clear();
    }



    bool deleteNehnutelnostRecord(int id, int x, int y) {
        return true;
    }

    bool deleteParcelaRecord() {
        return true;
    }

};



inline TreeDatabase::TreeDatabase(const string &nehnutelnostiFile, const string &parcelyFile) : tree_nehnutelnost(2), tree_parcela(2), tree_area(2), fileloader(idNehnutelnost, idParcely, nehnutelnosti, parcely)
{
    if(fileloader.loadNehnutelnosti(nehnutelnostiFile)) {
        nehnutelnosti = fileloader.getNehnutelnosti();
    }
    if(fileloader.loadParcely(parcelyFile)) {
        parcely = fileloader.getParcely();
    }
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
