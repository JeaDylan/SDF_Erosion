#include "primitives.h"

double g(const double& a, const double &b, const double& r) {
    double h = std::max(r-abs(b-a), 0.0)/r;
    return r*pow(h, 3.0)/6.0;
}

Tree::Tree()
{
}

Tree::Tree(Node* n)
{
    root = n;
}

double Tree::Value(const Vector& p) const
{
    return root->Value(p);
}

bool Tree::Intersect(const Ray& r,double& dist) const
{
    Vector o = r.Origin();
    Vector d = r.Direction();
    Vector v = o;
    double f = 0;
    std::cout<<"value : "<<Value(v)<<std::endl;
    f = Value(v);
    dist = 0.0;

    while (f >= 0.01)
    {
        std::cout<<f<<std::endl;
        dist += f;
        v += f * d;
        f = Value(v);
        if (f > 100)
        {
            return false;
        }
    }
    std::cout<<"dist : "<<dist<<std::endl;
    return true;
}


Tree::~Tree()
{

}


Sphere::Sphere(Vector _c, float _r)
{
    c = _c;
    r = _r;
}

Sphere::Sphere()
{
    Sphere(Vector(0.0), 0.000001);
}

double Sphere::Value(const Vector & p) const
{
    return Norm(p-c) - r;
}

Capsule::Capsule(Vector _a, Vector _b, float _r)
{
    a = _a;
    b = _b;
    r = _r;
}
double Capsule::Value(const Vector& p) const
{
    Vector pa = p - a;
    Vector ba = b - a;
    float h = Math::Clamp( (pa*ba)/(ba*ba), 0.0, 1.0 );
    return Norm( pa - ba*h ) - r;
}

SquareBox::SquareBox(Vector _a, Vector _b)
{
    a = _a;
    b = _b;
}
double SquareBox::Value(const Vector& p) const{
    Vector ab = b-a;
    Vector ap = p-a;
    Vector q = Abs(ap) - ab;
    return Norm(Vector::Max(q,Vector(0.0))) + std::min(std::max(q[0],std::max(q[1],q[2])),0.0);
}

Torus::Torus(Vector _c, float _r, float _r2, Vector _n)
{
    c = _c;
    r = _r;
    r2 = _r2;
    n = _n;
}

double Torus::Value(const Vector& p) const
{
    double h = n*(p-c);
    double l = sqrt(Norm(p-c)*Norm(p-c)-h*h);
    double m = l-r;
    return sqrt(m*m+h*h) - r2;
}

Union::Union(Node* _a, Node* _b)
{
    a = _a;
    b = _b;
}

double Union::Value(const Vector& p) const
{
    return std::min(a->Value(p), b->Value(p));
}

UnionSmooth::UnionSmooth(Node* _a, Node* _b)
{
    a = _a;
    b = _b;
}

double UnionSmooth::Value(const Vector& p) const
{
    return std::min(a->Value(p), b->Value(p)) - g(a->Value(p), b->Value(p), .40);
}

Intersection::Intersection(Node* _a, Node* _b)
{
    a = _a;
    b = _b;
}

double Intersection::Value(const Vector& p) const
{
    return std::max(a->Value(p), b->Value(p));
}

IntersectionSmooth::IntersectionSmooth(Node* _a, Node* _b)
{
    a = _a;
    b = _b;
}

double IntersectionSmooth::Value(const Vector& p) const
{
    return std::min(a->Value(p), b->Value(p)) - g(a->Value(p), b->Value(p), .40);
}

Difference::Difference(Node* _a, Node* _b)
{
    a = _a;
    b = _b;
}

double Difference::Value(const Vector& p) const
{
    return std::max(a->Value(p), 0 - b->Value(p));
}

DifferenceSmooth::DifferenceSmooth(Node* _a, Node* _b)
{
    a = _a;
    b = _b;
}

double DifferenceSmooth::Value(const Vector& p) const
{
    return std::max(a->Value(p), 0 - b->Value(p)) + g(a->Value(p), b->Value(p), .40);
}
