#ifndef CIL_RAY_TRACING_RAY_HPP
#define CIL_RAY_TRACING_RAY_HPP

#include <CIL/RayTracing/Vector3D.hpp>

namespace CIL {
    class Ray
    {
      public:
        Ray() {}
        Ray(const Point3D& origin, const Vector3D& direction)
            : orig(origin), dir(direction)
        {}

        Point3D origin() const { return orig; }
        Vector3D direction() const { return dir; }

        Point3D at(double t) const { return orig + t * dir; }
        double length(double t) const
        {
            Point3D p = at(t);
            return compute_distance(p, orig);
        }

      public:
        Point3D orig;
        Vector3D dir;
    };
} // namespace CIL

#endif
