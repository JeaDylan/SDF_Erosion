#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "node.h"

class Tree : public Node
{
public:
    Tree();
    Tree(Node*);
    ~Tree();
    double Value(const Vector&) const;
    bool Intersect(const Ray&, double&) const;

    Node* root;
};

class Sphere : public Node {
public:
    Sphere();
    Sphere(Vector _c, float _r);
    double Value(const Vector& p) const;
protected:
    Vector c;
    float r;
};

class Capsule : public Node {
public:
    Capsule(Vector _a, Vector _b, float _r);
    double Value(const Vector& p) const;
protected:
    Vector a;
    Vector b;
    float r;
};

class SquareBox : public Node {
public:
    SquareBox(Vector _a, Vector _b);
    double Value(const Vector& p) const;
protected:
    Vector a;
    Vector b;
};

class Torus : public Node {
public :
    Torus(Vector _c, float _r, float _r2, Vector _n);
    double Value(const Vector& p) const;
protected:
    Vector c;
    float r;
    float r2;
    Vector n;
};

class Union : public Node {
protected:
    Node* a;
    Node* b;
public:
    Union() {};
    Union(Node* _a, Node* _b);
    double Value(const Vector& p) const;
};

class UnionSmooth : public Node {
protected:
    Node* a;
    Node* b;
public:
    UnionSmooth() {};
    UnionSmooth(Node* _a, Node* _b);
    double Value(const Vector& p) const;
};

class Intersection : public Node {
protected:
    Node* a;
    Node* b;
public:
    Intersection() {};
    Intersection(Node* _a, Node* _b);
    double Value(const Vector& p) const;
};

class IntersectionSmooth : public Node {
protected:
    Node* a;
    Node* b;
public:
    IntersectionSmooth() {};
    IntersectionSmooth(Node* _a, Node* _b);
    double Value(const Vector& p) const;
};

class Difference : public Node {
protected:
    Node* a;
    Node* b;
public:
    Difference(Node* _a, Node* _b);
    Difference() {};
    double Value(const Vector& p) const;
};

class DifferenceSmooth : public Node {
protected:
    Node* a;
    Node* b;
public:
    DifferenceSmooth() {};
    DifferenceSmooth(Node* _a, Node* _b);
    double Value(const Vector& p) const;
};
#endif // PRIMITIVES_H
