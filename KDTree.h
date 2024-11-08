#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <stack>
#include <stack>
#include <queue>
#include <functional>
#include <unordered_set>
#include <list>

#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define UNKNOWN     "\033[33m"

using namespace std;

template <typename KeyType, typename DataType>
struct KDTreeNode {
    KDTreeNode* parent;
    KDTreeNode* _left;
    KDTreeNode* _right;
    int _level;
    DataType* _data;
    KeyType* _keyPart;

    KDTreeNode(DataType* data, KeyType* keys, int level = 0)
        : _data(data), _keyPart(keys), _level(level), parent(nullptr), _left(nullptr), _right(nullptr) {}
};

template <typename KeyType, typename DataType>
class GeneralKDTree {
    using KDNodeType = KDTreeNode<KeyType, DataType>;
public:
    GeneralKDTree(size_t dim_count);
    ~GeneralKDTree();
    void clear();
    vector<DataType*> find(KeyType* keys);
    DataType* insert(DataType* data, KeyType* keys);
    bool removeNode(DataType* data, KDNodeType* startNode = nullptr, int targetDimension = -1);
    bool updateNode(DataType* oldData, KeyType* oldKeys, DataType* newData, KeyType* newKeys);
    size_t size() const;
    KDNodeType* accessRoot();
    bool hasLeftSon(KDNodeType* node);
    bool hasRightSon(KDNodeType* node);
    bool isLeaf(KDNodeType* node);
    bool isLeftSon(KDNodeType* node, KDNodeType* parent);
    bool isRightSon(KDNodeType* node, KDNodeType* parent);
    KDNodeType* findMaxInLeftSubTree(KDNodeType* parent);
    KDNodeType* findMinInRightSubTree(KDNodeType* parent);
    void inOrderTraversal(std::function<void(KDNodeType*)> func, KDNodeType* startNode = nullptr);
    void levelOrderTraversal(std::function<void(KDNodeType*)> func);
    void reverseLevelOrderTraversal(std::function<void(KDNodeType*)> func);
private:
    int size_;
    KDNodeType* root;
    size_t k;
    KDNodeType* findNodeWithData(DataType* data, KDNodeType* startNode = nullptr);
    KDTreeNode<KeyType, DataType>* findNodeInRightSubtreeWithDimension(KDNodeType* node, DataType* data, int target_dimension);
    void reinsertNodesWithSameKey(KDNodeType* node);

    void clearProcessedNodes();

    std::unordered_set<KDNodeType*> processedNodes;
};



template<typename KeyType, typename DataType>
inline GeneralKDTree<KeyType, DataType>::GeneralKDTree(size_t dim_count) {
    if (dim_count < 1) {
        throw invalid_argument("It has to be at least 1D Tree");
    }
    this->k = dim_count;
    this->size_ = 0;
    this->root = nullptr;
}

template<typename KeyType, typename DataType>
inline GeneralKDTree<KeyType, DataType>::~GeneralKDTree() {
    this->clear();
}

template<typename KeyType, typename DataType>
void GeneralKDTree<KeyType, DataType>::clear() {
    if (root == nullptr) return;

    std::stack<KDNodeType*> nodeStack;
    nodeStack.push(root);

    while (!nodeStack.empty()) {
        KDNodeType* current = nodeStack.top();
        nodeStack.pop();

        if (current->_left != nullptr) {
            nodeStack.push(current->_left);
        }

        if (current->_right != nullptr) {
            nodeStack.push(current->_right);
        }

        current->_data = nullptr;
        current->_keyPart = nullptr;
        delete current;
    }

    root = nullptr;
    size_ = 0;
}



template<typename KeyType, typename DataType>
DataType* GeneralKDTree<KeyType, DataType>::insert(DataType* data, KeyType* keys) {
    if (keys == nullptr) {
        throw invalid_argument("Keys cannot be nullptr");
    }
    if (size_ == 0) {
        this->root = new KDNodeType(data, keys, 0);
        this->size_++;
        return this->root->_data;
    }

    KDNodeType* current = this->root;
    KDNodeType* parent = nullptr;

    int level = 0;
    int current_dimension = 0;

    while (current != nullptr) {
        parent = current;
        if (keys->compare(*(current->_keyPart), current_dimension) <= 0) {
            if (current->_left == nullptr) break;
            current = current->_left;
        }
        else {
            if (current->_right == nullptr) break;
            current = current->_right;
        }

        level++;
        current_dimension = level % this->k;
    }

    if (keys->compare(*(parent->_keyPart), current_dimension) <= 0) {
        parent->_left = new KDNodeType(data, keys, level + 1);
        current = parent->_left;
    }
    else {
        parent->_right = new KDNodeType(data, keys, level + 1);
        current = parent->_right;
    }

    current->parent = parent;
    this->size_++;
    return current->_data;
}


template<typename KeyType, typename DataType>
vector<DataType*> GeneralKDTree<KeyType, DataType>::find(KeyType* keys) {
    vector<DataType*> duplicates;

    KDNodeType* current = this->root;
    int level = 0;

    while (current != nullptr) {
        if (keys->equalsByKeys(*(current->_keyPart))) {
            duplicates.push_back(current->_data);
        }

        int current_dimension = level % this->k;
        if (keys->compare(*(current->_keyPart), current_dimension) <= 0) {
            current = current->_left;
        }
        else {
            current = current->_right;
        }

        level++;
    }

    return duplicates;
}


template<typename KeyType, typename DataType>
inline bool GeneralKDTree<KeyType, DataType>::removeNode(DataType* data, KDNodeType* startNode, int targetDimension) {
    KDNodeType* node = nullptr;
    std::cout << "Attempting to remove node with data: " << *data << std::endl;
    std::list<KDNodeType*> nodesToReinsert;
    std::vector<KDNodeType*> path;
    std::vector<KDNodeType*> duplicities;
    std::vector<std::pair<KeyType*, DataType*>> duplicitiesData;

    if (startNode == nullptr && targetDimension == -1) {
        node = this->findNodeWithData(data);
    }
    else {
        node = this->findNodeInRightSubtreeWithDimension(startNode, data, targetDimension);
    }


    if (node == nullptr) {
        std::cout << RED << "Node not found in the tree." << RESET << std::endl;
        return false;
    }

    bool isLeafAfterComing = isLeaf(node);
    nodesToReinsert.push_back(node);

    while (!nodesToReinsert.empty()) {
        KDNodeType* curr = nodesToReinsert.back();
        nodesToReinsert.pop_back();
        isLeafAfterComing = isLeaf(curr);
        std::stack<KDNodeType*> nodesToReplace;
        KeyType* original_key = nullptr;
        DataType* original_data = nullptr;
        if (curr != node) {
            original_data = curr->_data;
            original_key = curr->_keyPart;
        }
        nodesToReplace.push(curr);
        path.clear();
        if (!isLeafAfterComing) {
            path.push_back(curr);
        }
        while (!nodesToReplace.empty()) {
            KDNodeType* currentNode = nodesToReplace.top();
            nodesToReplace.pop();

            std::cout << "Processing node with key: " << *(currentNode->_keyPart) << std::endl;
            if (isLeaf(currentNode)) {
                std::cout << "Node is a leaf, deleting it." << std::endl;
                KDNodeType* parent = currentNode->parent;
                if (parent != nullptr) {
                    if (isLeftSon(currentNode, parent)) {
                        parent->_left = nullptr;
                    }
                    else {
                        parent->_right = nullptr;
                    }
                }
                else {
                    this->root = nullptr;
                }
                currentNode->_keyPart = nullptr;
                currentNode->_data = nullptr;
                currentNode->parent = nullptr;
                currentNode->_left = nullptr;
                currentNode->_right = nullptr;
                delete currentNode;
                currentNode = nullptr;
                if (isLeafAfterComing) node = nullptr;
                this->size_--;
                continue;
            }

            if (currentNode->_left != nullptr && currentNode->_right != nullptr) {
                std::cout << "Node has two children, replacing it with min node from right subtree." << std::endl;
                KDNodeType* minNode = findMinInRightSubTree(currentNode);
                std::cout << "Replacing node with key " << *(currentNode->_keyPart)
                          << " with node having key " << *(minNode->_keyPart) << std::endl;
                currentNode->_keyPart = minNode->_keyPart;
                currentNode->_data = minNode->_data;
                nodesToReplace.push(minNode);
                path.push_back(minNode);
            }
            else if (currentNode->_right != nullptr) {
                std::cout << "Node has only right child, replacing it with min node from right subtree." << std::endl;
                KDNodeType* minNode = findMinInRightSubTree(currentNode);
                std::cout << "Replacing node with key " << *(currentNode->_keyPart)
                          << " with node having key " << *(minNode->_keyPart) << std::endl;
                currentNode->_keyPart = minNode->_keyPart;
                currentNode->_data = minNode->_data;
                nodesToReplace.push(minNode);
                path.push_back(minNode);


            }
            else if (currentNode->_left != nullptr) {
                std::cout << "Node has only left child, replacing it with max node from left subtree." << std::endl;
                KDNodeType* maxNode = findMaxInLeftSubTree(currentNode);
                std::cout << "Replacing node with key " << *(currentNode->_keyPart)
                          << " with node having key " << *(maxNode->_keyPart) << std::endl;
                currentNode->_keyPart = maxNode->_keyPart;
                currentNode->_data = maxNode->_data;
                nodesToReplace.push(maxNode);
                path.push_back(maxNode);
            }



        }
        if (!path.empty()) {
            path.pop_back();
        }




        for (KDNodeType* p : path)
        {
            duplicitiesData.clear();
            duplicities.clear();
            KeyType* target_key = p->_keyPart;
            int target_dimension = p->_level % this->k;
            if (p->_right && p) {
                std::stack<KDNodeType*> nodesToVisit;
                nodesToVisit.push(p->_right);
                while (!nodesToVisit.empty()) {
                    KDNodeType* currentNode = nodesToVisit.top();
                    nodesToVisit.pop();

                    if (currentNode->_keyPart && currentNode->_keyPart->compare(*target_key, target_dimension) <= 0) {
                        duplicities.push_back(currentNode);
                        duplicitiesData.push_back({ currentNode->_keyPart, currentNode->_data });
                    }

                    int current_dimension = currentNode->_level % this->k;

                    if (current_dimension != targetDimension) {
                        if (currentNode->_left != nullptr) {
                            nodesToVisit.push(currentNode->_left);
                        }
                        if (currentNode->_right != nullptr) {
                            nodesToVisit.push(currentNode->_right);
                        }
                    }
                    else {
                        if (currentNode != nullptr) {
                            nodesToVisit.push(currentNode->_left);
                        }

                    }

                }

            }

            for (KDNodeType* k : duplicities) {
                nodesToReinsert.push_back(k);

            }


        }
        if (original_data && original_key) {
            this->insert(original_data, original_key);

        }


    }


    std::cout << GREEN << "Node removal completed." << RESET << std::endl;
    return true;
}

template<typename KeyType, typename DataType>
inline bool GeneralKDTree<KeyType, DataType>::updateNode(DataType* oldData, KeyType* oldKeys, DataType* newData, KeyType* newKeys)
{
    KDNodeType* oldNode = findNodeWithData(oldData);

    if (oldNode == nullptr) return false;

    bool isKeyPartChanged = !oldData->equalsByKeys(*newData);

    if (isKeyPartChanged) {
        oldNode->_data = newData;
    }
    else {
        removeNode(oldData);
        insert(newData, newKeys);
    }
    return true;
}







template<typename KeyType, typename DataType>
inline KDTreeNode<KeyType, DataType>* GeneralKDTree<KeyType, DataType>::findNodeWithData(DataType* data, KDNodeType* startNode) {
    if (this->size_ == 0 || data == nullptr) return nullptr;

    KDNodeType* current = startNode ? startNode : this->root;

    while (current != nullptr) {
        if (current->_data->equals(*data)) {
            return current;
        }

        int curr_dim = current->_level % this->k;
        int comparison_result = data->compare(*(current->_data), curr_dim);

        if (comparison_result <= 0) {
            current = current->_left;
        }
        else {
            current = current->_right;
        }
    }

    return nullptr;
}


template<typename KeyType, typename DataType>
KDTreeNode<KeyType, DataType>* GeneralKDTree<KeyType, DataType>::findNodeInRightSubtreeWithDimension(KDNodeType* node, DataType* data, int target_dimension) {
    if (node == nullptr || node->_right == nullptr) {
        return nullptr;
    }

    std::stack<KDNodeType*> nodesToVisit;
    nodesToVisit.push(node->_right);

    while (!nodesToVisit.empty()) {
        KDNodeType* currentNode = nodesToVisit.top();
        nodesToVisit.pop();

        if (currentNode->_data->equals(*data)) {
            return currentNode;
        }

        int current_dimension = currentNode->_level % this->k;

        if (current_dimension != target_dimension) {
            if (currentNode->_left != nullptr) {
                nodesToVisit.push(currentNode->_left);
            }
            if (currentNode->_right != nullptr) {
                nodesToVisit.push(currentNode->_right);
            }
        }
        else {
            if (currentNode->_left != nullptr) {
                nodesToVisit.push(currentNode->_left);
            }

        }
    }

    return nullptr;
}





template<typename KeyType, typename DataType>
inline KDTreeNode<KeyType, DataType>* GeneralKDTree<KeyType, DataType>::findMaxInLeftSubTree(KDNodeType* parent) {
    if (parent == nullptr || parent->_left == nullptr) {
        std::cout << "Left subtree is empty; no max node found." << std::endl;
        return nullptr;
    }

    KDNodeType* current = parent->_left;
    KDNodeType* maxNode = current;
    int target_dimension = parent->_level % this->k;

    std::stack<KDNodeType*> nodesToVisit;
    nodesToVisit.push(current);

    std::cout << "Starting search for max in left subtree. Initial node key: " << *(current->_keyPart)
              << ", dimension to compare: " << target_dimension << std::endl;

    while (!nodesToVisit.empty()) {
        KDNodeType* node = nodesToVisit.top();
        nodesToVisit.pop();

        std::cout << "Visiting node with key: " << *(node->_keyPart)
                  << ", level: " << node->_level
                  << ", current dimension: " << (node->_level % this->k) << std::endl;


        if (node->_keyPart->compare(*(maxNode->_keyPart), target_dimension) >= 0) {
            std::cout << "Updating max node to key: " << *(node->_keyPart) << std::endl;
            maxNode = node;
        }


        int current_dimension = node->_level % this->k;
        if (current_dimension != target_dimension) {

            if (node->_left != nullptr) {
                std::cout << "Pushing left child with key: " << *(node->_left->_keyPart) << " onto stack." << std::endl;
                nodesToVisit.push(node->_left);
            }
            if (node->_right != nullptr) {
                std::cout << "Pushing right child with key: " << *(node->_right->_keyPart) << " onto stack." << std::endl;
                nodesToVisit.push(node->_right);
            }
        }
        else {

            if (node->_right != nullptr) {
                std::cout << "Pushing right child with key: " << *(node->_right->_keyPart) << " onto stack (target dimension)." << std::endl;
                nodesToVisit.push(node->_right);
            }
        }
    }

    std::cout << "Max node in left subtree found with key: " << *(maxNode->_keyPart) << std::endl;
    return maxNode;
}




template<typename KeyType, typename DataType>
inline KDTreeNode<KeyType, DataType>* GeneralKDTree<KeyType, DataType>::findMinInRightSubTree(KDNodeType* parent) {
    if (parent == nullptr || parent->_right == nullptr) {
        std::cout << "Right subtree is empty; no min node found." << std::endl;
        return nullptr;
    }

    KDNodeType* current = parent->_right;
    KDNodeType* minNode = current;
    int target_dimension = parent->_level % this->k;

    std::stack<KDNodeType*> nodesToVisit;
    nodesToVisit.push(current);

    std::cout << "Starting search for min in right subtree. Initial node key: " << *(current->_keyPart)
              << ", dimension to compare: " << target_dimension << std::endl;

    while (!nodesToVisit.empty()) {
        KDNodeType* node = nodesToVisit.top();
        nodesToVisit.pop();

        std::cout << "Visiting node with key: " << *(node->_keyPart)
                  << ", level: " << node->_level
                  << ", current dimension: " << (node->_level % this->k) << std::endl;


        if (node->_keyPart->compare(*(minNode->_keyPart), target_dimension) <= 0) {
            std::cout << "Updating min node to key: " << *(node->_keyPart) << std::endl;
            minNode = node;
        }
        int current_dimension = node->_level % this->k;
        if (current_dimension != target_dimension) {


            if (node->_left != nullptr) {
                std::cout << "Pushing left child with key: " << *(node->_left->_keyPart) << " onto stack." << std::endl;
                nodesToVisit.push(node->_left);
            }
            if (node->_right != nullptr) {
                std::cout << "Pushing right child with key: " << *(node->_right->_keyPart) << " onto stack." << std::endl;
                nodesToVisit.push(node->_right);
            }
        }
        else {

            if (node->_left != nullptr) {
                std::cout << "Pushing left child with key: " << *(node->_left->_keyPart) << " onto stack (target dimension)." << std::endl;
                nodesToVisit.push(node->_left);
            }
        }
    }

    std::cout << GREEN << "Min node in right subtree found with key: " << *(minNode->_keyPart) << RESET << std::endl;
    return minNode;
}


template<typename KeyType, typename DataType>
inline void GeneralKDTree<KeyType, DataType>::clearProcessedNodes()
{
    processedNodes.clear();
}


template<typename KeyType, typename DataType>
inline bool GeneralKDTree<KeyType, DataType>::hasLeftSon(KDNodeType* node) {
    if (node == nullptr) {
        return false;
    }
    return node->_left != nullptr;
}

template<typename KeyType, typename DataType>
inline bool GeneralKDTree<KeyType, DataType>::hasRightSon(KDNodeType* node) {
    if (node == nullptr) {
        return false;
    }
    return node->_right != nullptr;
}

template<typename KeyType, typename DataType>
inline bool GeneralKDTree<KeyType, DataType>::isLeaf(KDNodeType* node) {
    if (node == nullptr) {
        return false;
    }

    return node->_right == nullptr && node->_left == nullptr;
}

template<typename KeyType, typename DataType>
inline bool GeneralKDTree<KeyType, DataType>::isLeftSon(KDNodeType* node, KDNodeType* parent) {
    return hasLeftSon(parent) && parent->_left == node;
}

template<typename KeyType, typename DataType>
inline bool GeneralKDTree<KeyType, DataType>::isRightSon(KDNodeType* node, KDNodeType* parent) {
    return hasRightSon(parent) && parent->_right == node;
}

template<typename KeyType, typename DataType>
size_t GeneralKDTree<KeyType, DataType>::size() const {
    return size_;
}

template<typename KeyType, typename DataType>
inline KDTreeNode<KeyType, DataType>* GeneralKDTree<KeyType, DataType>::accessRoot() {
    if (this->root == nullptr) {
        throw out_of_range("Structure is empty");
    }
    return this->root;
}

template<typename KeyType, typename DataType>
inline void GeneralKDTree<KeyType, DataType>::inOrderTraversal(std::function<void(KDNodeType*)> func, KDNodeType* startNode) {
    if (startNode == nullptr) {
        startNode = root;
    }

    if (startNode == nullptr) return;

    std::stack<KDNodeType*> nodeStack;
    KDNodeType* current = startNode;

    while (!nodeStack.empty() || current != nullptr) {
        while (current != nullptr) {
            nodeStack.push(current);
            current = current->_left;
        }

        current = nodeStack.top();
        nodeStack.pop();

        func(current);

        current = current->_right;
    }
}

template<typename KeyType, typename DataType>
void GeneralKDTree<KeyType, DataType>::levelOrderTraversal(std::function<void(KDNodeType*)> func) {
    if (this->root == nullptr) return;

    std::queue<KDNodeType*> nodeQueue;
    nodeQueue.push(this->root);

    while (!nodeQueue.empty()) {
        KDNodeType* current = nodeQueue.front();
        nodeQueue.pop();

        func(current);

        if (current->_left != nullptr) {
            nodeQueue.push(current->_left);
        }
        if (current->_right != nullptr) {
            nodeQueue.push(current->_right);
        }
    }
}

template<typename KeyType, typename DataType>
void GeneralKDTree<KeyType, DataType>::reverseLevelOrderTraversal(std::function<void(KDNodeType*)> func) {
    if (this->root == nullptr) return;

    std::queue<KDNodeType*> nodeQueue;
    std::stack<KDNodeType*> nodeStack;
    nodeQueue.push(this->root);

    while (!nodeQueue.empty()) {
        KDNodeType* current = nodeQueue.front();
        nodeQueue.pop();

        nodeStack.push(current);

        if (current->_right != nullptr) {
            nodeQueue.push(current->_right);
        }
        if (current->_left != nullptr) {
            nodeQueue.push(current->_left);
        }
    }


    while (!nodeStack.empty()) {
        KDNodeType* current = nodeStack.top();
        nodeStack.pop();
        func(current);
    }
}

