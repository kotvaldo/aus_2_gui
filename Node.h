template<typename KeyType, typename DataType>
struct KDTreeNode
{
    KDTreeNode *parent;
    KDTreeNode *_left;
    KDTreeNode *_right;
    int _level;
    DataType *_data;
    KeyType *_keyPart;

    KDTreeNode(DataType *data, KeyType *keys, int level = 0)
        : _data(data)
        , _keyPart(keys)
        , _level(level)
        , parent(nullptr)
        , _left(nullptr)
        , _right(nullptr)
    {}
};
