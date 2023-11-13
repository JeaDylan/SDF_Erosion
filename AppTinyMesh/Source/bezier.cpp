#include "bezier.h"
#include "math.h"

const double Bezier::Epsilon = 1e-6;

int BinomialCoefficient(int n, int k) {
    // Calcule le coefficient binomial (n choose k)
    // Utilisez une approche récursive pour plus de clarté
    if (k == 0 || k == n) {
        return 1;
    } else {
        return BinomialCoefficient(n - 1, k - 1) + BinomialCoefficient(n - 1, k);
    }
}

double Bernstein(int n, int i, double t) {
    return BinomialCoefficient(n, i) * pow(t, i) * pow(1 - t, n - i);
}

Vector Rotation(const Vector& point, const Vector& axis, const double& angle)
{
    double c = cos(angle);
    double s = sin(angle);

    std::vector<std::vector<double>> rotationMatrix{
        {axis[0]*axis[0]*(1-c) + c   ,   axis[0]*axis[1]*(1-c)-axis[2]*s   ,   axis[0]*axis[2]*(1-c)+axis[1]*s},
        {axis[0]*axis[1]*(1-c)+axis[2]*s   ,   axis[1]*axis[1]*(1-c)+c   ,   axis[1]*axis[2]*(1-c)-axis[0]*s},
        {axis[0]*axis[2]*(1-c)-axis[1]*s   ,   axis[1]*axis[2]*(1-c)+axis[0]*s   ,   axis[2]*axis[2]*(1-c)+c}
    };
    Vector result;
    result[0] = rotationMatrix[0][0] * point[0] + rotationMatrix[0][1] * point[1] + rotationMatrix[0][2] * point[2];
    result[1] = rotationMatrix[1][0] * point[0] + rotationMatrix[1][1] * point[1] + rotationMatrix[1][2] * point[2];
    result[2] = rotationMatrix[2][0] * point[0] + rotationMatrix[2][1] * point[1] + rotationMatrix[2][2] * point[2];
    return result;
}

Mesh Bezier::Polygonize(int resolution) const {
    std::vector<Vector> vertex;
    std::vector<Vector> normal;

    std::vector<int> triangle;

    // Pour chaque paramètre u et v
    for (int i = 0; i <= resolution; ++i) {
        double u = i / static_cast<double>(resolution);

        for (int j = 0; j <= resolution; ++j) {
            double v = j / static_cast<double>(resolution);

            // Calculer le point sur la surface de Bézier pour les paramètres u et v
            Vector point = CalculateBezierPoint(u, v);

            // Ajouter le point au maillage
            vertex.push_back(point);
            normal.push_back(Normal(point));

            // Si ce n'est pas le dernier point de la ligne ou de la colonne,
            // Ajouter deux triangles formant un quadrilatère
            if (i < resolution && j < resolution) {
                int currentIndex = i * (resolution + 1) + j;
                int nextIndexU = (i + 1) * (resolution + 1) + j;
                int nextIndexV = i * (resolution + 1) + (j + 1);
                int nextIndexUV = (i + 1) * (resolution + 1) + (j + 1);

                triangle.push_back(currentIndex);
                triangle.push_back(nextIndexU);
                triangle.push_back(nextIndexUV);
                triangle.push_back(currentIndex);
                triangle.push_back(nextIndexUV);
                triangle.push_back(nextIndexV);
            }
        }
    }

    std::vector<int> normals = triangle;

    Mesh mesh(vertex, normal, triangle, normals);
    return mesh;
}

/*!
\brief Compute the gradient of the field.
\param p Point.
*/
Vector Bezier::Gradient(const Vector& p) const
{
    // Initialisation du vecteur gradient
    Vector gradient(0.0, 0.0, 0.0);

    // Calcul des dérivées partielles par rapport à u et v
    for (int i = 0; i <= n_; ++i) {
        for (int j = 0; j <= m_; ++j) {
            // Calculer les dérivées partielles par rapport à u et v pour chaque composante (x, y, z)
            Vector controlPoint = controlPoints_[i][j];
            Vector partialU = n_ * (controlPoint - p) * Bernstein(n_, i, 1.0/n_) * Bernstein(m_, j, 1.0/m_);
            Vector partialV = m_ * (controlPoint - p) * Bernstein(n_, i, 1.0/n_) * Bernstein(m_, j, 1.0/m_);

            // Ajouter les dérivées partielles au gradient
            gradient += partialU + partialV;
        }
    }

    return gradient;
}


/*!
\brief Compute the normal to the surface.

\sa Node::Gradient(const Vector&) const

\param p Point (should be on the surface).
*/
Vector Bezier::Normal(const Vector& p) const
{
  Vector normal = Normalized(Gradient(p));

  return normal;
}

Vector Bezier::CalculateBezierPoint(double u, double v) const {
    Vector point(0.0, 0.0, 0.0);

    for (int i = 0; i <= n_; ++i) {
        for (int j = 0; j <= m_; ++j) {
            // Calculer les coefficients de Bézier
            double coeffU = BinomialCoefficient(n_, i) * pow(1 - u, n_ - i) * pow(u, i);
            double coeffV = BinomialCoefficient(m_, j) * pow(1 - v, m_ - j) * pow(v, j);

            // Ajouter le point de contrôle pondéré à la position finale
            point += coeffU * coeffV * controlPoints_[i][j];
        }
    }

    return point;
}

Vector Bezier::CalculateBezierPoint(double u) const
{
    if (controlPoints_.size() == 1)
    {
        std::vector<Vector> points(controlPoints_[0].begin(), controlPoints_[0].end());

        while (points.size() > 1)
        {
            std::vector<Vector> newPoints(points.size() - 1);
            for (size_t i = 0; i < points.size() - 1; i++) {
                newPoints[i] = (1 - u) * points[i] + u * points[i + 1];
            }
            points = newPoints;
        }

        return points[0];
    }
}

Mesh Revolution::Polygonize(int resolution) const {

    std::vector<Vector> vertex;
    std::vector<Vector> normal;

    std::vector<int> triangle;

    std::vector<std::vector<Vector>> profile{profileCurve_};
    Bezier b(1, profileCurve_.size()-1, profile);

    // Pour chaque paramètre theta (angle de rotation autour de l'axe)
        for (int i = 0; i <= resolution; ++i) {
            double theta = (2 * M_PI * i) / static_cast<double>(resolution);

            // Pour chaque point sur la courbe de Bézier
            for (int j = 0; j <= resolution; ++j) {
                double u = j / static_cast<double>(resolution);

                // Calculer le point sur la courbe de Bézier pour le paramètre u
                Vector profilePoint = b.CalculateBezierPoint(u);

                // Effectuer la rotation du point autour de l'axe pour obtenir un nouveau point
                // Effectuer la rotation du point autour de l'axe pour obtenir un nouveau point
                double x = profilePoint[0] * cos(theta) - profilePoint[1] * sin(theta);
                double y = profilePoint[0] * sin(theta) + profilePoint[1] * cos(theta);
                double z = profilePoint[2];
                Vector point(x, y, z);

                // Ajouter le point au maillage
                vertex.push_back(point);
                normal.push_back(Normal(point));

                // Si ce n'est pas le dernier point de la courbe de Bézier, ajouter deux triangles formant un quadrilatère
                if (j < resolution) {
                    int currentIndex = i * (resolution + 1) + j;
                    int nextIndexTheta = ((i + 1) % (resolution + 1)) * (resolution + 1) + j;
                    int nextIndexPhi = i * (resolution + 1) + (j + 1);
                    int nextIndexThetaPhi = ((i + 1) % (resolution + 1)) * (resolution + 1) + (j + 1);

                    triangle.push_back(currentIndex);
                    triangle.push_back(nextIndexTheta);
                    triangle.push_back(nextIndexThetaPhi);
                    triangle.push_back(currentIndex);
                    triangle.push_back(nextIndexThetaPhi);
                    triangle.push_back(nextIndexPhi);
                }
            }
        }
    std::vector<int> normals = triangle;

    Mesh mesh(vertex, normal, triangle, normals);
    return mesh;
}

/*!
\brief Compute the gradient of the field.
\param p Point.
*/
Vector Revolution::Gradient(const Vector& p) const
{
    // Initialisation du vecteur gradient
    Vector gradient(0.0, 0.0, 0.0);

    // Calcul des dérivées partielles par rapport à u et v
    for (int i = 0; i <= profileCurve_.size(); ++i) {
        // Calculer les dérivées partielles par rapport à u et v pour chaque composante (x, y, z)
        Vector controlPoint = profileCurve_[i];
        double t = CalculateParameter(p);
        Vector tangent = CalculateTangent(i, t);

        Vector partialT = (controlPoint - p) / tangent;

        // Ajouter les dérivées partielles au gradient
        gradient += partialT;
    }

    return gradient;
}

Vector Revolution::CalculatePoint(double t) const {
    // La position sur la courbe de Bézier est la somme pondérée des points de contrôle
    // Utilisez la formule de la courbe de Bézier pour calculer la position

    Vector point(0.0, 0.0, 0.0);
    int degree = profileCurve_.size();
    for (int i = 0; i <= degree; ++i) {
        double blend = Bernstein(degree, i, t);
        point += blend * profileCurve_[i];
    }

    return point;
}

double Revolution::CalculateParameter(const Vector& point) const {
    double t = 0.5;
    double epsilon = 1e-6;

    // Recherche binaire pour trouver une valeur approximative de t
    for (int iter = 0; iter < 100; ++iter) {
        Vector pointOnCurve = CalculatePoint(t);
        if (Norm(pointOnCurve - point) < epsilon) {
            break;
        }

        // Ajuster t selon la différence entre le point actuel et le point souhaité
        t -= (pointOnCurve[0] - point[0]) / CalculateTangent(0, t)[0];
    }

    return t;
}

Vector Revolution::CalculateTangent(int i, double t) const {
    // La dérivée de la courbe de Bézier par rapport à t donne le vecteur tangent
    // Vous devrez adapter cela en fonction de votre modèle de courbe de Bézier
    // Ici, je suppose que chaque point de contrôle est une instance de la classe Vector

    Vector tangent(0.0, 0.0, 0.0);
    int degree = profileCurve_.size();
    if (i > 0 && i < degree) {
        tangent = degree * (profileCurve_[i + 1] - profileCurve_[i - 1]);
    } else if (i == 0) {
        tangent = degree * (profileCurve_[i + 1] - profileCurve_[i]);
    } else if (i == degree) {
        tangent = degree * (profileCurve_[i] - profileCurve_[i - 1]);
    }

    return Normalized(tangent); // Normalisation du vecteur tangent
}

/*!
\brief Compute the normal to the surface.

\sa Node::Gradient(const Vector&) const

\param p Point (should be on the surface).
*/
Vector Revolution::Normal(const Vector& p) const
{
  Vector normal = Normalized(Gradient(p));

  return normal;
}
