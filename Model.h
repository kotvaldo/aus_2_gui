#pragma once
#include <memory>
template <typename Key>
class Model {
    virtual std::shared_ptr<Key> getKey();
};
