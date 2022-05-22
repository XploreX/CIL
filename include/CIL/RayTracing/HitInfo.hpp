#ifndef CIL_RAY_TRACING_HIT_INFO_HPP
#define CIL_RAY_TRACING_HIT_INFO_HPP

#include <CIL/RayTracing/Ray.hpp>
#include <CIL/RayTracing/Vector3D.hpp>
#include <limits>

namespace CIL {
    class HitInfo
    {
      public:
        HitInfo() = default;
        Point3D hit_point;
        Vector3D normal;
        bool front_face;
        ColorMap color = ColorMap({0, 0, 0});
        double distance = std::numeric_limits<double>::infinity();
        bool is_valid()
        {
            return distance != std::numeric_limits<double>::infinity();
        }
        inline void set_normal(const Ray& r, const Vector3D& outward_normal)
        {
            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
            // show(outward_normal.magnitude());
        }
    };
} // namespace CIL
#endif
