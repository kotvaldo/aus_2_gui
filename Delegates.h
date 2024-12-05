#pragma once

#include "Node.h"
#include "Operation.h"

#include <memory>
#include <stack>
#include <queue>

using namespace std;


template <typename KeyType, typename DataType>
class TreeTraversal {
private:
    TreeTraversal() = default;

public:

    TreeTraversal(const TreeTraversal&) = delete;
    TreeTraversal& operator=(const TreeTraversal&) = delete;

    static std::shared_ptr<TreeTraversal<KeyType, DataType>> GetInstance() {
        static std::shared_ptr<TreeTraversal<KeyType, DataType>> instance =
            std::shared_ptr<TreeTraversal<KeyType, DataType>>(new TreeTraversal());
        return instance;
    }

    std::shared_ptr<KDTreeNode<KeyType, DataType>> findNodeWithData(
        std::shared_ptr<DataType> data,
        std::shared_ptr<KDTreeNode<KeyType, DataType>> startNode, size_t k)
    {
        if (!startNode || !data) {
            return nullptr;
        }

        auto current = startNode;

        while (current) {
            if (current->_data->equals(*data)) {
                return current;
            }

            int curr_dim = current->_level % k;
            int comparison_result = data->compare(*(current->_data), curr_dim);

            if (comparison_result <= 0) {
                current = current->_left;
            } else {
                current = current->_right;
            }
        }

        return nullptr;
    }

    std::shared_ptr<KDTreeNode<KeyType, DataType>>
    findNodeInRightSubtreeWithDimension(
        std::shared_ptr<KDTreeNode<KeyType, DataType>> node,
        std::shared_ptr<DataType> data,
        int target_dimension, size_t k)
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

            int current_dimension = currentNode->_level % k;

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

    std::shared_ptr<KDTreeNode<KeyType, DataType>> findMaxInLeftSubTree(std::shared_ptr<KDTreeNode<KeyType, DataType>> parent, int k) {
        if (!parent || !parent->_left) {
            return nullptr;
        }

        auto current = parent->_left;
        auto maxNode = current;
        int target_dimension = parent->_level % k;

        std::stack<std::shared_ptr<KDTreeNode<KeyType, DataType>>> nodesToVisit;
        nodesToVisit.push(current);

        while (!nodesToVisit.empty()) {
            auto node = nodesToVisit.top();
            nodesToVisit.pop();

            if (node->_keyPart->compare(*(maxNode->_keyPart), target_dimension) >= 0) {
                maxNode = node;
            }

            int current_dimension = node->_level % k;
            if (current_dimension != target_dimension) {
                if (node->_left) {
                    nodesToVisit.push(node->_left);
                }
                if (node->_right) {
                    nodesToVisit.push(node->_right);
                }
            } else {
                if (node->_right) {
                    nodesToVisit.push(node->_right);
                }
            }
        }

        return maxNode;
    }

    std::shared_ptr<KDTreeNode<KeyType, DataType>> findMinInRightSubTree(std::shared_ptr<KDTreeNode<KeyType, DataType>> parent, int k) {
        if (!parent || !parent->_right) {
            return nullptr;
        }

        auto current = parent->_right;
        auto minNode = current;
        int target_dimension = parent->_level % k;

        std::stack<std::shared_ptr<KDTreeNode<KeyType, DataType>>> nodesToVisit;
        nodesToVisit.push(current);

        while (!nodesToVisit.empty()) {
            auto node = nodesToVisit.top();
            nodesToVisit.pop();

            if (node->_keyPart->compare(*(minNode->_keyPart), target_dimension) <= 0) {
                minNode = node;
            }

            int current_dimension = node->_level % k;
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

        return minNode;
    }

    void inOrderTraversal(
        std::shared_ptr<INodeOperation<KeyType, DataType>> operation,
        std::shared_ptr<KDTreeNode<KeyType, DataType>> startNode)
    {
        if (!startNode) {
            return;
        }

        if (!operation) {
            operation = NullOperation<KeyType, DataType>::getInstance();
        }

        std::stack<std::shared_ptr<KDTreeNode<KeyType, DataType>>> stack;
        auto current = startNode;

        while (!stack.empty() || current) {
            while (current) {
                stack.push(current);
                current = current->_left;
            }

            current = stack.top();
            stack.pop();
            operation->execute(current);

            current = current->_right;
        }
    }


    void levelOrderTraversal(
        std::shared_ptr<INodeOperation<KeyType, DataType>> operation,
        std::shared_ptr<KDTreeNode<KeyType, DataType>> startNode)
    {
        if (!startNode)
            return;

        if (!operation) {
            operation = NullOperation<KeyType, DataType>::getInstance();
        }

        std::queue<std::shared_ptr<KDTreeNode<KeyType, DataType>>> nodeQueue;
        nodeQueue.push(startNode);

        while (!nodeQueue.empty()) {
            auto current = nodeQueue.front();
            nodeQueue.pop();

            operation->execute(current);

            if (current->_left) {
                nodeQueue.push(current->_left);
            }
            if (current->_right) {
                nodeQueue.push(current->_right);
            }
        }
    }

    void reverseLevelOrderTraversal(
        std::shared_ptr<INodeOperation<KeyType, DataType>> operation,
        std::shared_ptr<KDTreeNode<KeyType, DataType>> root)
    {
        if (!root)
            return;
        if (!operation) {
            operation = NullOperation<KeyType, DataType>::getInstance();
        }

        std::queue<std::shared_ptr<KDTreeNode<KeyType, DataType>>> nodeQueue;
        std::stack<std::shared_ptr<KDTreeNode<KeyType, DataType>>> nodeStack;
        nodeQueue.push(root);

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
            operation->execute(current);
        }
    }

};

