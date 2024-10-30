#pragma once
#include <vector>
#include "KDTree.h"
#include "Controller.h"
#include <fstream>
#include <sstream>


template <typename KeyType, typename DataType>
class DatabaseManager {
protected:
    GeneralKDTree<KeyType, DataType>* tree;
    std::vector<DataType*> databaseRecords;
    vector<int> uid_list;

public:
    DatabaseManager(GeneralKDTree<KeyType, DataType>* tree) : tree(tree) {}

    virtual void loadDatabaseFromFile(const std::string& filename) = 0;

    virtual void fillTreeWithDatabaseRecords() = 0;

    vector<DataType*> findRecords(KeyType* key) {
        tree->find(key);
    
    };

    virtual void insertRecord(DataType* record) = 0;

    void deleteRecord(DataType* record) {
        tree->removeNode(record);
    }

    void clearDatabase() {
        tree->clear();

    }

    void clearTree() {
        tree->clear();

    }
    int getUnicateId() {
        if (uid_list.empty()) {
            uid_list.push_back(1);
            return 1;
        }
        uid_list.push_back(uid_list.size() + 1);
        return uid_list.size();
    }


    virtual ~DatabaseManager() {
        for (auto record : databaseRecords) {
            delete record;
        }
    }

protected:
    virtual DataType* parseRecord(const std::string& line) = 0;
};


class NehnutelnostLoader : public DatabaseManager<GPS, Nehnutelnost> {
public:
    NehnutelnostLoader(GeneralKDTree<GPS, Nehnutelnost>* tree) : DatabaseManager(tree) {}

    void loadDatabaseFromFile(const std::string& filename) override {
        std::ifstream file(filename);
        std::string line;
        while (std::getline(file, line)) {
            Nehnutelnost* record = parseRecord(line);
            if (record) databaseRecords.push_back(record);
        }
    }

    void fillTreeWithDatabaseRecords() override {
        for (auto record : this->databaseRecords) {
            tree->insert(record, record->gps);
        }
    }
    virtual void insertRecord(Nehnutelnost* record) {
        tree->insert(record, record->gps);
    }

protected:
    Nehnutelnost* parseRecord(const std::string& line) override {
        std::istringstream iss(line);
        int id, x, y;
        if (iss >> id >> x >> y) {
            return new Nehnutelnost(id, new GPS(x, y));
        }
        return nullptr;
    }
};