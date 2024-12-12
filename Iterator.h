#pragma once

#include "Node.h"

#include <queue>
#include <stack>

template<typename KeyType, typename DataType>
class BaseIterator {
protected:
    using KDNodeType = KDTreeNode<KeyType, DataType>;
    KDNodeType* current;

public:
    virtual ~BaseIterator() = default;

    virtual void operator++() = 0;
    virtual bool operator!=(const BaseIterator& other) const = 0;

    virtual DataType* operator*() const {
        return current ? current->_data : nullptr;
    }
};



template<typename KeyType, typename DataType>
class InOrderIterator : public BaseIterator<KeyType, DataType> {
    using KDNodeType = KDTreeNode<KeyType, DataType>;

public:
    InOrderIterator(KDNodeType* root) { initialize(root); }

    void operator++() override {
        if (!nodeStack.empty()) {
            this->current = nodeStack.top();
            nodeStack.pop();

            KDNodeType* temp = this->current->_right;
            while (temp != nullptr) {
                nodeStack.push(temp);
                temp = temp->_left;
            }
        } else {
            this->current = nullptr;
        }
    }

    bool operator!=(const BaseIterator<KeyType, DataType>& other) const override {
        return this->current != static_cast<const InOrderIterator&>(other).current;
    }

private:
    std::stack<KDNodeType*> nodeStack;

    void initialize(KDNodeType* root) {
        while (root != nullptr) {
            nodeStack.push(root);
            root = root->_left;
        }
        operator++();
    }
};


template<typename KeyType, typename DataType>
class LevelOrderIterator : public BaseIterator<KeyType, DataType> {
    using KDNodeType = KDTreeNode<KeyType, DataType>;

public:
    LevelOrderIterator(KDNodeType* root) {
        if (root) {
            nodeQueue.push(root);
            operator++(); // Inicializácia na prvý prvok
        }
    }

    void operator++() override {
        if (!nodeQueue.empty()) {
            this->current = nodeQueue.front();
            nodeQueue.pop();

            if (this->current->_left) {
                nodeQueue.push(this->current->_left);
            }
            if (this->current->_right) {
                nodeQueue.push(this->current->_right);
            }
        } else {
            this->current = nullptr;
        }
    }

    bool operator!=(const BaseIterator<KeyType, DataType>& other) const override {
        return this->current != static_cast<const LevelOrderIterator&>(other).current;
    }

private:
    std::queue<KDNodeType*> nodeQueue;
};
