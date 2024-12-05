#pragma once

#include <bits/shared_ptr.h>

// CRATE DP

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
