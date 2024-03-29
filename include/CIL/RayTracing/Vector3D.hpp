#ifndef CIL_VECTOR_3D_H
#define CIL_VECTOR_3D_H

#include <CIL/Color.hpp>
#include <CIL/Core/Utils.hpp>
#include <CIL/Drawing.hpp>
#include <cmath>
#include <iostream>
#include <vector>

namespace CIL {

    class Vector3D
    {
      public:
        Vector3D() : coord{0, 0, 0} {}
        Vector3D(double x, double y, double z) : coord{x, y, z} {}
        Vector3D(const CIL::ColorMap& c)
        {
            coord.resize(c.numComponents());
            for (auto i = 0U; i < c.numComponents(); i++)
                coord[i] = c[i];
        }

        double& x() { return coord[0]; }
        double& y() { return coord[1]; }
        double& z() { return coord[2]; }

        const double& x() const { return coord[0]; }
        const double& y() const { return coord[1]; }
        const double& z() const { return coord[2]; }

        Vector3D operator-() const
        {
            return Vector3D(-coord[0], -coord[1], -coord[2]);
        }
        double operator[](int i) const { return coord[i]; }
        double& operator[](int i) { return coord[i]; }

        Vector3D& operator+=(const Vector3D& v)
        {
            coord[0] += v.coord[0];
            coord[1] += v.coord[1];
            coord[2] += v.coord[2];
            return *this;
        }

        Vector3D& operator*=(const double t)
        {
            coord[0] *= t;
            coord[1] *= t;
            coord[2] *= t;
            return *this;
        }

        Vector3D getInRange0to1();

        Vector3D& operator/=(const double t) { return *this *= 1 / t; }
        bool operator==(const Vector3D& v) const { return coord == v.coord; }

        double magnitude() const { return sqrt(magnitude_squared()); }

        double magnitude_squared() const
        {
            return coord[0] * coord[0] + coord[1] * coord[1] +
                   coord[2] * coord[2];
        }

        inline static Vector3D random(double min = 0, double max = 1)
        {
            return Vector3D(utils::random_double(min, max),
                            utils::random_double(min, max),
                            utils::random_double(min, max));
        }

        inline static Vector3D randomInUnitSphere()
        {
            while (true)
            {
                Vector3D p = Vector3D::random(-1, 1);
                if (p.magnitude_squared() >= 1)
                    continue;
                return p;
            }
        }

        bool near_zero() const
        {
            // Return true if the vector is close to zero in all dimensions.
            const auto s = 1e-8;
            return (fabs(coord[0]) < s) && (fabs(coord[1]) < s) &&
                   (fabs(coord[2]) < s);
        }

      public:
        std::vector<double> coord;
    };

    // Type aliases for Vec3D
    using Point3D = Vector3D; // 3D point

    // vec3 Utility Functions
    inline std::ostream& operator<<(std::ostream& out, const Vector3D& v)
    {
        return out << v.coord[0] << ' ' << v.coord[1] << ' ' << v.coord[2];
    }

    inline Vector3D operator+(const Vector3D& u, const Vector3D& v)
    {
        return Vector3D(u.coord[0] + v.coord[0], u.coord[1] + v.coord[1],
                        u.coord[2] + v.coord[2]);
    }

    inline Vector3D operator-(const Vector3D& u, const Vector3D& v)
    {
        return Vector3D(u.coord[0] - v.coord[0], u.coord[1] - v.coord[1],
                        u.coord[2] - v.coord[2]);
    }

    inline Vector3D operator*(const Vector3D& u, const Vector3D& v)
    {
        return Vector3D(u.coord[0] * v.coord[0], u.coord[1] * v.coord[1],
                        u.coord[2] * v.coord[2]);
    }

    inline Vector3D operator*(double t, const Vector3D& v)
    {
        return Vector3D(t * v.coord[0], t * v.coord[1], t * v.coord[2]);
    }

    inline Vector3D operator+(double t, const Vector3D& v)
    {
        return Vector3D(t + v.coord[0], t + v.coord[1], t + v.coord[2]);
    }

    inline Vector3D operator*(const Vector3D& v, double t) { return t * v; }
    inline Vector3D operator+(const Vector3D& v, double t) { return t + v; }

    inline Vector3D operator/(Vector3D v, double val) { return (1 / val) * v; }

    inline double dot(const Vector3D& u, const Vector3D& v)
    {
        return u.coord[0] * v.coord[0] + u.coord[1] * v.coord[1] +
               u.coord[2] * v.coord[2];
    }

    inline Vector3D cross(const Vector3D& u, const Vector3D& v)
    {
        return Vector3D(u.coord[1] * v.coord[2] - u.coord[2] * v.coord[1],
                        u.coord[2] * v.coord[0] - u.coord[0] * v.coord[2],
                        u.coord[0] * v.coord[1] - u.coord[1] * v.coord[0]);
    }

    inline Vector3D unit_vector(Vector3D v) { return v / v.magnitude(); }

    inline double compute_distance(const Point3D p1, const Point3D p2)
    {
        auto p = p2 - p1;
        return p.magnitude();
    }

    inline Vector3D Vector3D::getInRange0to1()
    {
        return (unit_vector(*this) + 1.0) * 0.5;
    }

} // namespace CIL

#endif
