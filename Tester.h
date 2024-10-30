#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <string>
#include "Models.h"
#include "KDTree.h"

using namespace std;

template <typename TestClass>
class Tester {
private:
    vector<TestClass*> data_list;
    GeneralKDTree<TestClass, TestClass> tree;
    vector<int> uid_list;

public:
    Tester() : tree(4) {} // Štyri dimenzie pre 4-rozmerný K-d strom

    void genPoints(int num_points, int range_min, int range_max, unsigned int seed = 0, bool desc = false) {
        if (seed == 0) {
            seed = static_cast<unsigned int>(time(0));
        }
        mt19937 gen(seed);
        uniform_int_distribution<> dis(range_min, range_max);
        uniform_real_distribution<> dis_double(static_cast<double>(range_min), static_cast<double>(range_max));

        if (desc) cout << "Using seed: " << seed << endl;

        for (int i = 0; i < num_points; ++i) {
            int uid = this->getUnicateId();
            double A = static_cast<double>(dis(gen));
            string B = "Name" + std::to_string(dis(gen) % 100); // Random string
            int C = dis(gen);
            double D = static_cast<double>(dis(gen));

            TestClass* test_obj = new TestClass(uid, A, B, C, D);
            tree.insert(test_obj, test_obj);
            data_list.push_back(test_obj);

            if (desc) cout << "Point " << i + 1 << ": " << *test_obj << " added successfully" << endl;
        }

        if (desc) treeSizeCheck();
    }

    void testSeeds(int min_seed, int max_seed) {
        for (int seed = min_seed; seed <= max_seed; ++seed) {
            cout << "Testing seed: " << seed << endl;

            mt19937 gen(seed);
            uniform_int_distribution<> dis(0, 100);
            uniform_real_distribution<> dis_double(0.0, 100.0);

            for (int i = 0; i < 10; ++i) {
                int uid = this->getUnicateId();
                double A = dis_double(gen);
                string B = "Test" + to_string(dis(gen) % 50);
                int C = dis(gen);
                double D = dis_double(gen);

                TestClass* test_obj = new TestClass(uid, A, B, C, D);
                tree.insert(test_obj, test_obj);
            }
            treeSizeCheck();
        }
    }

    void oscilate(int count) {
        for (int i = 1; i <= count; ++i) {
            cout << "Oscillation " << i << " in progress..." << endl;
            genPoints(10, 0, 100, 0);

            treeSizeCheck();

            clearStructure();
            cout << "Oscillation " << i << " complete." << endl;
        }
    }

    void treeSizeCheck() {
        int realSize = 0;
        tree.inOrderTraversal([&](KDTreeNode<TestClass, TestClass>* node) {
            realSize++;
            });

        if (tree.size() == realSize) {
            cout << "Tree size verification successful: " << realSize << " nodes in tree." << endl;
        }
        else {
            cout << "Tree size mismatch! Actual: " << realSize << " vs Expected: " << tree.size() << endl;
        }
    }

    void findDataWithDuplicates(double A, string B, int C, double D) {
        if (tree.size() != 0) {
            TestClass query_obj(-1, A, B, C, D); // Vytvoríme objekt na vyh¾adávanie pod¾a hodnôt
            vector<TestClass*> results = tree.find(&query_obj);

            if (results.empty()) {
                cout << "No data found for specified attributes." << endl;
            }
            else {
                cout << "Found " << results.size() << " entries with specified attributes:" << endl;
                for (auto d : results) {
                    cout << *d << endl;
                }
            }
        }
        else {
            cout << "Tree is empty. Please generate data first." << endl;
        }
    }



    void deleteTestWithParams(int id) {
        TestClass* target = nullptr;

        for (auto it = data_list.begin(); it != data_list.end(); ++it) {
            if ((*it)->uid == id) {
                target = *it;

                if (tree.removeNode(target)) {
                    cout << "Entry deleted successfully." << endl;
                    data_list.erase(it);
                }
                else {
                    cout << "Failed to delete the entry from the tree." << endl;
                }
                break;
            }
        }

        if (target == nullptr) {
            cout << "No entry found with the given ID: " << id << endl;
        }
    }

    void printTreeNodes() {
        if (tree.size() == 0) {
            cout << "The tree is empty." << endl;
            return;
        }

        
        tree.levelOrderTraversal([&](KDTreeNode<TestClass, TestClass>* node) {
            if (node != nullptr && node->_data != nullptr) {
                cout << "Key: " << *(node->_keyPart)
                    << ", Data: " << *(node->_data) << endl;
            }
            });

        treeSizeCheck();
    }


    void clearStructure() {
        tree.clear();
        for (TestClass* obj : data_list) {
            delete obj;
        }

        uid_list.clear();
        data_list.clear();
        cout << "All records removed! Current tree size: " << tree.size() << endl;
    }

    ~Tester() {
        clearStructure();
    }

    void checkReferences() {
        if (tree.size() == 0) {
            cout << "The tree is empty." << endl;
            return;
        }

        bool isConsistent = true; // Flag na sledovanie konzistencie ukazovateľov

        tree.levelOrderTraversal([&](KDTreeNode<TestClass, TestClass>* node) {
            if (node != nullptr) {
                // Informácie o aktuálnom uzle
                cout << "Checking node with key: " << *(node->_keyPart) << ", Level: " << node->_level << endl;

                // Kontrola konzistencie pravého dieťaťa a jeho rodiča
                if (node->_right != nullptr) {
                    if (node->_right->parent != node) {
                        cout << "Inconsistency detected at node with key: " << *(node->_keyPart) << " (Level " << node->_level << ") - "
                            << "Right child's parent does not match current node." << endl;
                        cout << "Right child's key: " << *(node->_right->_keyPart) << ", Right child's parent key: "
                            << *(node->_right->parent->_keyPart) << endl;
                        isConsistent = false;
                    }
                }

                // Kontrola konzistencie ľavého dieťaťa a jeho rodiča
                if (node->_left != nullptr) {
                    if (node->_left->parent != node) {
                        cout << "Inconsistency detected at node with key: " << *(node->_keyPart) << " (Level " << node->_level << ") - "
                            << "Left child's parent does not match current node." << endl;
                        cout << "Left child's key: " << *(node->_left->_keyPart) << ", Left child's parent key: "
                            << *(node->_left->parent->_keyPart) << endl;
                        isConsistent = false;
                    }
                }
            }
            });

        if (isConsistent) {
            cout << "All references are consistent." << endl;
        }
    }

    int getUnicateId() {
        if (uid_list.empty()) {
            uid_list.push_back(1);
            return 1;
        }
        uid_list.push_back(uid_list.size() + 1);
        return uid_list.size();
    }
};