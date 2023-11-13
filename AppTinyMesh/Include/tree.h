#ifndef TREE_H
#define TREE_H
#include "node.h"

class Tree : public Node
{
public:
    Tree();
    Tree(Node*);
    double Value(const Vector&) const;
protected:
    Node* root;
};

#endif // TREE_H
