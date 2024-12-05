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





    void generateRandomUnits(int countPar, int countNeh, double overlapPercentage) {
        if (overlapPercentage > 1) overlapPercentage = 1;
        if (overlapPercentage < 0) overlapPercentage = 0;

        std::vector<std::shared_ptr<Data>> parcels;

        srand(static_cast<unsigned>(time(0)));

        for (int i = 0; i < countPar; i++) {
            auto keyParams = std::make_shared<KeyParams>();
            auto dataParams = std::make_shared<DataParams>();

            keyParams->randomize();  // Náhodné GPS parametre
            dataParams->id = getBiggerID();
            dataParams->description = "Testing parcel";

            auto parcel = dataFactory.createInstance(keyParams, dataParams);
            parcels.push_back(parcel);

            tree.insert(parcel);
        }

        int overlapCount = static_cast<int>(round(overlapPercentage * countNeh));
        int nonOverlapCount = countNeh - overlapCount;

        for (int i = 0; i < overlapCount; i++) {
            auto existingParcel = parcels[rand() % parcels.size()];

            auto keyParams = std::make_shared<KeyParams>(*existingParcel->getKeyParams());
            auto dataParams = std::make_shared<DataParams>();

            dataParams->id = getBiggerID();
            dataParams->description = "Testing nehnutelnost";

            auto nehnutelnost = dataFactory.createInstance(keyParams, dataParams);

            existingParcel->addRelated(nehnutelnost);
            nehnutelnost->addRelated(existingParcel);

            tree.insert(nehnutelnost);
        }

        for (int i = 0; i < nonOverlapCount; i++) {
            auto keyParams = std::make_shared<KeyParams>();
            auto dataParams = std::make_shared<DataParams>();

            keyParams->randomize();  // Náhodné GPS parametre
            dataParams->id = getBiggerID();
            dataParams->description = "Testing nehnutelnost";

            auto nehnutelnost = dataFactory.createInstance(keyParams, dataParams);

            tree.insert(nehnutelnost);
        }
    }


};

template <typename Key, typename Data, typename KeyFactory, typename DataFactory, typename KeyParams, typename DataParams>
inline TreeDatabase<Key, Data, KeyFactory, DataFactory, KeyParams, DataParams>::TreeDatabase(int k , string dataFile) : tree(k), dataFile(dataFile)
{

}
