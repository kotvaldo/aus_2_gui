#pragma once
#include <functional>
#include <iostream>
#include <list>
#include <queue>
#include <stack>
#include <vector>
#include <memory>
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define UNKNOWN "\033[33m"

using namespace std;

template<typename KeyType, typename DataType>
struct KDTreeNode
{
    std::shared_ptr<KDTreeNode> parent;
    std::shared_ptr<KDTreeNode> _left;
    std::shared_ptr<KDTreeNode> _right;
    int _level;
    std::shared_ptr<DataType> _data;
    std::shared_ptr<KeyType> _keyPart;

    KDTreeNode(std::shared_ptr<DataType> data, std::shared_ptr<KeyType> keys, int level = 0)
        : _data(std::move(data))
        , _keyPart(std::move(keys))
        , _level(level)
        , parent(nullptr)
        , _left(nullptr)
        , _right(nullptr)
    {}
};

template<typename KeyType, typename DataType>
class GeneralKDTree
{
    using KDNodeType = std::shared_ptr<KDTreeNode<KeyType, DataType>>;

public:
    GeneralKDTree(size_t dim_count);
    ~GeneralKDTree() = default;

    void clear();
    std::vector<std::shared_ptr<DataType>> find(std::shared_ptr<KeyType> keys);
    std::shared_ptr<DataType> insert(std::shared_ptr<DataType> data);
    bool removeNode(std::shared_ptr<DataType> data, KDNodeType startNode = nullptr, int targetDimension = -1);
    bool updateNode(std::shared_ptr<DataType> oldData,
                    std::shared_ptr<DataType> newData);
    size_t size() const;
    KDNodeType accessRoot();
    std::shared_ptr<KDTreeNode<KeyType, DataType>> findMaxInLeftSubTree(std::shared_ptr<KDTreeNode<KeyType, DataType>> parent);
    std::shared_ptr<KDTreeNode<KeyType, DataType>> findMinInRightSubTree(std::shared_ptr<KDTreeNode<KeyType, DataType>> parent);
    void inOrderTraversal(std::function<void(KDNodeType)> func, KDNodeType startNode = nullptr);
    void levelOrderTraversal(std::function<void(KDNodeType)> func);
    void reverseLevelOrderTraversal(std::function<void(KDNodeType)> func);

    bool hasLeftSon(std::shared_ptr<KDTreeNode<KeyType, DataType>> node);
    bool hasRightSon(std::shared_ptr<KDTreeNode<KeyType, DataType>> node);
    bool isLeaf(std::shared_ptr<KDTreeNode<KeyType, DataType>> node);
    bool isRoot(std::shared_ptr<KDTreeNode<KeyType, DataType>> node);
    bool isLeftSon(std::shared_ptr<KDTreeNode<KeyType, DataType>> node, std::shared_ptr<KDTreeNode<KeyType, DataType>> parent);
    bool isRightSon(std::shared_ptr<KDTreeNode<KeyType, DataType>> node, std::shared_ptr<KDTreeNode<KeyType, DataType>> parent);
    // Hľadanie v strome
    KDNodeType findNodeWithData(std::shared_ptr<DataType> data, KDNodeType startNode = nullptr);
    KDNodeType findNodeInRightSubtreeWithDimension(KDNodeType node, std::shared_ptr<DataType> data, int target_dimension);

private:
    size_t size_;
    KDNodeType root;
    size_t k;
};


template<typename KeyType, typename DataType>
inline GeneralKDTree<KeyType, DataType>::GeneralKDTree(size_t dim_count)
{
    if (dim_count < 1) {
        throw std::invalid_argument("It has to be at least 1D Tree");
    }
    this->k = dim_count;
    this->size_ = 0;
    this->root = nullptr;
}




template<typename KeyType, typename DataType>
void GeneralKDTree<KeyType, DataType>::clear()
{
    root = nullptr;
    size_ = 0;
}



template<typename KeyType, typename DataType>
std::shared_ptr<DataType> GeneralKDTree<KeyType,DataType>::insert(
    std::shared_ptr<DataType> data) {
    if (!data) {
        throw std::invalid_argument("Data cannot be nullptr");
    }

    if (size_ == 0) {
        this->root = std::make_shared<KDTreeNode<KeyType,DataType>>(data, 0);
        this->size_++;
        return this->root->_data;
    }

    KDNodeType current = this->root;
    KDNodeType parent = nullptr;

    int level = 0;
    int current_dimension = 0;

    while (current != nullptr) {
        parent = current;

        if (data->compare(*(current->_data), current_dimension) <= 0) {
            if (current->_left == nullptr)
                break;
            current = current->_left;
        } else {
            if (current->_right == nullptr)
                break;
            current = current->_right;
        }

        level++;
        current_dimension = level % this->k;
    }

    KDNodeType newNode = std::make_shared<KDTreeNode<KeyType,DataType>>(data, level + 1);

    if (data->compare(*(parent->_data), current_dimension) <= 0) {
        parent->_left = newNode;
    } else {
        parent->_right = newNode;
    }

    newNode->parent = parent;
    this->size_++;
    return newNode->_data;
}



template<typename KeyType, typename DataType>
std::vector<std::shared_ptr<DataType>> GeneralKDTree<KeyType, DataType>::find(std::shared_ptr<KeyType> keys)
{
    std::vector<std::shared_ptr<DataType>> duplicates;

    KDNodeType current = this->root;
    int level = 0;

    while (current != nullptr) {
        if (keys->equalsByKeys(*(current->_keyPart))) {
            duplicates.push_back(current->_data);
        }

        int current_dimension = level % this->k;
        if (keys->compare(*(current->_keyPart), current_dimension) <= 0) {
            current = current->_left;
        } else {
            current = current->_right;
        }

        level++;
    }

    return duplicates;
}


template<typename KeyType, typename DataType>
bool GeneralKDTree<KeyType, DataType>::removeNode(
    std::shared_ptr<DataType> data,
    KDNodeType startNode,
    int targetDimension)
{
    KDNodeType node = nullptr;
    std::cout << "Attempting to remove node with data: " << *data << std::endl;
    std::list<KDNodeType> nodesToReinsert;
    std::vector<KDNodeType> path;

    if (!startNode && targetDimension == -1) {
        node = this->findNodeWithData(data);
    } else {
        node = this->findNodeInRightSubtreeWithDimension(startNode, data, targetDimension);
    }

    if (!node) {
        std::cout << RED << "Node not found in the tree." << RESET << std::endl;
        return false;
    }

    bool isLeafAfterComing = isLeaf(node);
    nodesToReinsert.push_back(node);

    while (!nodesToReinsert.empty()) {
        KDNodeType curr = nodesToReinsert.back();
        nodesToReinsert.pop_back();
        isLeafAfterComing = isLeaf(curr);
        std::stack<KDNodeType> nodesToReplace;
        std::shared_ptr<KeyType> original_key = nullptr;
        std::shared_ptr<DataType> original_data = nullptr;

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
            KDNodeType currentNode = nodesToReplace.top();
            nodesToReplace.pop();

            std::cout << "Processing node with key: " << *(currentNode->_keyPart) << std::endl;

            if (isLeaf(currentNode)) {
                std::cout << "Node is a leaf, deleting it." << std::endl;
                KDNodeType parent = currentNode->parent;
                if (parent) {
                    if (isLeftSon(currentNode, parent)) {
                        parent->_left = nullptr;
                    } else {
                        parent->_right = nullptr;
                    }
                } else {
                    this->root = nullptr;
                }
                currentNode.reset(); // Automatické uvoľnenie pamäte
                if (isLeafAfterComing)
                    node = nullptr;
                this->size_--;
                continue;
            }

            if (currentNode->_left && currentNode->_right) {
                std::cout << "Node has two children, replacing it with min node from right subtree."
                          << std::endl;
                KDNodeType minNode = findMinInRightSubTree(currentNode);
                std::cout << "Replacing node with key " << *(currentNode->_keyPart)
                          << " with node having key " << *(minNode->_keyPart) << std::endl;
                currentNode->_keyPart = minNode->_keyPart;
                currentNode->_data = minNode->_data;
                nodesToReplace.push(minNode);
                path.push_back(minNode);
            } else if (currentNode->_right) {
                std::cout << "Node has only right child, replacing it with min node from right subtree."
                          << std::endl;
                KDNodeType minNode = findMinInRightSubTree(currentNode);
                currentNode->_keyPart = minNode->_keyPart;
                currentNode->_data = minNode->_data;
                nodesToReplace.push(minNode);
                path.push_back(minNode);
            } else if (currentNode->_left) {
                std::cout << "Node has only left child, replacing it with max node from left subtree."
                          << std::endl;
                KDNodeType maxNode = findMaxInLeftSubTree(currentNode);
                currentNode->_keyPart = maxNode->_keyPart;
                currentNode->_data = maxNode->_data;
                nodesToReplace.push(maxNode);
                path.push_back(maxNode);
            }
        }

        if (!path.empty()) {
            path.pop_back();
        }

        for (auto &p : path) {
            KeyType *target_key = p->_keyPart.get();
            int target_dimension = p->_level % this->k;
            if (p->_right) {
                std::stack<KDNodeType> nodesToVisit;
                nodesToVisit.push(p->_right);

                while (!nodesToVisit.empty()) {
                    KDNodeType currentNode = nodesToVisit.top();
                    nodesToVisit.pop();

                    if (currentNode->_keyPart &&
                        currentNode->_keyPart->compare(*target_key, target_dimension) <= 0) {
                        nodesToReinsert.push_back(currentNode);
                    }

                    int current_dimension = currentNode->_level % this->k;

                    if (current_dimension != targetDimension) {
                        if (currentNode->_left) {
                            nodesToVisit.push(currentNode->_left);
                        }
                        if (currentNode->_right) {
                            nodesToVisit.push(currentNode->_right);
                        }
                    } else {
                        if (currentNode->_left) {
                            nodesToVisit.push(currentNode->_left);
                        }
                    }
                }
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
bool GeneralKDTree<KeyType, DataType>::updateNode(
    std::shared_ptr<DataType> oldData,
    std::shared_ptr<DataType> newData)
{
    auto oldNode = findNodeWithData(oldData);

    if (!oldNode) {
        return false;
    }

    bool isKeyPartChanged = !oldData->equalsByKeys(*newData);

    if (!isKeyPartChanged) {
        oldNode->_data = newData;
    } else {
        removeNode(oldData);
        insert(newData);
    }

    return true;
}

template<typename KeyType, typename DataType>
std::shared_ptr<KDTreeNode<KeyType, DataType>> GeneralKDTree<KeyType, DataType>::findNodeWithData(
    std::shared_ptr<DataType> data,
    std::shared_ptr<KDTreeNode<KeyType, DataType>> startNode)
{
    if (this->size_ == 0 || !data) {
        return nullptr;
    }

    auto current = startNode ? startNode : this->root;

    while (current) {
        if (current->_data->equals(*data)) {
            return current;
        }

        int curr_dim = current->_level % this->k;
        int comparison_result = data->compare(*(current->_data), curr_dim);

        if (comparison_result <= 0) {
            current = current->_left;
        } else {
            current = current->_right;
        }
    }

    return nullptr;
}

template<typename KeyType, typename DataType>
std::shared_ptr<KDTreeNode<KeyType, DataType>>
GeneralKDTree<KeyType, DataType>::findNodeInRightSubtreeWithDimension(
    std::shared_ptr<KDTreeNode<KeyType, DataType>> node,
    std::shared_ptr<DataType> data,
    int target_dimension)
{
    if (!node || !node->_right) {
        return nullptr;
    }

    std::stack<std::shared_ptr<KDTreeNode<KeyType, DataType>>> nodesToVisit;
    nodesToVisit.push(node->_right);

    while (!nodesToVisit.empty()) {
        auto currentNode = nodesToVisit.top();
        nodesToVisit.pop();

        if (currentNode->_data->equals(*data)) {
            return currentNode;
        }

        int current_dimension = currentNode->_level % this->k;

        if (current_dimension != target_dimension) {
            if (currentNode->_left) {
                nodesToVisit.push(currentNode->_left);
            }
            if (currentNode->_right) {
                nodesToVisit.push(currentNode->_right);
            }
        } else {
            if (currentNode->_left) {
                nodesToVisit.push(currentNode->_left);
            }
        }
    }

    return nullptr;
}


template<typename KeyType, typename DataType>
std::shared_ptr<KDTreeNode<KeyType, DataType>> GeneralKDTree<KeyType, DataType>::findMaxInLeftSubTree(
    std::shared_ptr<KDTreeNode<KeyType, DataType>> parent)
{
    if (!parent || !parent->_left) {
        std::cout << "Left subtree is empty; no max node found." << std::endl;
        return nullptr;
    }

    auto current = parent->_left;
    auto maxNode = current;
    int target_dimension = parent->_level % this->k;

    std::stack<std::shared_ptr<KDTreeNode<KeyType, DataType>>> nodesToVisit;
    nodesToVisit.push(current);

    std::cout << "Starting search for max in left subtree. Initial node key: "
              << *(current->_keyPart) << ", dimension to compare: " << target_dimension
              << std::endl;

    while (!nodesToVisit.empty()) {
        auto node = nodesToVisit.top();
        nodesToVisit.pop();

        std::cout << "Visiting node with key: " << *(node->_keyPart) << ", level: " << node->_level
                  << ", current dimension: " << (node->_level % this->k) << std::endl;

        if (node->_keyPart->compare(*(maxNode->_keyPart), target_dimension) >= 0) {
            std::cout << "Updating max node to key: " << *(node->_keyPart) << std::endl;
            maxNode = node;
        }

        int current_dimension = node->_level % this->k;
        if (current_dimension != target_dimension) {
            if (node->_left) {
                std::cout << "Pushing left child with key: " << *(node->_left->_keyPart)
                << " onto stack." << std::endl;
                nodesToVisit.push(node->_left);
            }
            if (node->_right) {
                std::cout << "Pushing right child with key: " << *(node->_right->_keyPart)
                << " onto stack." << std::endl;
                nodesToVisit.push(node->_right);
            }
        } else {
            if (node->_right) {
                std::cout << "Pushing right child with key: " << *(node->_right->_keyPart)
                << " onto stack (target dimension)." << std::endl;
                nodesToVisit.push(node->_right);
            }
        }
    }

    std::cout << "Max node in left subtree found with key: " << *(maxNode->_keyPart) << std::endl;
    return maxNode;
}


template<typename KeyType, typename DataType>
std::shared_ptr<KDTreeNode<KeyType, DataType>> GeneralKDTree<KeyType, DataType>::findMinInRightSubTree(
    std::shared_ptr<KDTreeNode<KeyType, DataType>> parent)
{
    if (!parent || !parent->_right) {
        std::cout << "Right subtree is empty; no min node found." << std::endl;
        return nullptr;
    }

    auto current = parent->_right;
    auto minNode = current;
    int target_dimension = parent->_level % this->k;

    std::stack<std::shared_ptr<KDTreeNode<KeyType, DataType>>> nodesToVisit;
    nodesToVisit.push(current);

    std::cout << "Starting search for min in right subtree. Initial node key: "
              << *(current->_keyPart) << ", dimension to compare: " << target_dimension
              << std::endl;

    while (!nodesToVisit.empty()) {
        auto node = nodesToVisit.top();
        nodesToVisit.pop();

        std::cout << "Visiting node with key: " << *(node->_keyPart) << ", level: " << node->_level
                  << ", current dimension: " << (node->_level % this->k) << std::endl;

        if (node->_keyPart->compare(*(minNode->_keyPart), target_dimension) <= 0) {
            std::cout << "Updating min node to key: " << *(node->_keyPart) << std::endl;
            minNode = node;
        }
        int current_dimension = node->_level % this->k;
        if (current_dimension != target_dimension) {
            if (node->_left) {
                nodesToVisit.push(node->_left);
            }
            if (node->_right) {
                nodesToVisit.push(node->_right);
            }
        } else {
            if (node->_left) {
                nodesToVisit.push(node->_left);
            }
        }
    }

    std::cout << GREEN << "Min node in right subtree found with key: " << *(minNode->_keyPart)
              << RESET << std::endl;
    return minNode;
}

template<typename KeyType, typename DataType>
inline bool GeneralKDTree<KeyType, DataType>::hasLeftSon(std::shared_ptr<KDTreeNode<KeyType, DataType>> node)
{
    return node && node->_left != nullptr;
}


template<typename KeyType, typename DataType>
inline bool GeneralKDTree<KeyType, DataType>::hasRightSon(std::shared_ptr<KDTreeNode<KeyType, DataType>> node)
{
    return node && node->_right != nullptr;
}


template<typename KeyType, typename DataType>
inline bool GeneralKDTree<KeyType, DataType>::isLeaf(std::shared_ptr<KDTreeNode<KeyType, DataType>> node)
{
    return node && !node->_left && !node->_right;
}

template<typename KeyType, typename DataType>
inline bool GeneralKDTree<KeyType, DataType>::isLeftSon(
    std::shared_ptr<KDTreeNode<KeyType, DataType>> node,
    std::shared_ptr<KDTreeNode<KeyType, DataType>> parent)
{
    return parent && parent->_left == node;
}


template<typename KeyType, typename DataType>
inline bool GeneralKDTree<KeyType, DataType>::isRightSon(
    std::shared_ptr<KDTreeNode<KeyType, DataType>> node,
    std::shared_ptr<KDTreeNode<KeyType, DataType>> parent)
{
    return parent && parent->_right == node;
}


template<typename KeyType, typename DataType>
size_t GeneralKDTree<KeyType, DataType>::size() const
{
    return size_;
}

template<typename KeyType, typename DataType>
std::shared_ptr<KDTreeNode<KeyType, DataType>> GeneralKDTree<KeyType, DataType>::accessRoot()
{
    if (!this->root) {
        throw std::out_of_range("Structure is empty");
    }
    return this->root;
}


template<typename KeyType, typename DataType>
void GeneralKDTree<KeyType, DataType>::inOrderTraversal(
    std::function<void(std::shared_ptr<KDTreeNode<KeyType, DataType>>)> func,
    std::shared_ptr<KDTreeNode<KeyType, DataType>> startNode)
{
    if (!startNode) {
        startNode = root;
    }

    if (!startNode)
        return;

    inOrderTraversal(func, startNode->_left);
    func(startNode);
    inOrderTraversal(func, startNode->_right);
}


template<typename KeyType, typename DataType>
void GeneralKDTree<KeyType, DataType>::levelOrderTraversal(
    std::function<void(std::shared_ptr<KDTreeNode<KeyType, DataType>>)> func)
{
    if (!this->root)
        return;

    std::queue<std::shared_ptr<KDTreeNode<KeyType, DataType>>> nodeQueue;
    nodeQueue.push(this->root);

    while (!nodeQueue.empty()) {
        auto current = nodeQueue.front();
        nodeQueue.pop();

        func(current);

        if (current->_left) {
            nodeQueue.push(current->_left);
        }
        if (current->_right) {
            nodeQueue.push(current->_right);
        }
    }
}

template<typename KeyType, typename DataType>
void GeneralKDTree<KeyType, DataType>::reverseLevelOrderTraversal(
    std::function<void(std::shared_ptr<KDTreeNode<KeyType, DataType>>)> func)
{
    if (!this->root)
        return;

    std::queue<std::shared_ptr<KDTreeNode<KeyType, DataType>>> nodeQueue;
    std::stack<std::shared_ptr<KDTreeNode<KeyType, DataType>>> nodeStack;
    nodeQueue.push(this->root);

    while (!nodeQueue.empty()) {
        auto current = nodeQueue.front();
        nodeQueue.pop();

        nodeStack.push(current);

        if (current->_right) {
            nodeQueue.push(current->_right);
        }
        if (current->_left) {
            nodeQueue.push(current->_left);
        }
    }

    while (!nodeStack.empty()) {
        auto current = nodeStack.top();
        nodeStack.pop();
        func(current);
    }
}

