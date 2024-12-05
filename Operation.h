#pragma once
#include "Node.h"
#include <vector>

template <typename KeyType, typename DataType>
class INodeOperation {
public:
    virtual void execute(std::shared_ptr<KDTreeNode<KeyType, DataType>> node) = 0;

};

template <typename KeyType, typename DataType>
class NullOperation : public INodeOperation<KeyType, DataType> {
public:

    static std::shared_ptr<NullOperation<KeyType, DataType>> getInstance() {
        static std::shared_ptr<NullOperation<KeyType, DataType>> instance =
            std::make_shared<NullOperation<KeyType, DataType>>();
        return instance;
    }

    void execute(std::shared_ptr<KDTreeNode<KeyType, DataType>> node) override {

    }

private:
    NullOperation() = default;
    NullOperation(const NullOperation&) = delete;
    NullOperation& operator=(const NullOperation&) = delete;
};



template <typename KeyType, typename DataType>
class CollectAllOperation : public INodeOperation<KeyType, DataType> {
private:
    std::vector<std::shared_ptr<DataType>> _elements;

public:
    static std::shared_ptr<CollectAllOperation<KeyType, DataType>> getInstance() {
        static std::shared_ptr<CollectAllOperation<KeyType, DataType>> instance =
            std::make_shared<CollectAllOperation<KeyType, DataType>>();
        return instance;
    }

    void execute(std::shared_ptr<KDTreeNode<KeyType, DataType>> node) override {
        if (node && node->_data) {
            _elements.push_back(node->_data);
        }
    }

    const std::vector<std::shared_ptr<DataType>>& getElements() const {
        return _elements;
    }

    void reset() {
        _elements.clear();
    }

private:
    CollectAllOperation() = default;
    CollectAllOperation(const CollectAllOperation&) = delete;
    CollectAllOperation& operator=(const CollectAllOperation&) = delete;
};






