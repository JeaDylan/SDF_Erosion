#ifndef BEZIER_H
#define BEZIER_H
#include <vector>
#include "mesh.h"

class Bezier {
public:
    Bezier(int n, int m, const std::vector<std::vector<Vector>>& controlPoints)
        : n_(n), m_(m), controlPoints_(controlPoints) {}

    Vector CalculateBezierPoint(double u, double v) const;
    Vector CalculateBezierPoint(double u) const;
    Mesh Polygonize(int resolution) const;

    // Normal
    virtual Vector Normal(const Vector&) const;
    virtual Vector Gradient(const Vector& p) const;

protected:
    static const double Epsilon;
    int n_;  // Degré en u
    int m_;  // Degré en v
    std::vector<std::vector<Vector>> controlPoints_;
};

class Revolution {
public:
    Revolution(const std::vector<Vector>& profileCurve, const Vector& axis)
        : profileCurve_(profileCurve), axis_(axis) {}

    Mesh Polygonize(int resolution) const;
    Vector CalculatePoint(double t) const;
    Vector CalculateTangent(int i, double t) const;
    double CalculateParameter(const Vector& point) const;

    // Normal
    virtual Vector Normal(const Vector&) const;
    virtual Vector Gradient(const Vector& p) const;

private:
    std::vector<Vector> profileCurve_;
    Vector axis_;
};

class Twist {
public:
    Twist(double twistAngle) : twistAngle_(twistAngle) {}

    Vector Warp(const Vector& point) const {
        double angle = twistAngle_ * point[1];  // Variation de l'angle en fonction de la coordonnée y
        double x = point[0] * cos(angle) - point[2] * sin(angle);
        double y = point[1];
        double z = point[0] * sin(angle) + point[2] * cos(angle);

        return Vector(x, y, z);
    }

    void Warp(Mesh& mesh) const {
        for (size_t i = 0; i < mesh.Vertexes(); ++i) {
            Vector deformedPoint = Warp(mesh.vertices[i]);
            mesh.vertices[i] = deformedPoint;
        }
    }

private:
    double twistAngle_;
};

#endif // BEZIER_H
