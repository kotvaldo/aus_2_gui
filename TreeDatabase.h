#pragma once
#include "FileLoader.h"
#include "KDTree.h"

template <typename Key, typename Data, typename KeyFactory, typename DataFactory, typename KeyParams, typename DataParams>
class TreeDatabase {
private:
    GeneralKDTree<Key, Data> tree;
    int k;
    FileLoader fileloader;
    std::vector<int> idGenerator{};
    std::string dataFile;
    std::string parcelyFile;
    KeyFactory keyFactory;
    DataFactory dataFactory;


public:
    TreeDatabase(int k , string dataFile);

    int getBiggerID() {
        if(idGenerator.empty()) {
            idGenerator.push_back(1);
        } else {
            idGenerator.push_back(idGenerator.size() + 1);
        }
        return idGenerator.size();
    }


    bool create(KeyParams keyParams, DataParams dataParams) {
        dataParams.id = getBiggerID();
        auto data = dataFactory.createInstance(keyParams, dataParams);
        tree.insert(data);
    }



    bool update(const KeyParams& oldCoords, const KeyParams& newCoords, const DataParams& params) {
        auto key_ptr = keyFactory.createInstance(oldCoords);

        auto duplicities = tree.find(key_ptr);

        auto it = std::find_if(duplicities.begin(), duplicities.end(),
                               [&](const std::shared_ptr<Data>& item) {
                                   return item->hasId(params.id); // Compare IDs
                               });

        if (it == duplicities.end()) {
            return false;
        }

        auto new_element = dataFactory.createInstance(newCoords, params);

        if (!tree.updateNode(*it, new_element)) {
            return false;
        }

        return true;
    }




    vector<std::shared_ptr<Data>> show(const KeyParams& keyParams) {
        std::shared_ptr<Key> key = keyFactory.createInstance(keyParams);
        std::vector<std::shared_ptr<Data>> duplicities = tree.find(key);
        return duplicities;
    }




    std::vector<std::shared_ptr<Data>> index() {
        std::vector<std::shared_ptr<Data>> elements;
        tree.levelOrderTraversal([&elements](auto* node) {
            elements.push_back(node->_data);
        });
        return elements;
    }

    bool destroy(int id, const KeyParams& keyParams) {
        auto key_ptr = keyFactory.createInstance(keyParams);

        auto duplicities = tree.find(key_ptr);

        auto it = std::find_if(duplicities.begin(), duplicities.end(),
                               [&](const std::shared_ptr<Data>& item) {
                                   return item->id == id;
                               });

        if (it == duplicities.end()) {
            return false;
        }
        tree.removeNode(*it);
        return true;
    }


    void LoadDataFromFile() {
        fileloader.loadParcely(parcelyFile, tree_parcela, tree_area);
        fileloader.loadNehnutelnosti(nehnutelnostiFile, tree_parcela, tree_nehnutelnost, tree_area);
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
            nehnutelnostiOut << nehnutelnost->getUid() << ";"
                             << nehnutelnost->getGps()->getX() << ";"
                             << nehnutelnost->getGps()->getY() << ";"
                             << nehnutelnost->getGps()->getWidth() << ";"
                             << nehnutelnost->getGps()->getLength() << ";"
                             << nehnutelnost->getSupisneCislo() << ";"
                             << nehnutelnost->getPopis() << "\n";
        });

        parcelyOut << "UID;GPS_X;GPS_Y;Width;Length;CisloParcely;Popis\n";

        tree_parcela.levelOrderTraversal([&parcelyOut](auto node) {
            const Parcela* parcela = node->_data;
            parcelyOut << parcela->getUid() << ";"
                       << parcela->getGps()->getX() << ";"
                       << parcela->getGps()->getX() << ";"
                       << parcela->getGps()->getWidth() << ";"
                       << parcela->getGps()->getLength() << ";"
                       << parcela->getCisloParcely() << ";"
                       << parcela->getPopis() << "\n";
        });

        nehnutelnostiOut.close();
        parcelyOut.close();

        return true;
    }


    void clearAllData() {
        tree.clear();
    }





    void generateRandomUnits(int countPar, int countNeh, double prekryv) {
        std::vector<Parcela*> parcels;

        if (prekryv > 1) prekryv = 1;
        if (prekryv < 0) prekryv = 0;

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

            Parcela* p = new Parcela(getBiggerUIDParcely(), gps, 12222222, "Testing parcel");
            parcels.push_back(p);
            GPS* gps_copy = new GPS(*gps);
            tree_parcela.insert(p, gps);
            tree_area.insert(new Area(getBiggerUIDArea(), gps_copy, nullptr, p), gps_copy);
        }

        int count_prekryv = static_cast<int>(round(prekryv * countNeh));
        int count_not_prekryv = countNeh - count_prekryv;


        for (int i = 0; i < count_prekryv; i++) {
            Parcela* existingParcel = parcels[rand() % parcels.size()];

            GPS* gps = new GPS(*existingParcel->getGps());

            Nehnutelnost* n = new Nehnutelnost(getBiggerIDNehnutelnosti(), gps, rand() % 1000, "Testing nehnutelnost");

            n->addParcela(existingParcel);
            existingParcel->addNehnutelnost(n);

            Area* a = new Area(getBiggerUIDArea(), new GPS(*gps), n, existingParcel);
            tree_nehnutelnost.insert(n, gps);
            tree_area.insert(a, a->getGps());
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

            Nehnutelnost* n = new Nehnutelnost(getBiggerIDNehnutelnosti(), gps, rand() % 1000, "Testing nehnutelnost");
            Area* a = new Area(getBiggerUIDArea(), new GPS(*gps), n, nullptr);

            tree_nehnutelnost.insert(n, gps);
            tree_area.insert(a, a->getGps());
        }
    }




};

template <typename Key, typename Data, typename KeyFactory, typename DataFactory, typename KeyParams, typename DataParams>
inline TreeDatabase<Key, Data, KeyFactory, DataFactory, KeyParams, DataParams>::TreeDatabase(int k , string dataFile) : tree(k), dataFile(dataFile)
{

}
