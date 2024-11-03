#include "KDTree.h"
#include "Models.h"

template <typename DataType>
class TreeDatabase {
private:
    GeneralKDTree<GPS, DataType> dataTree;

public:
    bool loadAndInsert(const std::vector<DataType*>& data) {
        for (const auto* item : data) {
            if (item && item->gps) {
                dataTree.insert(item, item->gps);
            } else {
                throw std::invalid_argument("Data item or GPS key cannot be nullptr");
            }
        }
        return true;
    }

    std::vector<DataType*> FindDuplicities(const GPS& gps1, const GPS& gps2) {
        std::vector<DataType*> results1 = dataTree.find(gps1);
        std::vector<DataType*> results2 = dataTree.find(gps2);

        // Nájdeme spoločné prvky
        std::vector<DataType*> duplicates;
        std::sort(results1.begin(), results1.end());
        std::sort(results2.begin(), results2.end());
        std::set_intersection(
            results1.begin(), results1.end(),
            results2.begin(), results2.end(),
            std::back_inserter(duplicates)
            );

        return duplicates;
    }

    const GeneralKDTree<GPS, DataType>& getTree() const {
        return dataTree;
    }
};
