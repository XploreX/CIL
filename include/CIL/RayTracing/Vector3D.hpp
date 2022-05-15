#ifndef CIL_VECTOR_3D_H
#define CIL_VECTOR_3D_H

#include <CIL/Drawing.hpp>
#include <cmath>
#include <iostream>

namespace CIL {
    class Vector3D
    {
      public:
        Vector3D() : coord{0, 0, 0} {}
        Vector3D(double x, double y, double z) : coord{x, y, z} {}

        double x() const { return coord[0]; }
        double y() const { return coord[1]; }
        double z() const { return coord[2]; }

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

        Vector3D& operator/=(const double t) { return *this *= 1 / t; }

        double magnitude() const { return sqrt(magnitude_squared()); }

        double magnitude_squared() const
        {
            return coord[0] * coord[0] + coord[1] * coord[1] +
                   coord[2] * coord[2];
        }

      public:
        double coord[3];
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

    inline Vector3D operator*(const Vector3D& v, double t) { return t * v; }

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
} // namespace CIL

#endif
