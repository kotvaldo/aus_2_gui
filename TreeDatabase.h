#include "FileLoader.h"
#include "KDTree.h"
#include "Models.h"

class TreeDatabase {
private:
    GeneralKDTree<GPS, Nehnutelnost> tree_nehnutelnost;
    GeneralKDTree<GPS, Parcela> tree_parcela;
    GeneralKDTree<GPS, Area> tree_area;
    FileLoader& fileloader;
    vector<Area*> areas;
    std::vector<int>& sharedUIDs;

public:
    TreeDatabase(FileLoader &fileloader, const std::string& nehnutelnostiFile, const std::string& parcelyFile, std::vector<int>& uids) : sharedUIDs(uids),fileloader(fileloader), tree_nehnutelnost(2), tree_parcela(2), tree_area(2)
    {
        vector<Nehnutelnost*> nehnutelnosti;
        vector<Parcela*> parcely;
        if(fileloader.loadNehnutelnosti(nehnutelnostiFile)) {
            nehnutelnosti = fileloader.getNehnutelnosti();
        }
        if(fileloader.loadParcely(parcelyFile)) {
            parcely = fileloader.getParcely();
        }
        for(Parcela* k : parcely) {
            tree_parcela.insert(k, k->gps);
            Area* a = new Area(this->getBiggerUID(), k->gps, nullptr, k);
            areas.push_back(a);
            tree_area.insert(a, a->gps);
        }

        for(Nehnutelnost* k : nehnutelnosti) {
            tree_nehnutelnost.insert(k, k->gps);
            Area* a = new Area(this->getBiggerUID(), k->gps, k, nullptr);
            areas.push_back(a);
            tree_area.insert(a, a->gps);
        }


    }

    int getBiggerUID() const {
        return sharedUIDs.size();
    }



};


