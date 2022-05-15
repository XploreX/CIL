#ifndef CIL_RAY_TRACING_HIT_INFO_HPP
#define CIL_RAY_TRACING_HIT_INFO_HPP

#include <CIL/RayTracing/Vector3D.hpp>
#include <limits>

namespace CIL {
    class HitInfo
    {
      public:
        HitInfo() = default;
        Point3D hit_point;
        ColorMap color = ColorMap({0, 0, 0});
        double distance = std::numeric_limits<double>::infinity();
        bool is_valid()
        {
            return distance != std::numeric_limits<double>::infinity();
        }
    };
} // namespace CIL
#endif