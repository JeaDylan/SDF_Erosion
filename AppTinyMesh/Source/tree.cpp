#include "tree.h"
#include "node.h"

Tree::Tree()
{
}

Tree::Tree(Node* n)
{
    root = n;
}

double Tree::Value(const Vector& p) const
{
    return root.Value(p);
}
